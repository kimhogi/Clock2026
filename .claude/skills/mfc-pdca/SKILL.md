---
name: mfc-pdca
description: >-
  MFC / Visual Studio C++ 프로젝트를 PDCA(Plan-Do-Check-Act) + TDD 규율로
  안전하게 개발할 때 사용한다. 특정 저장소 전용이 아니라 모든 MFC 프로젝트에 두루 쓴다.
  버그 수정, 기능 추가, 리팩터링을 "실패 테스트 → 최소 구현 → 빌드/테스트 green"의
  작은 사이클로 끝까지 굴린다. 빌드는 MSBuild, 검증은 CppUnitTest를 쓴다.
  Use for disciplined, test-driven changes to any MFC/Visual Studio C++ codebase.
---

# mfc-pdca — MFC 프로젝트용 PDCA 개발 스킬

MFC(또는 Visual Studio C++) 프로젝트의 변경을 **추측 없이, 한 걸음씩, 매번 검증하며**
진행하기 위한 스킬이다. 어떤 MFC 저장소에서도 동작하도록 프로젝트 구성은 자동 인식한다.

## 이 스킬이 따르는 루프

이 스킬은 직접 단계를 다시 정의하지 않는다. 두 개의 루프 문서를 **참조**한다:

1. **범용 루프**: `@.claude/skills/loop/SKILL.md`
   - FRAME → OBSERVE → PLAN → ACT → CHECK → REFLECT → DONE 6단계와 가드레일.
2. **MFC 전용 절차**: `@.claude/skills/mfc-pdca/loop.md`
   - 위 6단계의 빈 칸(특히 **CHECK = 빌드 + 단위 테스트**)을 MFC/MSBuild/CppUnitTest로 채움.
   - PDCA ↔ 6단계 매핑, 프로젝트 자동 인식, TDD 규율, DONE 기준을 담음.

작업 시 항상 위 두 문서를 먼저 읽고 그 절차대로 진행한다.

## 작업 합의 (working agreement)

- **TDD가 기본**: 모든 동작 변경은 실패하는 테스트부터. 규율이 깨지면 멈추고 바로잡는다.
- **한 번에 하나**: 한 사이클엔 검증 가능한 변경 하나만. scope creep 금지.
- **기존 관례 존중**: 그 저장소의 네이밍·스타일·타입 캐스팅 관례를 그대로 따른다.
- **속도보다 규율**: "빨리"보다 "검증된 채로"가 우선.
- **검증 없는 완료 없음**: 마지막 CHECK가 green이고 DONE 기준을 모두 충족할 때만 "완료".
- **정직한 보고**: 실패는 출력과 함께, 건너뜀은 이유와 함께 보고한다.

## 빠른 시작

```
1) loop.md 0단계로 프로젝트 자동 인식: <SLN> <Config>|<Plat> <TEST_DLL> 확정
2) FRAME: 검증 가능한 완료 기준(DoD)을 한 문장으로
3) 사이클 반복:
   RED  ─ 실패 TEST_METHOD 추가
   GREEN─ 최소 구현
   CHECK─ msbuild 빌드 + vstest 실행 (출력 직접 확인)
   REFLECT─ 통과면 다음 걸음 / 실패면 가설 변경 (같은 수정 2회 반복 금지, 3회 막히면 보고)
4) DONE: 빌드 green + 전체 테스트 green + 증거와 함께 보고
```

## 환경 주의

MSBuild/vstest는 Windows 전용이다. Linux 세션 등 빌드 불가 환경이면 CHECK 명령을
사용자가 Windows에서 실행하도록 안내하고, 결과를 받기 전까지는 "검증 대기"로 보고한다.
(자세한 내용은 `loop.md` 참조.)

## 참고

GitHub의 검증된 PDCA 스킬을 참고했다.
- kenjudy/pdca-framework — <https://github.com/kenjudy/pdca-framework>
- kenjudy/pdca-code-generation-process — <https://github.com/kenjudy/pdca-code-generation-process>
