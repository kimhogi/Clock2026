# claude-config-sync — 여러 PC 간 ~/.claude 설정 동기화

`~/.claude`(skills/agents/commands/settings.json)를 git으로 여러 PC에서 동기화한다.

## 최초 1회 설정 (PC마다)
1. `~/.claude` 를 git 저장소로 만들고 원격 연결 (비공개 repo 권장):
   ```powershell
   cd $env:USERPROFILE\.claude
   copy <이 폴더>\dot-claude.gitignore .gitignore   # 비밀/휘발성 제외 (중요!)
   git init; git add -A; git commit -m "init claude config"
   git remote add origin <your-private-repo-url>
   git push -u origin main
   ```
   (다른 PC는 빈 `~/.claude` 대신 `git clone <repo> ~/.claude`)
2. 동기화 스크립트 배치:
   ```powershell
   copy <이 폴더>\ClaudeConfigGitSync.ps1 $env:USERPROFILE\.claude\
   ```
3. Task Scheduler 동작(현재 쓰시는 그대로):
   - 프로그램: `powershell.exe`
   - 인수: `-NoProfile -WindowStyle Hidden -ExecutionPolicy Bypass -File "%USERPROFILE%\.claude\ClaudeConfigGitSync.ps1"`
   - 충돌 자동해결까지 원하면 인수 끝에 ` -ResolveWithClaude` 추가.
   - 트리거: 로그온 시 + N분마다(예: 30분) 반복.

## 충돌 처리
- **기본**: 충돌 시 rebase를 안전 중단하고 `conflict/<PC>-<시각>` 브랜치 보존 +
  `SYNC_CONFLICT.txt` 생성. 그 PC에서 Claude Code를 `~/.claude`에서 열고
  "~/.claude 동기화 충돌 해결해줘" 라고 하면 Claude가 해결.
- **자동(-ResolveWithClaude)**: 스케줄러가 `claude -p`(헤드리스)로 충돌을 자동 병합
  (skills/agents는 양쪽 보존, settings.json은 키 union). 실패 시 안전 중단.

## 충돌을 애초에 줄이는 핵심
`dot-claude.gitignore`로 **비밀·기기별·휘발성**(projects/, todos/, statsig/, 인증 등)을
제외하고 **공유 설정만** 추적하면 충돌이 거의 안 난다. skills/agents는 파일이 분리돼 있어
서로 다른 PC에서 다른 스킬을 추가해도 충돌하지 않는다.
