---
name: khg-mfc
description: >-
  MFC / Visual Studio C++ 프로젝트를 PDCA + TDD 규율로 안전하게 개발할 때 사용한다.
  특정 저장소 전용이 아니라 모든 MFC 프로젝트에 두루 쓴다. 큰 작업은 공용 khg-loop 스킬의
  풀 라이프사이클(PRD→TRD→계획→구현→검증→테스트→회고)을 멀티에이전트로 돌리고,
  검증/테스트 단계를 MSBuild 빌드 + CppUnitTest로 채운다. 작은 변경은 안쪽 6단계
  마이크로 루프만 쓴다. 산출물은 docs/loops/<task-id>/ 에 파일로 남긴다.
  Use for disciplined, test-driven, multi-agent changes to any MFC/Visual Studio C++ codebase.
---

# khg-mfc — MFC 프로젝트용 PDCA 개발 스킬 (풀 라이프사이클 + 멀티에이전트)

MFC(또는 Visual Studio C++) 프로젝트의 변경을 **추측 없이, 한 걸음씩, 매번 검증하며**
진행하기 위한 스킬이다. 어떤 MFC 저장소에서도 동작하도록 프로젝트 구성은 자동 인식한다.

## 이 스킬이 따르는 루프

이 스킬은 단계를 다시 정의하지 않고 두 문서를 **참조**한다:

1. **공용 풀 라이프사이클 루프**: `@.claude/skills/khg-loop/SKILL.md`
   - 바깥 루프: PRD → TRD → 계획 → 구현 → 검증 → 테스트 → 회고 (각 단계 전담 **서브에이전트**, 산출물 파일 인수인계).
   - 안쪽 루프: FRAME → OBSERVE → PLAN → ACT → CHECK → REFLECT (구현 단계 엔진, 작은 작업은 단독 사용).
   - 게이트·가드레일·산출물 작업공간(`docs/loops/<task-id>/`)·템플릿 정의.
2. **MFC 전용 절차**: `@.claude/skills/khg-mfc/loop.md`
   - 위 루프의 **VERIFY(검증)=MSBuild 빌드+리뷰**, **TEST(테스트)=CppUnitTest(vstest)** 로 빈 칸을 채움.
   - 프로젝트 자동 인식, TDD 규율, MFC 주의점, DONE 기준.

작업 시 항상 위 두 문서를 먼저 읽고 그 절차대로 진행한다.

## PDCA ↔ 라이프사이클 매핑

| PDCA  | 라이프사이클 단계        | MFC에서 의미                          |
| ----- | ------------------------ | ------------------------------------- |
| Plan  | PRD · TRD · 계획         | 무엇/왜 → 설계 → 작은 태스크 분해     |
| Do    | 구현 (안쪽 마이크로 루프)| TDD red→green, 최소 구현              |
| Check | 검증 · 테스트            | MSBuild 빌드 + CppUnitTest green      |
| Act   | 회고                     | 리팩터·회고·다음 사이클로 순환        |

## 작업 합의 (working agreement)

- **TDD가 기본**: 모든 동작 변경은 실패하는 테스트부터. 규율이 깨지면 멈추고 바로잡는다.
- **한 번에 하나**: 한 사이클엔 검증 가능한 변경 하나만. scope creep 금지.
- **산출물은 파일로**: PRD/TRD/계획/검증/테스트/회고를 `docs/loops/<id>/`에 남긴다(멀티에이전트 인수인계 수단).
- **인코딩 보존**: CP949/UTF-16 등 비-UTF8 파일은 **직접 편집 금지**. `mfc_encoding.py begin/end`로 안전 편집한다(PreToolUse 훅이 직접 편집을 자동 차단, PostToolUse 훅이 UTF-8+BOM을 복원). 상세는 `loop.md` 0.5절.
- **기존 관례 존중**: 그 저장소의 네이밍·스타일·타입 캐스팅 관례를 그대로 따른다.
- **속도보다 규율**: "빨리"보다 "검증된 채로"가 우선.
- **검증 없는 완료 없음**: VERIFY/TEST가 green이고 DONE 기준을 모두 충족할 때만 "완료".
- **정직한 보고**: 실패는 출력과 함께, 건너뜀은 이유와 함께. `_state.md`에 실제 상태 반영.

## 빠른 시작

```
[큰 작업 — 풀 라이프사이클]
1) docs/loops/<YYYYMMDD-슬러그>/ 생성, templates/ 복사 (_state.md 포함)
2) PRD → TRD → PLAN 을 서브에이전트(general-purpose/Plan)로 작성, 각 단계 사람 승인 게이트
3) IMPLEMENT: plan.md 태스크를 하나씩 안쪽 마이크로 루프(TDD)로 구현
4) VERIFY: msbuild 빌드 + /code-review → verification.md
5) TEST: vstest(CppUnitTest) 실행 → test-report.md (전체 green + PRD 수용 기준 충족)
6) RETRO: retro.md, 다음 증분 있으면 PRD로 순환 / 없으면 DONE(증거와 함께 보고)

[작은 작업 — 경량]
loop.md 0단계로 프로젝트 인식 → 안쪽 마이크로 루프만:
RED(실패 테스트) → GREEN(최소 구현) → CHECK(빌드+vstest) → REFLECT
(같은 수정 2회 반복 금지, 3회 막히면 보고)
```

## 환경 주의

MSBuild/vstest는 Windows 전용이다. Linux 세션 등 빌드 불가 환경이면 VERIFY/TEST 명령을
사용자가 Windows에서 실행하도록 안내하고, 결과를 받기 전까지는 "검증 대기"로 보고한다.
(자세한 내용은 `loop.md` 참조.)

## 참고

GitHub의 검증된 PDCA 스킬을 참고했다.
- kenjudy/pdca-framework — <https://github.com/kenjudy/pdca-framework>
- kenjudy/pdca-code-generation-process — <https://github.com/kenjudy/pdca-code-generation-process>
