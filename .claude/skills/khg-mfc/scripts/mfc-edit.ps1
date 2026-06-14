<#
  MFC 안전 편집/인코딩 래퍼 (Windows/PowerShell). 실제 로직은 mfc_encoding.py.
    .\mfc-edit.ps1 detect|manifest|begin|end|scan|recover <args>   (--cp 932/936)
#>
param([Parameter(Mandatory=$true)][string]$Cmd,
      [Parameter(ValueFromRemainingArguments=$true)]$Rest)
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$script = Join-Path $here 'mfc_encoding.py'
$py = $null
foreach ($c in 'python','python3') { if (Get-Command $c -ErrorAction SilentlyContinue) { $py = $c; break } }
if (-not $py) { Write-Error 'Python(python/python3) 필요'; exit 1 }
& $py $script $Cmd @Rest
exit $LASTEXITCODE
