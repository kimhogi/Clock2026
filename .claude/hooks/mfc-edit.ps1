<#
  MFC 안전 편집 / 인코딩 래퍼 (Windows / PowerShell).
  핵심 로직은 mfc_encoding.py 에 있고, 이 래퍼는 python/python3 로 호출한다.

  사용:
    .\.claude\hooks\mfc-edit.ps1 detect   <file>
    .\.claude\hooks\mfc-edit.ps1 manifest [paths...]
    .\.claude\hooks\mfc-edit.ps1 begin    <file>   # CP949/UTF-16 -> UTF-8 (편집 시작)
    .\.claude\hooks\mfc-edit.ps1 end      <file>   # 원래 인코딩으로 복원 (편집 종료)

  (Python이 전혀 없을 때의 수동 대안 — .NET:
     begin: $t=[IO.File]::ReadAllText($f,[Text.Encoding]::GetEncoding(949)); [IO.File]::WriteAllText($f,$t,(New-Object Text.UTF8Encoding $false))
     end  : $t=[IO.File]::ReadAllText($f,(New-Object Text.UTF8Encoding $false)); [IO.File]::WriteAllText($f,$t,[Text.Encoding]::GetEncoding(949))
   단, BOM/UTF-16/상태기록은 처리하지 않으므로 가능하면 python 경로를 쓸 것.)
#>
param(
  [Parameter(Mandatory=$true)][ValidateSet('detect','manifest','begin','end','guard','restore-bom')]
  [string]$Cmd,
  [Parameter(ValueFromRemainingArguments=$true)]$Rest
)
$here   = Split-Path -Parent $MyInvocation.MyCommand.Path
$script = Join-Path $here 'mfc_encoding.py'
$py = $null
foreach ($c in 'python','python3') {
  if (Get-Command $c -ErrorAction SilentlyContinue) { $py = $c; break }
}
if (-not $py) { Write-Error 'Python(python/python3)이 필요합니다.'; exit 1 }
& $py $script $Cmd @Rest
exit $LASTEXITCODE
