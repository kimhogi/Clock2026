# 인코딩 안전 편집 — 레시피 & 복구 (khg-mfc 참고문서)

한글/CJK MFC 소스는 파일마다 인코딩이 다르다(CP949 NoBOM / UTF-8 BOM / UTF-16 LE).
편집 도구는 UTF-8로 다루므로 **CP949 파일을 직접 건드리면 한글이 U+FFFD로 영구 손상**된다.
아래 두 경로(Python 번들 / 순수 PowerShell) 중 환경에 맞는 것을 쓴다. 둘 다 결과는 동일.

## 1. 권장: 번들 Python 스크립트 (`scripts/mfc_encoding.py`) — 크로스플랫폼

```bash
# 인코딩 확인
python3 .../scripts/mfc_encoding.py detect  File.cpp            # mbcs|utf-8-bom|utf-16le|ascii|...
# 안전 편집 (CP949/UTF-16 파일)
python3 .../scripts/mfc_encoding.py begin   File.cpp [--cp 949] # 자동 .bak + UTF-8 임시변환
#   ... Read/Edit 로 수정 (새 주석은 ASCII만) ...
python3 .../scripts/mfc_encoding.py end     File.cpp            # 원래 인코딩 복원 + round-trip 검증
# 손상 스캔 / 인벤토리 / 복구
python3 .../scripts/mfc_encoding.py scan    File.cpp            # U+FFFD / '????' 검출
python3 .../scripts/mfc_encoding.py manifest                    # 프로젝트 전체 인코딩 인벤토리
python3 .../scripts/mfc_encoding.py recover  Corrupt.cpp Clean.bak [--cp 949]
```
일본어/중국어는 `--cp 932`(Shift-JIS) / `--cp 936`(GBK) 또는 환경변수 `MFC_CODEPAGE`.
훅으로 자동 적용되므로, CP949 파일을 그냥 Edit 하려 하면 차단되며 위 begin/end 안내가 뜬다.

## 2. 순수 PowerShell 레시피 (Python 없이, Windows)

```powershell
function Get-Enc($p){
  $b=[IO.File]::ReadAllBytes($p)
  if($b.Length -ge 3 -and $b[0] -eq 0xEF -and $b[1] -eq 0xBB -and $b[2] -eq 0xBF){return 'UTF8BOM'}
  if($b.Length -ge 2 -and $b[0] -eq 0xFF -and $b[1] -eq 0xFE){return 'UTF16LE'}
  if($b.Length -ge 2 -and $b[0] -eq 0xFE -and $b[1] -eq 0xFF){return 'UTF16BE'}
  return 'NoBOM'   # 한글 MFC는 기본 CP949; 모호하면 물어볼 것
}
function Get-EncObj($enc, $codepage = 949){
  switch($enc){
    'UTF8BOM' { New-Object System.Text.UTF8Encoding($true) }
    'UTF16LE' { [Text.Encoding]::Unicode }
    'UTF16BE' { [Text.Encoding]::BigEndianUnicode }
    'NoBOM'   { [Text.Encoding]::GetEncoding($codepage) }
  }
}
function Write-PreserveEnc($p, $text, $enc, $codepage = 949){
  $e = Get-EncObj $enc $codepage
  if($enc -eq 'NoBOM'){ [IO.File]::WriteAllBytes($p, $e.GetBytes($text)) }  # raw bytes, no BOM
  else { [IO.File]::WriteAllText($p, $text, $e) }
}
# 쓰기 전 round-trip 인코더빌리티 검사 (NoBOM/CP949):
function Test-Encodable($text, $codepage = 949){
  $e=[Text.Encoding]::GetEncoding($codepage)
  if($e.GetString($e.GetBytes($text)) -eq $text){ @() } else {
    $bad=@(); for($i=0;$i -lt $text.Length;$i++){ $c=$text[$i]
      if($e.GetString($e.GetBytes([string]$c)) -ne [string]$c){ $bad += ('U+{0:X4} at {1}' -f [int]$c,$i) } }; $bad }
}
# 쓰기 후 손상 스캔: EF BF BD(U+FFFD) 와 0x3F 4+연속('????')
```
**금지**: `Set-Content`, `Out-File`, `Get-Content -Raw | Set-Content` (기본 인코딩 적용 → 손상).

## 3. Recovery — 이미 손상된 파일 복구 (3중 검증)

전제: 코드(ASCII)는 UTF-8 라운드트립을 견뎌 살아있고, **한글 주석/문자열만 U+FFFD**로 사라진 경우.
깨끗한 `.bak`(한글 원본) + 현재 파일(코드 원본)을 라인 단위 difflib로 병합한다.

1. 깨끗한 백업 확보(`.bak.<timestamp>` 또는 git 이전 버전). 없으면 복구 불가(U+FFFD는 무손실 불가).
2. `mfc_encoding.py recover <corrupt> <clean.bak> [--cp N]` 실행 — 현재 코드 라인 우선,
   U+FFFD 포함 라인은 백업의 대응 라인으로 대체, 병합 결과를 원래 코드페이지로 기록(+자동 .bak).
3. **검증 3종**: (a) `scan` 으로 U+FFFD/`????` 0건, (b) 한글 라인 diff가 백업과 일치,
   (c) 빌드 sanity. 하나라도 실패면 수동 확인.
   *실측: 425KB `.cpp`의 5883개 손상을 이 절차로 무손실 복구.*

## 4. 치트시트
| 파일 | 인코딩 | 비고 |
|---|---|---|
| `.cpp`/`.h`/`.rc`(한글) | CP949 NoBOM | BOM 추가 금지 |
| `.vcxproj`/`.filters` | UTF-8 BOM | BOM 보존 |
| VS 생성 다이얼로그/테스트 `.cpp` | UTF-16 LE | 널바이트 ~50% |

코드페이지: 한글 949 / 일본어 932 / 중국어 간체 936.
