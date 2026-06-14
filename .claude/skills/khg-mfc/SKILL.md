---
name: khg-mfc
description: >-
  All-in-one MFC / Visual C++ team for any project (Korean/CJK CP949·CP932·CP936 safe).
  `/khg-mfc <intent>` 또는 "MFC ..."로 호출. 기본 동작은 4-역할 협업(planner → cpp-expert →
  encoding-guardian → error-verifier)을 의도에 맞게 순차 실행한다. 새 MFC 프로젝트, 기능/리팩터,
  rename·migration, 툴셋 업그레이드(v142/v143/v145), 인코딩 민감 편집, 출시 전 검증에 사용.
  큰 기능은 khg-loop 스킬의 PDCA 풀 라이프사이클로 감싼다. 인코딩은 번들 스크립트 + 훅으로
  자동 보호(CP949/UTF-16/BOM 무손상). 독립형 — bkit이나 외부 /mfc 에이전트 설치 불필요.
  Triggers on: mfc, MFC, CDialog, CWnd, CString, AfxBeginThread, .rc, .vcxproj, UseOfMfc,
  한글 깨짐, mojibake, CP949, EUC-KR, Shift-JIS, 文字化け, 编码, message map, DDX, RichEdit OLE,
  v143, v145, vs2022, vs2026, 한글 MFC, MFC PDCA.
hooks:
  PreToolUse:
    - matcher: "Edit|Write|MultiEdit"
      hooks:
        - type: command
          command: 'python3 "${CLAUDE_SKILL_DIR}/scripts/mfc_encoding.py" guard || python "${CLAUDE_SKILL_DIR}/scripts/mfc_encoding.py" guard'
  PostToolUse:
    - matcher: "Edit|Write|MultiEdit"
      hooks:
        - type: command
          command: 'python3 "${CLAUDE_SKILL_DIR}/scripts/mfc_encoding.py" restore-bom || python "${CLAUDE_SKILL_DIR}/scripts/mfc_encoding.py" restore-bom'
---

# /khg-mfc — MFC 팀 (독립형 올인원)

모든 MFC / Visual C++ 프로젝트(특히 한글/CJK CP949 소스)를 **추측 없이, 인코딩을 깨지 않고,
매번 검증하며** 다루는 스킬이다. 외부 의존(bkit `/pdca`, 별도 `/mfc` 에이전트) **없이 단독 동작**한다.
인코딩 보호는 번들 스크립트(`scripts/mfc_encoding.py`)와 이 스킬의 훅으로 **자동** 적용된다.

## 호출 방법
| 방법 | 동작 |
|---|---|
| `/khg-mfc` (인자 없음) | 무엇을 할지 1개 질문 후 라우팅 |
| `/khg-mfc <작업 설명>` | 의도 분류 후 해당 워크플로 즉시 실행 |
| 자연어: "MFC로 ...", "이 MFC 프로젝트에서 ..." | description 키워드로 자동 트리거 |

인자 없이 호출되면 `AskUserQuestion`으로 묻는다:
> 무엇을 도와드릴까요? (1) 새 MFC 프로젝트 기획 / (2) 코드 편집·리팩터 / (3) rename·migration / (4) 툴셋 업그레이드 / (5) 사전 검증만 / (6) 큰 기능을 PDCA로

## 4-역할 팀 (서브에이전트로 실행)

각 역할은 `Agent` 도구로 **순차** 위임한다. 기본 `subagent_type`은 `general-purpose`이며,
로컬에 전용 에이전트(`mfc-project-planner` 등)가 설치돼 있으면 그 이름을 우선 사용한다.

| # | 역할 | 하는 일 | subagent_type |
|---|---|---|---|
| 1 | **planner** | 아키텍처·앱타입·CharacterSet·툴셋·클래스 스켈레톤·다이얼로그 목록·단계별 빌드순서 → `PROJECT_PLAN.md`(코드 없음) | Plan |
| 2 | **cpp-expert** | MFC + 모던 C++ 설계/구현. 메시지맵, DDX/DDV, 스레딩, COM/OLE, v143/v145, diff와 파급효과 | general-purpose |
| 3 | **encoding-guardian** | 모든 파일 쓰기를 여기로. **per-file 인코딩 탐지→보존**(번들 begin/end), 백업, round-trip, 손상 스캔 | general-purpose |
| 4 | **error-verifier** | 읽기전용 감사 — 아래 10항목 체크리스트 | general-purpose |

## 의도 라우팅 (언어 무관 키워드로 분류)

- **Intent 1 — 새 프로젝트** (new/create/기획/새로/新規): planner → (승인) → cpp-expert(스캐폴드) → guardian(쓰기) → verifier. *planner 후 필수 일시정지·승인.*
- **Intent 2 — 코드 편집/기능/리팩터** (add/fix/refactor/추가/고쳐/구현): cpp-expert(diff+파급) → guardian(인코딩 보존 적용) → verifier(변경 파일 + 리소스/프로젝트 의존 감사).
- **Intent 3 — rename/migration** (rename/migrate/이름 바꿔/改名): guardian(인벤토리 스냅샷) → cpp-expert(파일·디렉터리·#include·.vcxproj/.sln/.filters·ID 프리픽스·RootNamespace·PCH 전체 계획) → guardian(인코딩 안전 실행) → verifier(전체 체크리스트).
- **Intent 4 — 툴셋 업그레이드** (v143/v145/VS2022/업그레이드): cpp-expert(설정별 `<PlatformToolset>`/`<CharacterSet>`/`<LanguageStandard>`/`<UseOfMfc>` 감사 + `/permissive-`·`min`/`max` 매크로 충돌·`_HAS_DEPRECATED_*` 분석) → guardian(.vcxproj/.sln 편집, **UTF-8 BOM 보존**) → verifier(전 구성 일관성 + 빌드 sanity).
- **Intent 5 — 검증만** (verify/audit/검증/점검): verifier(10항목) → CRITICAL 시 guardian(바이트 확증)·cpp-expert(수정 diff, 승인 전 미실행).
- **Intent 6 — 큰 기능(PDCA)** : **khg-loop 스킬**의 풀 라이프사이클(PRD→TRD→설계→구현→검증→테스트→회고)로 감싸고, 그 안의 "구현/검증/테스트"를 이 팀으로 수행한다(아래 "khg-loop 연동").

## 실행 계약 (Execution Contract)
1. **항상 TaskList 생성** — 선택한 시퀀스의 에이전트마다 1개 태스크. 디스패치 전 `in_progress`, 반환 후 `completed`.
2. **항상 순차 디스패치** (병렬 금지) — 각 단계는 이전 출력에 의존.
3. **항상 전달**: 프로젝트 루트, NoBOM 파일의 가정 코드페이지(한글 949 — 처음 한 번 확인), 인코딩 인벤토리 스냅샷, 파일 범위.
4. **항상 사용자 승인 게이트**: planner의 `PROJECT_PLAN.md` 후 / guardian이 20개 초과 파일을 건드리기 전 / verifier가 CRITICAL 발견 시.
5. **항상 쓰기 전 백업** — guardian은 수정할 파일을 `.bak.<timestamp>`로 스냅샷(번들 `begin`이 자동). 위치를 알린다.
6. **항상 verifier로 종료** — "사소한" 편집도. mojibake는 조용하다.

## 인코딩 — 자동 보호 + 하드 규칙 (절대 타협 없음)

> **자동**: 이 스킬이 활성화되면 위 frontmatter 훅이 CP949/UTF-16 파일의 직접 Edit/Write를
> **차단**하고, UTF-8+BOM 파일의 BOM을 **복원**한다. (이 저장소엔 `.claude/settings.json`로
> 상시 적용도 걸려 있다.) 수동 절차·복구·PowerShell 레시피는 **`references/encoding-safe-editing.md`**.

1. **Never** CP949/CJK NoBOM 파일을 Edit/Write로 직접 편집(서브에이전트 포함). 안전 편집:
   `python3 ${CLAUDE_SKILL_DIR}/scripts/mfc_encoding.py begin <file>` → 수정 → `... end <file>`.
   (Windows: `scripts\mfc-edit.ps1 begin|end <file>` / 일·중: `--cp 932|936`)
2. **Never** NoBOM을 UTF-8로 변환하거나 `.cpp/.h/.rc`에 BOM을 추가하지 않는다.
3. **Never** PowerShell `Set-Content`/`Out-File` 사용. `[IO.File]::ReadAllBytes/WriteAllBytes` + 명시적 `System.Text.Encoding`.
4. **Always** 쓰기 전 `.bak.<timestamp>` 백업(`begin`이 자동).
5. **Always** round-trip(encode→decode→compare) + 쓰기 후 U+FFFD/`????` 스캔(`end`가 자동 + `scan` 커맨드). 실패 시 중단·롤백, 재시도 루프 금지.
6. **Never** CP949 파일의 새 주석/문자열에 비-ASCII(em-dash, smart quote, 불릿, box-drawing U+2500–257F) 삽입 — 조용히 `????`로 손상. **새 주석은 ASCII만**(`//=====` O, `// ═════` X).
7. **Always** **per-file** 탐지·보존 — 프로젝트 전체 단일 인코딩 가정 금지.

### 이미 손상된 경우(복구)
코드가 ASCII로 생존했고 한글 주석/문자열만 U+FFFD가 됐다면, 깨끗한 `.bak`(한글 원본)과 현재
파일(코드 원본)을 difflib로 병합해 무손상 복구 가능:
`python3 ${CLAUDE_SKILL_DIR}/scripts/mfc_encoding.py recover <corrupt> <clean.bak> [--cp 949]`
→ 이후 `scan`으로 확인. 상세 절차는 `references/encoding-safe-editing.md`의 "Recovery".

## error-verifier 10항목 체크리스트
1) mojibake/U+FFFD/`????` 0건 + Korean 라인 diff 일치  2) MBCS/Unicode(`_UNICODE`/`_MBCS`, `CString` vs `CStringA/W`) 정합  3) 메시지맵 ↔ 핸들러 선언 정합  4) 리소스 ID(`IDD_/IDC_/IDS_`) 드리프트/충돌  5) 다이얼로그 템플릿 ↔ DDX/DDV 동기  6) `CWnd`/GDI 객체 수명·누수  7) `.vcxproj`/`.sln`/`.filters` 드리프트(파일 누락/잉여)  8) 툴셋/CharacterSet/표준 전 구성 일관  9) `#include`/PCH 정합  10) (가능 시) 실제 빌드 sanity.

## 인코딩 치트시트 (어떤 파일이 무슨 인코딩인가)
| 파일 | 보통 인코딩 | 비고 |
|---|---|---|
| `.cpp`/`.h`/`.rc` (한글 MFC) | **CP949 NoBOM** | 가장 흔함, BOM 추가 금지 |
| `.vcxproj`/`.filters`/`.user` | **UTF-8 BOM** | BOM 보존 |
| VS 생성 다이얼로그/테스트 `.cpp` | **UTF-16 LE** | 널바이트 ~50% |
| 일부 `.rc` | UTF-16 LE 또는 ANSI | 각각 확인 |

코드페이지: 한글 **949**, 일본어 **932**(Shift-JIS), 중국어 간체 **936**(GBK).

## khg-loop 연동 (큰 기능 = PDCA)
다단계/다파일 기능은 **khg-loop 스킬**을 불러 PRD→TRD→설계→구현→검증→테스트→회고로 감싸고,
산출물은 `docs/loops/<task-id>/`에 남긴다. 그중 **구현/검증/테스트**는 이 스킬의 4-역할 팀과
인코딩 안전 편집으로 수행한다. MFC 빌드/테스트 명령(MSBuild + CppUnitTest)은 `loop.md` 참조.
(원칙: **사고·문서는 인코딩 중립, 소스 편집은 반드시 안전 경로.**)

## 환경 주의
- MSBuild/vstest는 Windows 전용. Linux 세션이면 빌드/테스트는 "Windows에서 실행" 안내 후 "검증 대기"로 보고.
- 인코딩 스크립트는 Python(코어)으로 동작 — Windows/Mac/Linux 공통. (PowerShell 래퍼/순수 PowerShell 레시피는 references 참조.)

## 참고
- 인코딩 상세/레시피/복구: `references/encoding-safe-editing.md`
- 라이프사이클: khg-loop 스킬
- 설계 참고(GitHub): kenjudy/pdca-framework, popup-studio-ai/bkit-claude-code
