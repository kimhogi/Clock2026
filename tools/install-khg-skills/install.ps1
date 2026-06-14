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
  # khg-mfc 폴더가 실제로 들어있는 위치를 자동탐지:
  #  1) 스크립트와 같은 폴더(zip을 풀면 install.ps1 옆에 khg-mfc/khg-loop가 있음)
  #  2) Clock2026 체크아웃의 ..\..\.claude\skills
  #  3) 현재 작업 폴더
  $scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
  $candidates = @(
    $scriptDir,
    (Join-Path $scriptDir '..\..\.claude\skills'),
    (Get-Location).Path
  )
  $Source = $candidates | Where-Object { Test-Path (Join-Path $_ 'khg-mfc') } | Select-Object -First 1
  if (-not $Source) { throw "khg-mfc/khg-loop 폴더를 찾을 수 없습니다. zip을 푼 폴더에서 실행하거나 -Source 로 경로를 지정하세요." }
}
Write-Host "소스: $Source"
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
