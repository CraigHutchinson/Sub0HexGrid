# Incremental work breakdown

| Phase | Deliverable | Exit |
|---|---|---|
| H0 architecture | Requirements, ownership, source-backed conventions, C++ setup | Plan reviewed before code |
| H1 scalar kernel | Axial topology, pointy geometry, example, fixtures, package and CI | Reviewed code; Debug/Release/sanitizers and independent consumer pass |
| H2 bounded regions | Finite shape/storage mapping and complete inclusive query candidates | Real spatial caller; brute-force completeness, numeric/capacity/border tests |
| H3 Crucible adoption | Pinned spatial consumer, application-owned bins and exact filtering | Scenario/query/replay evidence; separate Blight adjacency decision |
| H4 measured refinement | Real workload improvements and extra consumed operations | Controlled evidence, package consumers and compatibility |

H0/H1 are the authorized initialization. H2/H3 are planned, not automatically started.
Review boundaries and reuse at each phase start; use small consumed increments.
Library geometry and Crucible gameplay are separate owners. A second consumer can
justify broader facets but must not be fabricated. Record upstream/consumer commits.

Before each public API, cpp-write and cpp-review plan mode assess named callers,
type invariants, failure semantics and reinvention. After code, cpp-review assesses
the actual source. Package/fixture correctness remain independent gates. No
research delegation, extra workers or optimization campaign is needed for H1.
