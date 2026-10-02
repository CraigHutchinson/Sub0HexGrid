# Tooling and workstream boundaries — 2026-10-02

Base: 59eddb5. Outcome: CPM-backed doctest/nanobench tooling and independent-agent
component boundaries. Owner: Codex integrator; branch test/doctest-nanobench.
No runtime public API changes or workers dispatched.

| Stream | Disposition | Reason |
|---|---|---|
| I | Retain; current sole owner | Shared tooling/docs must be consistent |
| T/G | Consolidate fixture migration under I | Shared kernel.cpp; no numerical changes |
| R/Q | Defer implementation | Shape/index/candidate contracts and real caller not frozen |
| X | Defer implementation | External backend caller/capabilities not selected |

Shared surfaces: CMake/pins/CI, central docs and fixtures. Prerequisites were inspected
repository/parent architecture, upstream releases, C++ and skill guidance.

## Evidence and review

Windows MSVC 19.51: Debug and Release build/CTest passed 3/3 each, including relocated
package consumer. Doctest: 2 cases and 52,736 assertions passed. Release nanobench
smoke passed four operations; no performance conclusion. Fresh runtime-only
configure/build passed with tests/examples/benchmarks OFF and no dependency fetch.
Local documentation links, whitespace and project/installed skill validation passed.

Cpp-review plan/code self-review checked executable callers, private dependencies,
explicit sources, benchmark setup/dead-code protection, unchanged numerical fixtures
and package boundaries. It corrected the benchmark command to its benchmarks/
subdirectory before delivery. No unresolved blocking finding; not independent review.
Exact-head CI passed for f0a53ae3375eb8f4c74fa07bd0ee4becc2097f48: Linux and
Windows Debug/Release plus ASan/UBSan, including Release benchmark smoke.
[PR 2](https://github.com/CraigHutchinson/Sub0HexGrid/pull/2) records final delivery
and the merge baseline.

## Follow-ups and next split

| ID | Owner | Closure gate |
|---|---|---|
| HX-01 | I + Crucible spatial owner | Freeze shape, boundaries, capacity, occupancy/time budget and caller |
| HX-02 | I/T/G | Separate fixtures/manifests preserving assertions/oracles before parallel edits |
| HX-03 | R then Q | Checked bijection and inclusive traversal contracts with brute-force fixtures |
| HX-04 | X + backend owner | Named caller, layout/precision/seam policy and capability evidence |

Next phase is proposed: retain I, separate R from G/T after contract freeze; Q can
prepare independent completeness oracles before R handoff. X stays deferred. Shared
fixtures are the coordination risk, so separate them before dispatch. Artifacts
remain under build/; no other repository/worktree was edited or removed.
Maintained skill: skills/parallel-workstreams; installed user-wide at
~/.codex/skills/parallel-workstreams. GitHub records exact-head PR/merge; refresh the
phase index from that baseline before next dispatch.
