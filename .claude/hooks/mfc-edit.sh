#!/usr/bin/env bash
# MFC 안전 편집 / 인코딩 래퍼 (Linux / macOS).
# 핵심 로직은 mfc_encoding.py 에 있고, 이 래퍼는 python3/python 중 가능한 것으로 호출한다.
#
# 사용:
#   .claude/hooks/mfc-edit.sh detect   <file>     # 인코딩 확인
#   .claude/hooks/mfc-edit.sh manifest [paths...] # 매니페스트 생성/갱신
#   .claude/hooks/mfc-edit.sh begin    <file>     # CP949/UTF-16 -> UTF-8 (편집 시작)
#   .claude/hooks/mfc-edit.sh end      <file>     # 원래 인코딩으로 복원 (편집 종료)
#
# (Python이 전혀 없을 때의 수동 대안 — iconv:
#    begin: iconv -f CP949 -t UTF-8 file > file.utf8 && mv file.utf8 file
#    end  : iconv -f UTF-8 -t CP949 file > file.cp949 && mv file.cp949 file
#  단, BOM/UTF-16/상태기록은 처리하지 않으므로 가능하면 python 경로를 쓸 것.)
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
script="$here/mfc_encoding.py"
if command -v python3 >/dev/null 2>&1; then PY=python3
elif command -v python >/dev/null 2>&1; then PY=python
else echo "Python(python3/python)이 필요합니다." >&2; exit 1; fi
case "${1:-}" in
  detect|manifest|begin|end|guard|restore-bom) exec "$PY" "$script" "$@" ;;
  *) echo "usage: mfc-edit.sh {detect|manifest|begin|end} <file>"; exit 1 ;;
esac
