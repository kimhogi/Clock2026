# 인계 문서 (HANDOFF) — Claude 스킬 + ~/.claude 동기화 구축

> 새 세션에서 이 파일을 읽혀서 이어서 진행하세요. (작성: 2026-06-14)

## 0. 목표
- 한글 MFC 프로젝트(Clock2026 등)용 Claude 스킬과, 범용 PDCA 루프 스킬을 만들어
  여러 PC에서 `~/.claude` git 동기화로 공유한다.
- 핵심 제약: **Claude는 원격 컨테이너에서 동작 → 사용자 PC 파일/작업스케줄러에 직접 접근 불가.**
  GitHub repo를 통해서만 전달 가능.

## 1. 만든 결과물 (저장소: `kimhogi/Clock2026`, 브랜치 `claude/mfc-skill-loop-md-mz7z38`, PR #1)
### 스킬 (`.claude/skills/`)
- **khg-loop** — 범용 풀 라이프사이클(PRD→TRD→계획→구현→검증→테스트→회고) + 안쪽 6단계
  마이크로 루프. 멀티에이전트(단계별 서브에이전트), 산출물은 `docs/loops/<task-id>/`에 파일로.
  템플릿: `khg-loop/templates/`.
- **khg-mfc** — 독립형 올인원 MFC 팀(외부 bkit/`/mfc` 에이전트 불필요).
  - 4-역할 팀(planner·cpp-expert·encoding-guardian·error-verifier), 6 의도 라우팅,
    실행계약(TaskList·승인 게이트·백업·verifier 종료), verifier 10항목 체크리스트, 치트시트.
  - 인코딩 자동 보호: frontmatter 훅(PreToolUse guard / PostToolUse restore-bom) +
    번들 스크립트 `khg-mfc/scripts/mfc_encoding.py` (다중 코드페이지 949/932/936,
    detect/manifest/begin/end/scan/recover, 백업, round-trip 검증).
  - 참고문서 `khg-mfc/references/encoding-safe-editing.md` (Python+PowerShell 레시피, 복구).
  - 큰 기능은 **khg-loop** 스킬로 감싸 PDCA 수행.
- 두 스킬은 zip(`dist/khg-skills.zip`)으로도 전달됨. (`tools/install-khg-skills/install.ps1|.sh`)

### 동기화 도구 (`tools/claude-config-sync/`)
- `ClaudeConfigGitSync.ps1` — 주기 동기화(commit→pull --rebase --autostash→push, 충돌 시
  안전중단/플래그, `-ResolveWithClaude`면 `claude -p` 헤드리스 자동해결).
- `dot-claude.gitignore` — `~/.claude/.gitignore`로 쓸 것(비밀·휘발성 제외 → 충돌·유출 방지).
- `Set-SyncSchedule.ps1` — 작업 트리거를 하루 5회(09/12/15/18/21시)로 교체.
- `Setup-NewPC.ps1` — 새 PC 부트스트랩(claude-config를 ~/.claude로 + 작업 등록 + 최초 동기화).

## 2. 인코딩 사실 (Clock2026 기준, 일반 한글 MFC도 유사)
- `.cpp/.h/.rc`(한글) = **CP949 NoBOM** / `.vcxproj/.filters` = **UTF-8 BOM** / VS 생성 일부 `.cpp` = **UTF-16 LE**.
- Claude의 Read/Edit/Write는 UTF-8로 다룸 → **CP949 직접 편집 시 한글이 U+FFFD로 영구 손상**(실측).
  그래서 guard 훅이 비-UTF8 직접편집을 차단하고, begin/end 안전편집을 강제.

## 3. 제약/주의 (중요)
- Claude는 사용자 PC·로컬 폴더·작업스케줄러에 **직접 접근 불가**(원격 컨테이너).
- 이 세션의 GitHub 권한은 **`kimhogi/clock2026` 한정** → `kimhogi/claude-config`에 **쓸 수 없음**(확인됨: MCP/직접git/프록시 모두 거부).
- `.claude/skills`/`hooks`/`settings.json`/`mfc-encodings.json`만 추적되도록 `.gitignore` 처리됨.
- 빌드/테스트(MSBuild·CppUnitTest)는 Windows 전용 → Linux 세션에선 "검증 대기"로 보고.

## 4. 사용자 PC 현재 상태 (2026-06-14 진단)
- ✅ 스킬 `khg-mfc`/`khg-loop`가 `C:\Users\CG\.claude\skills\`에 설치됨.
- ✅ 작업 `ClaudeConfigGitSync`가 하루 5회 트리거, State Ready.
- ❌ **`~/.claude`가 git repo가 아님** → 동기화 실패(LastTaskResult=0xFFFD0000, sync.log 없음).
- ❌ `~/.claude/.gitignore` 없음(이대로 init 시 비밀파일 유출 위험).
- ℹ️ 작업이 실행하는 스크립트는 `C:\Users\CG\.claude\git-sync.ps1`(기존 사용자 스크립트).

## 5. 다음 할 일 (TODO)
1. **`~/.claude`를 `kimhogi/claude-config`에 안전 연결** — 반드시 `.gitignore` 먼저(비밀 제외) 후 init/commit.
   - 권장: 아래 6번처럼 세션에 `claude-config` 추가 → Claude가 repo에 .gitignore+스크립트+스킬 커밋 →
     PC에서 `Setup-NewPC.ps1 -RepoUrl https://github.com/kimhogi/claude-config` 한 줄.
2. `claude-config` repo에 `ClaudeConfigGitSync.ps1`·`.gitignore`·`Set-SyncSchedule.ps1`·`Setup-NewPC.ps1`·`skills/` 배치.
3. (선택) 충돌 자동해결 `-ResolveWithClaude` 테스트 후 사용.
4. (선택) khg-mfc/khg-loop 실제 한 사이클 시연으로 검증.

## 6. ★ 접근 권한 — 어디를 열어주면 되나
- **로컬 폴더가 아니라 GitHub 저장소입니다.** 새 세션을 시작할 때(또는 환경 설정에서),
  Clock2026을 추가했던 그 화면에서 **`kimhogi/claude-config`** 저장소를 **이 세션에 추가**하세요.
- 그러면 Claude가 `claude-config`에 직접 커밋 가능 → `.gitignore`(먼저)·동기화 스크립트·스킬을
  안전하게 넣고, 사용자님은 각 PC에서 `Setup-NewPC.ps1`만 실행하면 됩니다.
- (선택) `kimhogi/Clock2026`도 함께 두면 이 작업 이력/PR #1을 계속 참조 가능.

## 7. 새 세션 시작 멘트(복붙용)
> "docs/HANDOFF.md 이어서. 세션에 kimhogi/claude-config 추가했어. 거기에 .gitignore(비밀 제외) 먼저
>  넣고 ClaudeConfigGitSync.ps1·Set-SyncSchedule.ps1·Setup-NewPC.ps1·skills/khg-mfc·skills/khg-loop를
>  커밋해줘. 그다음 내 PC에서 Setup-NewPC.ps1 실행 절차 알려줘."
