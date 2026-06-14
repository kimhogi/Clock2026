#!/usr/bin/env bash
# install.sh — khg-loop / khg-mfc 스킬을 이 PC의 ~/.claude/skills 로 설치 (Linux/macOS).
#   ./install.sh                 # 복사만
#   ./install.sh --commit        # ~/.claude 가 git repo면 커밋까지(동기화 전파)
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
src="$here/../../.claude/skills"
[ -d "$src" ] || src="$(pwd)"      # zip 풀고 그 안에서 실행하는 경우
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
