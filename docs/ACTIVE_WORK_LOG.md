# Active work

| Owner | State | Scope / resource | Base |
|---|---|---|---|
| Architect | Complete increment | Initial kernel/docs/package; CPU released | 2026-10-01 / 9a4e604 / initial-kernel |
| Codex | Complete increment; CPU released | CMake/CPM, tests, benchmarks, architecture/catalog/skill; [evidence](phases/tooling-and-boundaries.md), [PR 2](https://github.com/CraigHutchinson/Sub0HexGrid/pull/2) | 2026-10-02 / 59eddb5 / test/doctest-nanobench |
| Codex sprint planning | Complete planning; no CPU held | [H2 sprint definition](phases/h2-bounded-spatial.md), phase/catalog links and work breakdown; no workers dispatched | 2026-10-02 / 0953973 / plan/h2-sprint |
| Codex H2 integrator | Active; CPU reserved | G observations, contract ADR, root wiring, split scalar fixtures, spatial example/package/allocation checks, benchmarks and docs; controlled H2 measurements authorized on free host by user; serial builds/benchmarks | 2026-10-02 / d24d08a / implement/h2-bounded-spatial |
| H2 R worker | Active | regions headers/source/tests/benchmarks only; shared tree, disjoint paths, no shared manifests/builds/commits | 2026-10-02 / d24d08a / integrator branch |
| H2 Q worker | Active | candidates headers/source/tests only; frozen ADR/R contract; shared tree disjoint paths, no shared builds/commits | 2026-10-02 / d24d08a / integrator branch |

Local Debug/Release and ASan/UBSan gates passed; see validation.md and review.md.
Exact-head CI and merge are recorded in the PR. H2/H3 and future surfaces are unstarted.
