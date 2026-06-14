# docs/loops — 라이프사이클 루프 산출물 보관소

`loop` / `mfc-pdca` 스킬이 작업을 진행할 때, 작업마다 이 아래에 폴더를 만든다.

```
docs/loops/<task-id>/     # task-id = YYYYMMDD-<짧은-슬러그>  (예: 20260614-add-alarm)
├── _state.md             # 현재 단계 / 게이트 상태 / 결정 로그
├── PRD.md                # 무엇을/왜
├── TRD.md                # 어떻게(설계)
├── plan.md               # 작은 태스크 분해
├── progress.md           # 구현 진행 로그
├── verification.md       # 빌드/리뷰 검증
├── test-report.md        # 테스트 결과
└── retro.md              # 회고
```

- 양식은 `.claude/skills/loop/templates/`의 동명 템플릿을 복사해 채운다.
- 멀티에이전트로 돌 때, 각 단계 서브에이전트는 **이 폴더의 파일로만** 인수인계한다
  (서브에이전트는 대화 맥락을 공유하지 못하므로).
- 자세한 루프 정의: `.claude/skills/loop/SKILL.md`.
