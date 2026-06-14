#!/usr/bin/env bash
# MFC 안전 편집/인코딩 래퍼 (Linux/macOS). 실제 로직은 mfc_encoding.py.
#   detect|manifest|begin|end|scan|recover <args>   (--cp 932/936 로 일/중 코드페이지)
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if command -v python3 >/dev/null 2>&1; then PY=python3
elif command -v python  >/dev/null 2>&1; then PY=python
else echo "Python(python3/python) 필요" >&2; exit 1; fi
exec "$PY" "$here/mfc_encoding.py" "$@"
