<#
  Set-SyncSchedule.ps1 — ClaudeConfigGitSync 작업의 트리거를
  하루 5회(09:00, 12:00, 15:00, 18:00, 21:00)로 교체.
  실행: powershell -ExecutionPolicy Bypass -File .\Set-SyncSchedule.ps1
#>
param([string]$TaskName = 'ClaudeConfigGitSync',
      [string[]]$Times = @('09:00','12:00','15:00','18:00','21:00'))
$triggers = $Times | ForEach-Object { New-ScheduledTaskTrigger -Daily -At $_ }
Set-ScheduledTask -TaskName $TaskName -Trigger $triggers
Write-Host "[$TaskName] 트리거를 다음으로 설정: $($Times -join ', ')"
