<#
  ClaudeConfigGitSync.ps1
  여러 PC 간 ~/.claude 설정(skills/agents/commands/settings.json 등)을 git으로 동기화.

  Task Scheduler 동작(현재 설정과 동일하게):
    프로그램: powershell.exe
    인수: -NoProfile -WindowStyle Hidden -ExecutionPolicy Bypass -File "%USERPROFILE%\.claude\ClaudeConfigGitSync.ps1"

  동작 순서: 로컬변경 commit -> fetch -> pull --rebase --autostash -> (충돌처리) -> push
  로그: ~/.claude/sync.log

  충돌 시 -ResolveWithClaude 를 주면 claude 헤드리스(-p)로 자동 해결을 시도하고,
  안 주면 rebase를 안전하게 중단하고 conflict 브랜치 + SYNC_CONFLICT.txt 로 알린다.
#>
param(
  [string]$ConfigDir = "$env:USERPROFILE\.claude",
  [switch]$ResolveWithClaude
)
$ErrorActionPreference = 'Stop'
$log = Join-Path $ConfigDir 'sync.log'
function Log($m) { "$(Get-Date -Format o) [$env:COMPUTERNAME] $m" | Out-File -FilePath $log -Append -Encoding utf8 }

try {
  if (-not (Test-Path (Join-Path $ConfigDir '.git'))) { Log "no git repo at $ConfigDir; skip"; return }
  Set-Location $ConfigDir

  # 1) 로컬 변경 커밋
  git add -A 2>&1 | Out-Null
  if (git status --porcelain) {
    git commit -m "sync($env:COMPUTERNAME): $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')" 2>&1 | Out-Null
    Log "committed local changes"
  }

  # 2) 원격 가져와 rebase (로컬 미커밋 변경은 autostash)
  git fetch origin 2>&1 | Out-Null
  $branch = (git rev-parse --abbrev-ref HEAD).Trim()
  git pull --rebase --autostash origin $branch 2>&1 | Out-File $log -Append -Encoding utf8

  # 3) 충돌 검사
  if (git ls-files -u) {
    Log "CONFLICT detected on branch $branch"
    if ($ResolveWithClaude -and (Get-Command claude -ErrorAction SilentlyContinue)) {
      Log "invoking claude -p (headless) to resolve"
      $prompt = @'
You are inside the ~/.claude config git repository, mid-rebase with merge conflicts.
Resolve EVERY conflicted file with these rules:
- These are Claude config files (skills/agents/commands/settings.json). Most conflicts are two
  machines adding different things. KEEP BOTH sides additions when they are independent.
- For settings.json, merge the JSON: union of permissions/hooks/env, never drop keys.
- For a genuinely conflicting single value, prefer the newer/more complete version.
- NEVER delete a skill, agent, or command. NEVER touch secrets.
Then run: git add -A   (do not commit; do not continue the rebase). Then stop.
'@
      claude -p $prompt --permission-mode acceptEdits --allowedTools "Read,Edit,Bash,Glob,Grep" 2>&1 | Out-File $log -Append -Encoding utf8
      git add -A 2>&1 | Out-Null
      git -c core.editor=true rebase --continue 2>&1 | Out-File $log -Append -Encoding utf8
      if (git ls-files -u) { Log "still conflicted after claude; aborting"; git rebase --abort 2>&1 | Out-Null; throw "unresolved conflicts" }
      Log "claude resolved conflicts"
    } else {
      git rebase --abort 2>&1 | Out-Null
      $b = "conflict/$env:COMPUTERNAME-$(Get-Date -Format 'yyyyMMdd-HHmmss')"
      git branch $b 2>&1 | Out-Null
      "동기화 충돌 발생: 이 PC($env:COMPUTERNAME)에서 Claude Code를 ~/.claude 에서 열고 " +
        "'~/.claude 동기화 충돌 해결해줘' 라고 말하세요. 충돌 스냅샷 브랜치: $b" |
        Out-File (Join-Path $ConfigDir 'SYNC_CONFLICT.txt') -Encoding utf8
      Log "rebase aborted; saved $b; wrote SYNC_CONFLICT.txt"
      return
    }
  }

  # 4) 푸시
  git push origin $branch 2>&1 | Out-File $log -Append -Encoding utf8
  Remove-Item (Join-Path $ConfigDir 'SYNC_CONFLICT.txt') -ErrorAction SilentlyContinue
  Log "push done ($branch)"
}
catch { Log "ERROR: $_" }
