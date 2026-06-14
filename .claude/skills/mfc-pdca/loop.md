# MFC PDCA Loop — MFC / Visual Studio C++ 공용 실행 루프

> 이 파일은 범용 루프 **@.claude/skills/loop/SKILL.md** 를 **모든 MFC(또는 VS C++)
> 프로젝트**에 맞게 구체화한 것이다. 특정 저장소에 묶이지 않는다 —
> 솔루션 이름·구성·테스트 프로젝트는 **작업 시작 시 자동 인식(auto-detect)** 한다.
>
> 범용 루프의 6단계(FRAME→OBSERVE→PLAN→ACT→CHECK→REFLECT→DONE)는 그대로 따르고,
> 비어 있던 **CHECK(검증) 명령**과 **OBSERVE 대상**을 MFC/MSBuild 절차로 채운다.
>
> PDCA ↔ 범용 루프 대응:
> | PDCA  | 범용 루프 단계         | MFC 프로젝트에서 의미                        |
> | ----- | ---------------------- | ------------------------------------------- |
> | Plan  | FRAME + OBSERVE + PLAN | 목표 고정, 관련 코드 읽기, 실패 테스트 설계  |
> | Do    | ACT                    | 최소 구현 (TDD: red→green)                  |
> | Check | CHECK                  | **빌드 + 단위 테스트 실행** (아래 절차)      |
> | Act   | REFLECT                | 리팩터·회고·다음 사이클 반영                 |

---

## 0. 프로젝트 자동 인식 (OBSERVE의 출발점)

작업 시작 시 **하드코딩 대신** 아래를 실제로 조사해 변수처럼 채운다. 어떤 MFC 저장소든 동일.

| 인식할 것            | 찾는 방법                                                                 |
| -------------------- | ------------------------------------------------------------------------ |
| `<SLN>` 솔루션 파일  | 루트의 `*.sln` (Glob: `**/*.sln`)                                          |
| `<APP_PROJ>` 앱      | `*.vcxproj` 중 `<ConfigurationType>Application` + `<UseOfMfc>` 존재         |
| `<CONFIG>`/`<PLAT>`  | `.sln`의 `SolutionConfigurationPlatforms` (예: `Debug\|x64`)               |
| `<TOOLSET>`/표준     | `.vcxproj`의 `<PlatformToolset>`, `<LanguageStandard>`, `<CharacterSet>`   |
| `<TEST_PROJ>` 테스트 | `CppUnitTest.h` 또는 `Microsoft.VisualStudio.TestTools` 참조 `*.vcxproj`    |
| `<TEST_DLL>`         | 테스트 프로젝트의 `OutDir`(보통 `<Plat>\<Config>\<TestProj>.dll`)          |

> 인식 결과(솔루션명, 구성, 테스트 DLL 경로)를 한 줄로 적어 두고 이후 명령에 대입한다.
> 예: "SLN=Foo.sln, CONFIG=Debug|x64, TEST_DLL=x64\Debug\FooTests.dll".

### MFC 일반 사실(대부분의 MFC 저장소 공통)
- 빌드는 **MSBuild/devenv** 로 한다(Windows 전용 툴체인).
- 문자열은 **Unicode + `_T(...)`/`L"..."`**, 런타임은 보통 **MFC Dynamic/Static** 중 하나.
- 리소스(`.rc`)와 `resource.h`의 ID는 충돌에 민감하다.
- 코드/주석 언어(한국어/영어 등)와 네이밍 관례는 **그 저장소 것을 그대로** 따른다.

> ⚠️ **플랫폼 주의**: MSBuild/vstest는 **Windows 전용**이다. 현재 세션이 Linux 컨테이너면
> 빌드/테스트를 직접 실행할 수 없다. 이때 CHECK는 "Windows에서 아래 명령을 실행"하도록
> 사용자에게 **명시적으로 안내**하고, 가능한 범위에서 코드 레벨 검증(로직 추론·헤더 일관성·
> 정적 점검)으로 보완한다. **절대 "빌드/테스트 통과했을 것"이라 가정하지 않는다.**

---

## 1. CHECK 절차 (이 루프의 심장 — 반드시 실행/안내)

아래 명령의 `<...>`는 0단계에서 인식한 값으로 치환한다.

### 1-1. 빌드 (MSBuild, "Developer Command Prompt for VS")

```bat
:: 솔루션 전체 (앱 + 테스트)
msbuild <SLN> /t:Build /p:Configuration=<Config> /p:Platform=<Plat> /m /nologo /v:minimal

:: 변경이 특정 프로젝트에 한정되면 그 프로젝트만 빠르게
msbuild <TEST_PROJ> /p:Configuration=<Config> /p:Platform=<Plat> /m /nologo /v:minimal
```

- **경고도 본다.** 새 경고가 생겼으면 회귀로 간주하고 REFLECT에서 다룬다.
- 빌드 실패 → 에러 **원문**을 기록하고 OBSERVE로 복귀(추측 수정 금지).

### 1-2. 단위 테스트 (VSTest, CppUnitTest 산출 DLL)

```bat
vstest.console.exe <TEST_DLL> /Platform:<Plat>

:: 특정 테스트 클래스/메서드만
vstest.console.exe <TEST_DLL> /Tests:<TestClassOrMethod>
```

- 또는 Visual Studio **Test Explorer**에서 실행.
- **출력을 실제로 읽는다.** Passed/Failed 수와 실패 메시지를 그대로 기록한다.
- 1개라도 실패면 그 사이클은 green이 아니다 → REFLECT.

> 테스트 프로젝트가 **없다면**: 첫 사이클의 목표를 "최소 CppUnitTest 프로젝트 추가"로
> 잡는다(솔루션에 `*Tests.vcxproj` 생성, `CppUnitTest.h` 참조, 앱 로직을 링크/스텁).
> 이것이 PDCA의 토대다 — 검증 수단이 없으면 CHECK가 성립하지 않는다.

### 1-3. (선택) 앱 스모크 실행
- UI/동작이 바뀐 경우 산출물 `*.exe`를 띄워 핵심 시나리오를 눈으로 확인하고,
  필요하면 스크린샷으로 증거를 남긴다.

---

## 2. Do 단계 규율 (TDD, MFC 맥락)

순서를 지킨다 — **실패 테스트 먼저, 그다음 최소 구현.**

1. **RED**: 테스트 프로젝트에 새 동작을 검증하는 **실패하는** `TEST_METHOD`를 먼저 추가.
   - 구체 구현(concrete)을 테스트한다. 추상 인터페이스용 테스트를 만들지 않는다.
   - `Assert::AreEqual` 비교 시 그 저장소의 타입 관례(`CString`, `(ULONG)`, `(BYTE)` 등
     명시적 캐스팅)를 **기존 테스트 코드 그대로** 따른다.
2. **GREEN**: 테스트를 통과시키는 **가장 작은** 구현만 한다.
3. **REFACTOR**: green을 유지한 채 정리. 그리고 다시 CHECK.

> 한 사이클 = "실패 테스트 1개 → 최소 구현 → 전체 green". 여러 기능을 한 번에 넣지 않는다.

---

## 3. MFC 특화 주의점 (OBSERVE/ACT 체크리스트, 프로젝트 불문)

- **유니코드**: 문자열 리터럴은 `_T(...)`/`L"..."`. 그 저장소의 공용 매크로/기본값
  (빈 문자열 매크로, 기본 charset 등)을 그대로 따른다.
- **대형 공용 헤더**: MFC 프로젝트는 큰 공용 헤더(데이터 모델 등)를 두는 경우가 많다.
  시그니처를 바꾸면 앱·테스트 양쪽이 영향받는다 → 변경 후 **두 프로젝트 모두** 빌드.
- **테스트 링크 에러/스텁**: 테스트가 앱 심볼을 참조하면 스텁 cpp(예: `TestStubs.cpp`)가
  필요할 수 있다. unresolved external 링크 에러가 나면 여기부터 본다.
- **리소스(.rc)**: 손으로 무분별하게 수정하지 말고 변경 범위를 최소화. `resource.h`의
  ID 충돌에 주의.
- **툴셋/표준 일관성**: `.vcxproj`가 지정한 PlatformToolset과 C++ 표준에 맞춘다.
  비표준 확장에 의존하지 않는다.

---

## 4. DONE 기준 (MFC 프로젝트에서 "완료")

다음을 **모두** 충족해야 완료라고 말한다.

- [ ] 새/수정 동작을 덮는 `TEST_METHOD`가 있고 **통과**한다.
- [ ] 인식한 `<Config>|<Plat>`로 **앱 + 테스트 솔루션 전체 빌드 성공** (새 경고 없음).
- [ ] 전체 단위 테스트 **green** (회귀 없음).
- [ ] (UI 변경 시) 앱 스모크 실행으로 핵심 시나리오 확인.
- [ ] 변경 요약과 **검증 증거**(테스트 Passed 수, 빌드 결과)를 함께 보고.

검증을 실행할 수 없는 환경(Linux 등)이면, 위 명령을 **사용자가 Windows에서 실행하도록
안내**하고 그 결과를 받기 전까지는 "완료"가 아니라 "검증 대기"로 보고한다.

---

## 5. 참고

이 루프의 게이트·TDD 규율은 GitHub의 PDCA 스킬을 참고했다.
- kenjudy/pdca-framework — <https://github.com/kenjudy/pdca-framework>
- (범용 단계 정의는 `@.claude/skills/loop/SKILL.md` 참고)
