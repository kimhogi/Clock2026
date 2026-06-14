<#
  Setup-NewPC.ps1 — 새 PC에서 ~/.claude 설정 동기화를 "한 번에" 구성.
    1) claude-config repo 를 ~/.claude 로 가져오기(아직 git repo가 아니면)
    2) 작업 스케줄러 'ClaudeConfigGitSync' 등록/갱신 (하루 5회: 09/12/15/18/21시, 숨김 실행)
    3) 즉시 1회 동기화

  사용 (새 PC에서, 관리자 아니어도 됨 — 본인 계정 작업):
    powershell -ExecutionPolicy Bypass -File Setup-NewPC.ps1 -RepoUrl https://github.com/kimhogi/claude-config

  주의: claude-config 가 비공개면 그 PC에 git 로그인(자격증명)이 돼 있어야 clone 됩니다.
#>
param(
  [Parameter(Mandatory=$true)][string]$RepoUrl,
  [string[]]$Times = @('09:00','12:00','15:00','18:00','21:00'),
  [string]$TaskName = 'ClaudeConfigGitSync'
)
$ErrorActionPreference = 'Stop'
$cfg = Join-Path $env:USERPROFILE '.claude'

# 1) ~/.claude 를 git repo 로 확보 (기존 파일 보존하며 repo 병합)
if (-not (Test-Path (Join-Path $cfg '.git'))) {
  New-Item -ItemType Directory -Force $cfg | Out-Null
  $tmp = Join-Path $env:TEMP ('claudecfg-' + [guid]::NewGuid().ToString('N'))
  git clone $RepoUrl $tmp
  if ($LASTEXITCODE -ne 0) { throw "clone 실패: $RepoUrl (자격증명/네트워크 확인)" }
  # repo 내용을 ~/.claude 위로 병합(+ .git 이동). 기존 파일은 repo 버전으로 덮어씀.
  robocopy $tmp $cfg /E /MOVE /NFL /NDL /NJH /NJS /NC /NS /NP | Out-Null
  Remove-Item $tmp -Recurse -Force -ErrorAction SilentlyContinue
  Write-Host "[1/3] claude-config 가져옴 -> $cfg"
} else {
  Write-Host "[1/3] 이미 git repo: $cfg (clone 생략, pull로 최신화)"
  Push-Location $cfg; git pull --rebase --autostash 2>&1 | Out-Null; Pop-Location
}

# 2) 동기화 스크립트 위치 확인 (repo에 포함돼 있어야 함)
$sync = Join-Path $cfg 'ClaudeConfigGitSync.ps1'
if (-not (Test-Path $sync)) {
  Write-Warning "repo에 ClaudeConfigGitSync.ps1 이 없습니다. 그 파일을 claude-config repo 루트에 넣어 두세요."
}

# 3) 작업 스케줄러 등록/갱신
$action   = New-ScheduledTaskAction -Execute 'powershell.exe' `
            -Argument "-NoProfile -WindowStyle Hidden -ExecutionPolicy Bypass -File `"$sync`""
$triggers = $Times | ForEach-Object { New-ScheduledTaskTrigger -Daily -At $_ }
$settings = New-ScheduledTaskSettingsSet -StartWhenAvailable
if (Get-ScheduledTask -TaskName $TaskName -ErrorAction SilentlyContinue) {
  Set-ScheduledTask -TaskName $TaskName -Action $action -Trigger $triggers -Settings $settings | Out-Null
  Write-Host "[2/3] 작업 갱신: $TaskName ($($Times -join ', '))"
} else {
  Register-ScheduledTask -TaskName $TaskName -Action $action -Trigger $triggers -Settings $settings `
    -Description "Sync ~/.claude config across PCs" | Out-Null
  Write-Host "[2/3] 작업 등록: $TaskName ($($Times -join ', '))"
}

# 4) 즉시 1회 동기화
if (Test-Path $sync) {
  powershell -NoProfile -ExecutionPolicy Bypass -File $sync
  Write-Host "[3/3] 최초 동기화 실행 완료."
} else {
  Write-Host "[3/3] 동기화 스크립트가 없어 최초 동기화는 생략."
}
Write-Host "새 PC 설정 완료. Claude Code 새 세션에서 스킬 사용 가능."
