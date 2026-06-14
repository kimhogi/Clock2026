<#
  install.ps1 — khg-loop / khg-mfc 스킬을 이 PC의 ~/.claude/skills 로 설치.
  Clock2026 체크아웃에서 실행하거나, zip을 풀고 그 폴더에서 실행.

  사용:
    powershell -ExecutionPolicy Bypass -File install.ps1                # 복사만
    powershell -ExecutionPolicy Bypass -File install.ps1 -CommitConfigRepo
        # ~/.claude 가 git repo면 커밋까지(=> Task Scheduler 동기화가 다른 PC로 전파)
#>
param(
  [string]$Source,
  [switch]$CommitConfigRepo
)
$ErrorActionPreference = 'Stop'
if (-not $Source) {
  # 기본: 이 스크립트가 Clock2026/tools/install-khg-skills/ 에 있다고 보고 ..\..\.claude\skills
  $Source = Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Path) '..\..\.claude\skills'
  if (-not (Test-Path $Source)) { $Source = (Get-Location).Path }  # zip 풀고 그 안에서 실행하는 경우
}
$dest = Join-Path $env:USERPROFILE '.claude\skills'
New-Item -ItemType Directory -Force -Path $dest | Out-Null
foreach ($s in 'khg-loop','khg-mfc') {
  $src = Join-Path $Source $s
  if (-not (Test-Path $src)) { Write-Warning "건너뜀(없음): $src"; continue }
  Copy-Item $src $dest -Recurse -Force
  Write-Host "설치됨: $s -> $dest\$s"
}
if ($CommitConfigRepo) {
  $cfg = Join-Path $env:USERPROFILE '.claude'
  if (Test-Path (Join-Path $cfg '.git')) {
    Push-Location $cfg
    git add skills/khg-loop skills/khg-mfc 2>&1 | Out-Null
    git commit -m "add/update khg-loop, khg-mfc skills" 2>&1 | Out-Null
    Pop-Location
    Write-Host "~/.claude config repo에 커밋 완료 (동기화가 다른 PC로 전파)."
  } else { Write-Warning "~/.claude 가 git repo가 아니라 커밋 생략." }
}
Write-Host "완료. Claude Code 새 세션에서 /khg-mfc, /khg-loop 사용 가능."
