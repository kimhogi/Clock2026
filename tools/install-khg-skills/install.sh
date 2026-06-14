#!/usr/bin/env bash
# install.sh — khg-loop / khg-mfc 스킬을 이 PC의 ~/.claude/skills 로 설치 (Linux/macOS).
#   ./install.sh                 # 복사만
#   ./install.sh --commit        # ~/.claude 가 git repo면 커밋까지(동기화 전파)
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# khg-mfc 폴더가 실제로 있는 위치 자동탐지: 스크립트 옆 -> Clock2026 -> 현재 폴더
src=""
for c in "$here" "$here/../../.claude/skills" "$(pwd)"; do
  if [ -d "$c/khg-mfc" ]; then src="$c"; break; fi
done
[ -n "$src" ] || { echo "khg-mfc/khg-loop 폴더를 찾을 수 없습니다." >&2; exit 1; }
echo "소스: $src"
dest="$HOME/.claude/skills"
mkdir -p "$dest"
for s in khg-loop khg-mfc; do
  if [ -d "$src/$s" ]; then cp -rf "$src/$s" "$dest/"; echo "설치됨: $s -> $dest/$s"
  else echo "건너뜀(없음): $src/$s"; fi
done
if [ "${1:-}" = "--commit" ] && [ -d "$HOME/.claude/.git" ]; then
  ( cd "$HOME/.claude" && git add skills/khg-loop skills/khg-mfc && \
    git commit -m "add/update khg-loop, khg-mfc skills" >/dev/null 2>&1 || true )
  echo "~/.claude config repo에 커밋 완료."
fi
echo "완료. Claude Code 새 세션에서 /khg-mfc, /khg-loop 사용 가능."
