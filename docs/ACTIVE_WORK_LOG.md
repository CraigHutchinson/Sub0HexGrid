# Active work

| Owner | State | Scope / resource | Base |
|---|---|---|---|
| Architect | Complete increment | Initial kernel/docs/package; CPU released | 2026-10-01 / 9a4e604 / initial-kernel |
| Codex | Complete increment; CPU released | CMake/CPM, tests, benchmarks, architecture/catalog/skill; [evidence](phases/tooling-and-boundaries.md), [PR 2](https://github.com/CraigHutchinson/Sub0HexGrid/pull/2) | 2026-10-02 / 59eddb5 / test/doctest-nanobench |
| Codex sprint planning | Complete planning; no CPU held | [H2 sprint definition](phases/h2-bounded-spatial.md), phase/catalog links and work breakdown; no workers dispatched | 2026-10-02 / 0953973 / plan/h2-sprint |
| Codex H2 integrator | Implementation complete; CPU released | [PR 4](https://github.com/CraigHutchinson/Sub0HexGrid/pull/4), serial Debug/Release, package/allocation/runtime-only checks and controlled baseline complete; final CI/delivery closing | 2026-10-02 / d24d08a -> 96d8b15 / implement/h2-bounded-spatial |
| H2 R worker | Complete | Regions implementation and independent Q/numerical/benchmark review; no builds/commits | 2026-10-02 / d24d08a / integrator branch |
| H2 Q worker | Complete | Candidates implementation and independent caller/lifetime review; no builds/commits | 2026-10-02 / d24d08a / integrator branch |
| H hierarchy review | Complete research/plan increment; no CPU held | Primary-source and sibling read-only research, hierarchy docs/phase and independent geometry/integration plan review; no blocking findings | 2026-10-02 / 96d8b15 / integrator branch |
| H use-case iteration / integrator | Complete plan increment; no CPU held | Prioritized game-use matrix, competing spikes/phases A-D, evidence template, catalog and architecture; source-backed read-only receiving audit and plan self-review; PR records delivery; no spikes measured | 2026-10-02 / 59e56f5 / plan/hierarchy-use-case-spikes |
| Responsibility audit / integrator | Complete documentation increment; no CPU held | All stream briefs and ownership map, catalog/architecture/agent guidance; matching Crucible handoffs; link/coverage checks and self-review; PR records delivery; Sub0ECS read-only | 2026-10-02 / aaae5c2 / docs/hierarchy-responsibilities |

Local Debug/Release and ASan/UBSan gates passed; see validation.md and review.md.
Exact-head CI and merge are recorded in the PR. H2 is implemented; H3 adoption and
hierarchy implementation are unstarted. Hierarchy research/architecture phase is defined.

Resumed 2026-10-07: original responsibility commits were pushed but PR creation
stopped at an approval-service usage limit. Access recovered; audit reconciled with
current Crucible main and standing consumer/foundation methodology. Documentation
only, no CPU reservation; final PR/CI delivery follows. Crucible now consumes the
H2 pin aaae5c2; the earlier H3-unstarted status above is historical.
