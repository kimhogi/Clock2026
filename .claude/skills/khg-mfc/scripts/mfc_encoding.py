#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
mfc_encoding.py — MFC 소스 파일의 원본 인코딩을 보존/복구하기 위한 코어 엔진. (khg-mfc 번들)

위치 무관: 프로젝트 경로는 환경변수 CLAUDE_PROJECT_DIR(훅 실행 시 주입) 또는 cwd 기준.
스크립트 자기 위치(__file__)에 의존하지 않으므로 ${CLAUDE_SKILL_DIR}/scripts 에 있어도 동작.

다중 코드페이지: 한글 949(기본), 일본어 932(Shift-JIS), 중국어 간체 936(GBK).
  환경변수 MFC_CODEPAGE=932 또는 서브커맨드 인자 --cp 932 로 변경.

서브커맨드:
  detect <file> [--cp N]      인코딩 토큰 (ascii|utf-8|utf-8-bom|utf-16le|utf-16be|mbcs|binary)
  manifest [paths...] [--cp N] 소스 스캔 -> <project>/.claude/mfc-encodings.json (인벤토리)
  guard                        PreToolUse 훅: 비-UTF8 직접 편집 차단, UTF-8+BOM은 상태기록 후 허용
  restore-bom                  PostToolUse 훅: 편집으로 빠진 UTF-8 BOM 복원
  begin <file> [--cp N]        안전 편집 시작: 원본 인코딩 백업+기록 후 UTF-8(BOM없음)로 변환
  end <file>                   안전 편집 종료: 원본 인코딩으로 복원(+BOM, round-trip 검증)
  scan <file> [--cp N]         손상 스캔: U+FFFD / '????'(0x3F 4+연속) 검출
  recover <corrupt> <backup> [--cp N]  difflib 복구: 깨끗한 .bak 한글 + 현재 ASCII 코드 병합

훅(guard/restore-bom)은 항상 exit 0 (결정은 JSON; 내부 오류 시 fail-open).
"""
import sys, os, json, time, difflib

SRC_EXTS = {
    ".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".hxx", ".inl",
    ".rc", ".rc2", ".idl", ".odl", ".def", ".sln", ".vcxproj", ".filters",
}
DENY_ENCODINGS = {"mbcs", "utf-16le", "utf-16be", "binary"}

def codepage(argv=None):
    if argv:
        for i, a in enumerate(argv):
            if a == "--cp" and i + 1 < len(argv):
                try: return int(argv[i + 1])
                except ValueError: pass
    env = os.environ.get("MFC_CODEPAGE")
    return int(env) if env and env.isdigit() else 949

def _codec(cp): return "cp%d" % cp
def _pos(argv): return [x for x in argv if not x.startswith("--") and not x.isdigit()]

# ---------------------------------------------------------------- 경로
def project_dir(): return os.environ.get("CLAUDE_PROJECT_DIR") or os.getcwd()
def manifest_path(): return os.path.join(project_dir(), ".claude", "mfc-encodings.json")
def state_dir():
    d = os.path.join(project_dir(), ".claude", ".enc-state"); os.makedirs(d, exist_ok=True); return d
def rel(p):
    try: return os.path.relpath(os.path.abspath(p), project_dir()).replace(os.sep, "/")
    except Exception: return p
def _key(p): return os.path.abspath(p).replace(os.sep, "_").replace(":", "_").replace("/", "_")
def begin_sidecar(p): return os.path.join(state_dir(), _key(p) + ".begin")
def bom_sidecar(p): return os.path.join(state_dir(), _key(p) + ".bom")
def self_cmd(): return '"%s"' % os.path.abspath(__file__)

# ---------------------------------------------------------------- 탐지/변환
def detect_bytes(data, cp=949):
    if data.startswith(b"\xef\xbb\xbf"): return "utf-8-bom"
    if data.startswith(b"\xff\xfe"): return "utf-16le"
    if data.startswith(b"\xfe\xff"): return "utf-16be"
    if data == b"": return "ascii"
    try: data.decode("ascii"); return "ascii"
    except Exception: pass
    try: data.decode("utf-8"); return "utf-8"
    except Exception: pass
    try: data.decode(_codec(cp)); return "mbcs"
    except Exception: pass
    return "binary"

def detect_file(p, cp=949):
    with open(p, "rb") as f: return detect_bytes(f.read(), cp)

def decode_to_text(data, tok, cp=949):
    if tok == "utf-8-bom": return data[3:].decode("utf-8")
    if tok == "utf-16le": return data[2:].decode("utf-16-le")
    if tok == "utf-16be": return data[2:].decode("utf-16-be")
    if tok == "mbcs": return data.decode(_codec(cp))
    if tok in ("utf-8", "ascii"): return data.decode("utf-8")
    raise ValueError("cannot decode binary")

def encode_from_text(text, tok, cp=949):
    if tok == "utf-8-bom": return b"\xef\xbb\xbf" + text.encode("utf-8")
    if tok == "utf-16le": return b"\xff\xfe" + text.encode("utf-16-le")
    if tok == "utf-16be": return b"\xfe\xff" + text.encode("utf-16-be")
    if tok == "mbcs": return text.encode(_codec(cp))
    return text.encode("utf-8")

def unencodable_chars(text, cp):
    codec = _codec(cp); bad = []
    for i, ch in enumerate(text):
        try: ch.encode(codec)
        except Exception: bad.append("U+%04X@%d(%r)" % (ord(ch), i, ch))
    return bad

def backup(path):
    bp = "%s.bak.%s" % (path, time.strftime("%Y%m%d-%H%M%S"))
    with open(path, "rb") as s, open(bp, "wb") as d: d.write(s.read())
    return bp

# ---------------------------------------------------------------- 커맨드
def cmd_detect(argv):
    p = _pos(argv)
    if not p: print("usage: detect <file> [--cp N]", file=sys.stderr); return 2
    print(detect_file(p[0], codepage(argv))); return 0

def cmd_manifest(argv):
    cp = codepage(argv); root = project_dir(); paths = _pos(argv)
    if paths: files = [os.path.abspath(x) for x in paths]
    else:
        files = []
        for dp, dns, fns in os.walk(root):
            dns[:] = [d for d in dns if d not in (".git", ".enc-state", "node_modules")]
            for fn in fns:
                if os.path.splitext(fn)[1].lower() in SRC_EXTS: files.append(os.path.join(dp, fn))
    result = {}
    for fp in sorted(files):
        try: result[rel(fp)] = detect_file(fp, cp)
        except Exception as e: result[rel(fp)] = "error:" + type(e).__name__
    os.makedirs(os.path.dirname(manifest_path()), exist_ok=True)
    with open(manifest_path(), "w", encoding="utf-8") as f:
        json.dump(result, f, ensure_ascii=False, indent=2, sort_keys=True); f.write("\n")
    counts = {}
    for v in result.values(): counts[v] = counts.get(v, 0) + 1
    print("manifest: %d files -> %s (cp%d)" % (len(result), rel(manifest_path()), cp))
    for k in sorted(counts): print("  %-12s %d" % (k, counts[k]))
    return 0

def _hin():
    try: return json.load(sys.stdin)
    except Exception: return {}
def _hpath(d):
    fp = (d.get("tool_input") or {}).get("file_path")
    if not fp: return None
    return fp if os.path.isabs(fp) else os.path.join(os.getcwd(), fp)
def _allow(): print(json.dumps({"hookSpecificOutput": {"hookEventName": "PreToolUse", "permissionDecision": "allow"}}))
def _deny(r): print(json.dumps({"hookSpecificOutput": {"hookEventName": "PreToolUse", "permissionDecision": "deny", "permissionDecisionReason": r}}, ensure_ascii=False))

def cmd_guard(argv):
    cp = codepage(argv); path = _hpath(_hin())
    if not path or not os.path.exists(path): _allow(); return 0
    if os.path.exists(begin_sidecar(path)): _allow(); return 0
    try: enc = detect_file(path, cp)
    except Exception: _allow(); return 0
    if enc in DENY_ENCODINGS:
        r = rel(path)
        _deny("이 파일은 %s(NoBOM MBCS/UTF-16)입니다. 직접 Edit/Write 하면 한글이 깨져 원본이 손상됩니다.\n"
              "안전 편집:\n  1) python3 %s begin \"%s\"   (UTF-8 임시변환 + 자동 .bak)\n"
              "  2) Read/Edit 로 수정 (새 주석은 ASCII 권장)\n"
              "  3) python3 %s end \"%s\"     (원래 인코딩 복원 + round-trip)"
              % (enc, self_cmd(), r, self_cmd(), r))
    else:
        if enc == "utf-8-bom":
            try:
                with open(bom_sidecar(path), "w") as f: f.write("1")
            except Exception: pass
        _allow()
    return 0

def cmd_restore_bom(argv):
    path = _hpath(_hin())
    if not path or not os.path.exists(path): return 0
    if os.path.exists(begin_sidecar(path)):
        print("[mfc-encoding] '%s' begin 상태. 끝나면 `end` 실행." % rel(path), file=sys.stderr); return 0
    bs = bom_sidecar(path)
    if not os.path.exists(bs): return 0
    try:
        with open(path, "rb") as f: raw = f.read()
        if not raw.startswith(b"\xef\xbb\xbf"):
            raw.decode("utf-8")
            with open(path, "wb") as f: f.write(b"\xef\xbb\xbf" + raw)
            print("[mfc-encoding] '%s' UTF-8 BOM 재부착" % rel(path), file=sys.stderr)
    except Exception: pass
    finally:
        try: os.remove(bs)
        except Exception: pass
    return 0

def cmd_begin(argv):
    cp = codepage(argv); p = _pos(argv)
    if not p: print("usage: begin <file> [--cp N]", file=sys.stderr); return 2
    path = os.path.abspath(p[0])
    if not os.path.exists(path): print("없는 파일: %s" % path, file=sys.stderr); return 2
    enc = detect_file(path, cp)
    if enc == "binary": print("바이너리. 편집 비권장: %s" % rel(path), file=sys.stderr); return 2
    if enc in ("ascii", "utf-8"): print("이미 UTF-8(%s). 그냥 편집: %s" % (enc, rel(path))); return 0
    bp = backup(path)
    with open(path, "rb") as f: raw = f.read()
    with open(path, "wb") as f: f.write(decode_to_text(raw, enc, cp).encode("utf-8"))
    with open(begin_sidecar(path), "w", encoding="utf-8") as f: f.write("%s\t%d" % (enc, cp))
    print("begin: '%s' %s(cp%d) -> UTF-8. 백업 %s. 수정 후 `end`." % (rel(path), enc, cp, rel(bp))); return 0

def cmd_end(argv):
    p = _pos(argv)
    if not p: print("usage: end <file>", file=sys.stderr); return 2
    path = os.path.abspath(p[0]); sc = begin_sidecar(path)
    if not os.path.exists(sc): print("begin 상태 아님: %s" % rel(path), file=sys.stderr); return 2
    meta = open(sc, encoding="utf-8").read().strip().split("\t")
    enc = meta[0]; cp = int(meta[1]) if len(meta) > 1 else 949
    text = open(path, "rb").read().decode("utf-8")
    if enc == "mbcs":
        bad = unencodable_chars(text, cp)
        if bad:
            print("end 중단: cp%d 표현불가 %d개 (예: %s). 수정 후 다시 `end`. UTF-8 유지."
                  % (cp, len(bad), ", ".join(bad[:5])), file=sys.stderr); return 2
    out = encode_from_text(text, enc, cp)
    if decode_to_text(out, enc, cp) != text:
        print("end 중단: round-trip 불일치. UTF-8 유지.", file=sys.stderr); return 2
    with open(path, "wb") as f: f.write(out)
    os.remove(sc)
    print("end: '%s' UTF-8 -> %s(cp%d) 복원 + round-trip OK." % (rel(path), enc, cp)); return 0

def cmd_scan(argv):
    p = _pos(argv)
    if not p: print("usage: scan <file> [--cp N]", file=sys.stderr); return 2
    raw = open(p[0], "rb").read()
    ufffd = raw.count(b"\xef\xbf\xbd"); runs = 0; i = 0
    while i < len(raw):
        if raw[i] == 0x3F:
            j = i
            while j < len(raw) and raw[j] == 0x3F: j += 1
            if j - i >= 4: runs += 1
            i = j
        else: i += 1
    ok = (ufffd == 0 and runs == 0)
    print("scan %s: %s (U+FFFD=%d, '????'runs=%d)" % (rel(p[0]), "CLEAN" if ok else "CORRUPT", ufffd, runs))
    return 0 if ok else 1

def cmd_recover(argv):
    cp = codepage(argv); p = _pos(argv)
    if len(p) < 2: print("usage: recover <corrupt> <clean-backup> [--cp N]", file=sys.stderr); return 2
    corrupt, bak = p[0], p[1]
    bak_text = decode_to_text(open(bak, "rb").read(), detect_file(bak, cp), cp)
    cur_text = open(corrupt, "rb").read().decode("utf-8", errors="replace")
    cl, bl = cur_text.splitlines(keepends=True), bak_text.splitlines(keepends=True)
    sm = difflib.SequenceMatcher(None, [l.replace("�", "") for l in cl], bl, autojunk=False)
    out = []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "equal": out.extend(cl[i1:i2])
        elif tag in ("replace", "delete"):
            for k in range(i1, i2):
                if "�" in cl[k] and j1 < j2: out.append(bl[min(j1 + (k - i1), j2 - 1)])
                else: out.append(cl[k])
        elif tag == "insert": out.extend(bl[j1:j2])
    merged = "".join(out)
    if "�" in merged: print("경고: 병합 후 U+FFFD 잔존. 수동 확인.", file=sys.stderr)
    bp = backup(corrupt)
    with open(corrupt, "wb") as f: f.write(encode_from_text(merged, "mbcs", cp))
    print("recover: '%s' 복구 시도 완료(cp%d). 원본백업 %s. scan으로 확인." % (rel(corrupt), cp, rel(bp))); return 0

def main():
    cmds = {"detect": cmd_detect, "manifest": cmd_manifest, "guard": cmd_guard,
            "restore-bom": cmd_restore_bom, "begin": cmd_begin, "end": cmd_end,
            "scan": cmd_scan, "recover": cmd_recover}
    if len(sys.argv) < 2 or sys.argv[1] not in cmds:
        print("commands: " + ", ".join(cmds), file=sys.stderr); return 0
    cmd = sys.argv[1]
    try: rc = cmds[cmd](sys.argv[2:])
    except Exception as e:
        if cmd in ("guard", "restore-bom"):
            if cmd == "guard": _allow()
            print("[mfc-encoding] 내부 오류(fail-open): %r" % e, file=sys.stderr); return 0
        print("오류: %r" % e, file=sys.stderr); return 1
    return 0 if cmd in ("guard", "restore-bom") else rc

if __name__ == "__main__":
    sys.exit(main())
