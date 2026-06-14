#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
mfc_encoding.py — MFC 소스 파일의 원본 인코딩을 기억하고 보존하기 위한 코어 엔진.

배경:
  이 저장소(및 일반적인 한글 MFC 프로젝트)는 파일마다 인코딩이 섞여 있다.
    - CP949 (BOM 없음): 대부분의 .cpp/.h/.rc (한글 주석·문자열)
    - UTF-8 with BOM  : 일부 파일 (.vcxproj, 테스트 등)
    - ASCII           : 한글 없는 파일
  Claude Code의 Read/Edit/Write는 파일을 UTF-8로 다룬다. 그래서 CP949 파일을
  직접 편집하면 '읽는 순간' 한글이 U+FFFD로 깨지고, 저장 시 그대로 UTF-8로
  덮어써져 원본이 영구 손상된다(실험으로 확인됨). 또 UTF-8+BOM 파일은 편집 중
  BOM이 빠지면 MSVC가 한글 코드페이지로 오인한다.

이 스크립트가 제공하는 것:
  detect <file>        파일 인코딩 토큰 출력 (ascii|utf-8|utf-8-bom|utf-16le|utf-16be|cp949|binary)
  manifest [paths...]  소스 파일을 스캔해 .claude/mfc-encodings.json 생성/갱신
  guard                PreToolUse 훅: 비-UTF8(CP949/UTF-16/binary) 직접 편집을 차단
  restore-bom          PostToolUse 훅: 원래 UTF-8+BOM이던 파일에 BOM 재부착
  begin <file>         안전 편집 시작: 원본 인코딩을 기록하고 파일을 UTF-8(BOM없음)로 변환
  end <file>           안전 편집 종료: 기록된 원본 인코딩으로 되돌림(+BOM 복원)

훅은 항상 exit 0 으로 끝낸다(결정은 JSON으로 전달, 내부 오류 시 fail-open).
"""
import sys, os, json

SRC_EXTS = {
    ".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".hxx", ".inl",
    ".rc", ".rc2", ".idl", ".odl", ".def", ".sln", ".vcxproj", ".filters",
}
DENY_ENCODINGS = {"cp949", "utf-16le", "utf-16be", "binary"}  # 직접 편집 시 손상되는 인코딩

# ---------------------------------------------------------------- 경로/매니페스트
def repo_root():
    # 이 파일은 <root>/.claude/hooks/mfc_encoding.py 에 있다.
    return os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def manifest_path():
    return os.path.join(repo_root(), ".claude", "mfc-encodings.json")

def state_dir():
    d = os.path.join(repo_root(), ".claude", ".enc-state")
    os.makedirs(d, exist_ok=True)
    return d

def rel(path):
    try:
        return os.path.relpath(os.path.abspath(path), repo_root()).replace(os.sep, "/")
    except Exception:
        return path

def state_file_for(path):
    key = rel(path).replace("/", "__").replace(":", "_")
    return os.path.join(state_dir(), key + ".enc")

def load_manifest():
    try:
        with open(manifest_path(), "r", encoding="utf-8") as f:
            return json.load(f)
    except Exception:
        return {}

# ---------------------------------------------------------------- 인코딩 탐지/변환
def detect_bytes(data: bytes) -> str:
    if data.startswith(b"\xef\xbb\xbf"):
        return "utf-8-bom"
    if data.startswith(b"\xff\xfe"):
        return "utf-16le"
    if data.startswith(b"\xfe\xff"):
        return "utf-16be"
    if data == b"":
        return "ascii"
    for enc, token in (("ascii", "ascii"), ("utf-8", "utf-8"), ("cp949", "cp949")):
        try:
            data.decode(enc)
            return token
        except Exception:
            pass
    return "binary"

def detect_file(path: str) -> str:
    with open(path, "rb") as f:
        return detect_bytes(f.read())

def decode_to_text(data: bytes, token: str) -> str:
    if token == "utf-8-bom":
        return data[3:].decode("utf-8")
    if token == "utf-16le":
        return data[2:].decode("utf-16-le")
    if token == "utf-16be":
        return data[2:].decode("utf-16-be")
    if token == "cp949":
        return data.decode("cp949")
    if token in ("utf-8", "ascii"):
        return data.decode("utf-8")
    raise ValueError("cannot decode binary")

def encode_from_text(text: str, token: str) -> bytes:
    if token == "utf-8-bom":
        return b"\xef\xbb\xbf" + text.encode("utf-8")
    if token == "utf-16le":
        return b"\xff\xfe" + text.encode("utf-16-le")
    if token == "utf-16be":
        return b"\xfe\xff" + text.encode("utf-16-be")
    if token == "cp949":
        return text.encode("cp949")  # 매핑 불가 문자가 있으면 예외 → 호출부에서 잡음
    return text.encode("utf-8")

# ---------------------------------------------------------------- 서브커맨드
def cmd_detect(argv):
    if not argv:
        print("usage: detect <file>", file=sys.stderr); return 2
    print(detect_file(argv[0]))
    return 0

def cmd_manifest(argv):
    root = repo_root()
    paths = argv if argv else None
    result = {}
    if paths:
        files = [os.path.abspath(p) for p in paths]
    else:
        files = []
        for dirpath, dirnames, filenames in os.walk(root):
            dirnames[:] = [d for d in dirnames if d not in (".git", ".enc-state", "node_modules")]
            for fn in filenames:
                if os.path.splitext(fn)[1].lower() in SRC_EXTS:
                    files.append(os.path.join(dirpath, fn))
    for fp in sorted(files):
        try:
            result[rel(fp)] = detect_file(fp)
        except Exception as e:
            result[rel(fp)] = "error:" + type(e).__name__
    with open(manifest_path(), "w", encoding="utf-8") as f:
        json.dump(result, f, ensure_ascii=False, indent=2, sort_keys=True)
        f.write("\n")
    counts = {}
    for v in result.values():
        counts[v] = counts.get(v, 0) + 1
    print("manifest 작성: %d개 파일 -> %s" % (len(result), rel(manifest_path())))
    for k in sorted(counts):
        print("  %-12s %d" % (k, counts[k]))
    return 0

def _read_hook_input():
    try:
        return json.load(sys.stdin)
    except Exception:
        return {}

def _allow():
    print(json.dumps({"hookSpecificOutput": {
        "hookEventName": "PreToolUse", "permissionDecision": "allow"}}))

def _deny(reason):
    print(json.dumps({"hookSpecificOutput": {
        "hookEventName": "PreToolUse",
        "permissionDecision": "deny",
        "permissionDecisionReason": reason}}, ensure_ascii=False))

def cmd_guard(argv):
    """PreToolUse: 비-UTF8 파일의 직접 편집을 차단한다."""
    data = _read_hook_input()
    fp = (data.get("tool_input") or {}).get("file_path")
    if not fp:
        _allow(); return 0
    path = fp if os.path.isabs(fp) else os.path.join(os.getcwd(), fp)
    # 새로 만드는 파일이거나 begin 상태(이미 utf-8)면 통과
    if not os.path.exists(path):
        _allow(); return 0
    if os.path.exists(state_file_for(path)):
        _allow(); return 0
    try:
        enc = detect_file(path)
    except Exception:
        _allow(); return 0  # fail-open
    if enc in DENY_ENCODINGS:
        r = rel(path)
        _deny(
            "이 파일은 %s 인코딩입니다. 직접 Edit/Write 하면 한글이 깨져 원본이 손상됩니다.\n"
            "안전 편집 절차를 쓰세요:\n"
            "  1) python3 .claude/hooks/mfc_encoding.py begin \"%s\"   (UTF-8로 임시 변환)\n"
            "  2) 평소대로 Read/Edit 로 수정\n"
            "  3) python3 .claude/hooks/mfc_encoding.py end \"%s\"     (원래 %s 인코딩으로 복원)"
            % (enc, r, r, enc)
        )
    else:
        _allow()
    return 0

def cmd_restore_bom(argv):
    """PostToolUse: 원래 UTF-8+BOM 이던 파일이 편집 후 BOM을 잃었으면 다시 붙인다."""
    data = _read_hook_input()
    fp = (data.get("tool_input") or {}).get("file_path")
    if not fp:
        return 0
    path = fp if os.path.isabs(fp) else os.path.join(os.getcwd(), fp)
    if not os.path.exists(path):
        return 0
    # begin 상태면 end 호출을 잊지 말라고 알림(자동 변환은 하지 않음)
    if os.path.exists(state_file_for(path)):
        print("[mfc-encoding] '%s' 는 안전편집(begin) 상태입니다. 끝나면 `end` 로 원복하세요."
              % rel(path), file=sys.stderr)
        return 0
    original = load_manifest().get(rel(path))
    if original != "utf-8-bom":
        return 0
    try:
        with open(path, "rb") as f:
            raw = f.read()
        if raw.startswith(b"\xef\xbb\xbf"):
            return 0  # 이미 BOM 있음
        # 현재 내용이 정상 UTF-8이라고 보고 BOM만 부착
        raw.decode("utf-8")
        with open(path, "wb") as f:
            f.write(b"\xef\xbb\xbf" + raw)
        print("[mfc-encoding] '%s' UTF-8 BOM 재부착" % rel(path), file=sys.stderr)
    except Exception:
        pass  # fail-open
    return 0

def cmd_begin(argv):
    if not argv:
        print("usage: begin <file>", file=sys.stderr); return 2
    path = os.path.abspath(argv[0])
    if not os.path.exists(path):
        print("없는 파일: %s" % path, file=sys.stderr); return 2
    enc = detect_file(path)
    if enc == "binary":
        print("바이너리로 보입니다. 편집하지 않는 게 안전: %s" % rel(path), file=sys.stderr)
        return 2
    if enc in ("ascii", "utf-8"):
        print("이미 UTF-8(%s) 입니다. 그냥 편집하면 됩니다: %s" % (enc, rel(path)))
        return 0
    with open(path, "rb") as f:
        raw = f.read()
    text = decode_to_text(raw, enc)
    with open(path, "wb") as f:
        f.write(text.encode("utf-8"))  # BOM 없는 UTF-8 (Read/Edit가 깨끗하게 다룸)
    with open(state_file_for(path), "w", encoding="utf-8") as f:
        f.write(enc)
    print("begin: '%s' 를 %s -> UTF-8 로 변환했습니다. 수정 후 `end` 를 실행하세요." % (rel(path), enc))
    return 0

def cmd_end(argv):
    if not argv:
        print("usage: end <file>", file=sys.stderr); return 2
    path = os.path.abspath(argv[0])
    sf = state_file_for(path)
    if not os.path.exists(sf):
        print("begin 상태가 아닙니다(원복할 정보 없음): %s" % rel(path), file=sys.stderr)
        return 2
    with open(sf, "r", encoding="utf-8") as f:
        enc = f.read().strip()
    with open(path, "rb") as f:
        text = f.read().decode("utf-8")
    try:
        out = encode_from_text(text, enc)
    except UnicodeEncodeError as e:
        print("원복 실패: %s 로 표현할 수 없는 문자가 있습니다(%s). 파일을 UTF-8로 유지합니다."
              % (enc, e), file=sys.stderr)
        return 2
    with open(path, "wb") as f:
        f.write(out)
    os.remove(sf)
    print("end: '%s' 를 UTF-8 -> %s 로 복원했습니다." % (rel(path), enc))
    return 0

def main():
    cmds = {
        "detect": cmd_detect, "manifest": cmd_manifest,
        "guard": cmd_guard, "restore-bom": cmd_restore_bom,
        "begin": cmd_begin, "end": cmd_end,
    }
    if len(sys.argv) < 2 or sys.argv[1] not in cmds:
        print("commands: " + ", ".join(cmds), file=sys.stderr)
        return 0
    cmd = sys.argv[1]
    try:
        rc = cmds[cmd](sys.argv[2:])
    except Exception as e:
        # 훅(guard/restore-bom)은 절대 작업을 막지 않도록 fail-open
        if cmd in ("guard", "restore-bom"):
            if cmd == "guard":
                _allow()
            print("[mfc-encoding] 내부 오류(fail-open): %r" % e, file=sys.stderr)
            return 0
        print("오류: %r" % e, file=sys.stderr)
        return 1
    # 훅은 항상 0 (결정은 JSON으로 전달; || 폴백 중복실행 방지)
    if cmd in ("guard", "restore-bom"):
        return 0
    return rc

if __name__ == "__main__":
    sys.exit(main())
