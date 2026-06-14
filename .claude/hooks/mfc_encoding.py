#!/usr/bin/env python3
# Project hook shim -> delegates to the bundled khg-mfc encoding core.
# (이 세션의 settings.json 훅이 이 경로를 가리키므로 유지. 실제 로직은 번들 코어.)
import os, sys, runpy, json
target = os.path.normpath(os.path.join(
    os.path.dirname(os.path.abspath(__file__)),
    "..", "skills", "khg-mfc", "scripts", "mfc_encoding.py"))
try:
    sys.argv[0] = target
    runpy.run_path(target, run_name="__main__")
except SystemExit:
    raise
except Exception:
    # fail-open: 코어가 없거나 깨져도 편집을 막지 않는다
    if len(sys.argv) > 1 and sys.argv[1] == "guard":
        print(json.dumps({"hookSpecificOutput": {
            "hookEventName": "PreToolUse", "permissionDecision": "allow"}}))
    sys.exit(0)
