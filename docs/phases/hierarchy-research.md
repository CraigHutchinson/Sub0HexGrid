# Next phase: hierarchy research, review and architecture

Requested 2026-10-02 during H2 delivery. Outcome: select whether and how a multilevel
representation supports processing, zoom/LOD and wide spatial queries across
Sub0HexGrid, Crucible and Sub0ECS. Initial [research](../research/hierarchy.md) and
cross-repository read-only review are complete; this phase does not ship a tree API.
H2 remains the fine-grid reference. HX-05 tighter local candidates is a separate
complementary refinement, not a substitute for this hierarchy investigation.

## Division and sequence

| Package / owner | Exclusive work and handoff | Gate |
|---|---|---|
| HH-01 / H geometry researcher | Source/algorithm comparison in docs/research/hierarchy.md and H brief; exact logical vs geometric nesting, negative addressing, bounds, index widths | Primary sources and explicit remapping to planar q/r; initial pass complete |
| HH-02 / Crucible spatial reviewer | Read-only workload/snapshot/LOD requirements; proposed application tree/summary layout and update policy | Real receiving scenarios, fixed ECS pin, deterministic IDs/replay and memory/time limits |
| HH-03 / ECS reviewer | Audit bounded gathering, stable IDs, structural invalidation and executor barriers against Crucible's actual dependency | Narrow upstream gap report; no speculative hierarchy ownership in ECS |
| HH-04 / integrator + H | Join findings into accepted architecture ADR and interface/dataflow proposal | cpp-review plan mode, independent geometric review, all owners' interaction and handoffs explicit |
| HH-05 / integrator | Bounded comparison spike only after HH-04 approves its scope | Doctest exact oracle, CPM nanobench consumer baseline, resumable build/query/update, footprint and latency evidence |

HH-01 and HH-02/03 can research independently. HH-04 waits for both geometry and
receiving contracts; implementation workers do not guess parent or snapshot APIs.
Integrator owns central docs/catalog and cross-repo proposals. Read-only sibling
research is complete; changes to Crucible/ECS require their own active claims,
instructions and reviewed work packages. Record exact bases and fresh host resources
before prototypes. Native compute remains an external adapter activity.

## Required architecture decision

Compare aperture/hex path schemes, dense axial block summaries, sparse linear trees
and chunk/entity BVHs. Consider locality ordering separately from hierarchy shape.
An ADR must specify exact fine-to-parent assignment and coverage, logical vs display
geometry, count/address limits, clipping, overlap/deduplication and order. Decide
which reusable geometry is consumed from Sub0HexGrid and which occupancy/state is
owned in Crucible. Freeze snapshot generation, row-to-ID mapping, invalidation or
lease lifetime, publication barriers and tick-start/post-movement separation.

Keep identity domains distinct: snapshot epoch, snapshot row, stable application
SampleId and ECS slot/generation. Future spawn/destroy and delayed query-to-ECS writes
need liveness validation and explicit handle/ID reuse or exhaustion policy; current
fixed-population uint64 SampleId is not an indefinite-uniqueness proof for ECS handles.
Yielded gathering must read one frozen tick epoch. Block movement/structural commits
until gathering finishes, or gather from an already owned immutable source snapshot.
Pending builders write only pending leaves/summaries; readers use committed epochs.

Design reset/gather/map/sort/scan/scatter/ancestor aggregation and incremental dirty
updates as bounded stages; sorting/gather cannot hide an unbounded call inside a
nominal step. Traversal budgets include nodes and leaf entities with retained frontier
state, explicit capacity failure and zero-budget behavior. Time deadlines are checked
between workload batches; no clock/scheduler belongs in the scalar geometry kernel.
Document mutation/read sets, concurrency and cancellation before parallel execution.

Zoom/LOD selects an application aggregate with an explicit error/policy contract;
exact query enumeration remains complete and inclusive. Counts/aggregates and
approximate steering must be named distinct operations. State whether coarser
processing can lag and how conservative bounds stay valid during motion. Preserve
ID-sorted reduction/replay unless a separately approved deterministic policy replaces it.

For accelerators, propose flat node/key/child-range/summary arrays, fixed-width
index limits and conservative geometry precision. Rebuild/refit, CPU transfers,
dispatch and completion barriers must be measured end-to-end by a named adapter.
Do not encode C++ object bytes, size_t, virtual interfaces or device handles into
Sub0HexGrid. Compare level-order processing, compact traversal and bounded frontier
capacity without assuming they have identical CPU/GPU costs.

## Comparison and exit

Reuse H2 exact brute-force fixtures and 100k/500k/1m sizes, adding sparse worlds,
moving fractions, mixed query radii and coarse zoom/aggregate outputs. Include uniform,
clustered, border, empty, coincident and world-wide queries. Measure construction,
incremental update, publication, node/leaf visits, exact predicates, output ordering,
peak pending/scratch payload and consumer p95/p99 separately. Exact K-output work
is unavoidable; wide aggregate queries and empty-region pruning are different wins.
Rotate controlled arms and qualify thermal/cache drift; CI smoke is not timing proof.

Exit with an accepted/deferred architecture ADR, comparison rationale, explicit
cross-repo ownership, actual receiving caller, stable follow-ups and a bounded next
implementation sprint. Any public API waits for this gate. Unknown workload/time/LOD
inputs remain visible; do not fabricate a frame budget or claim hierarchy pays merely
because prior art exists. Initial research recommends testing exact fine-cell grouping
with conservative parent bounds first; final selection remains open.

## Initial plan review

Independent geometry/source and receiving-integration reviewers found no blocking
findings. The integrator incorporated their precision improvements: explicit identity
domains, liveness/reuse policy and frozen gather epochs. Research, architecture
selection and measured prototype remain separate gates; there is no guessed public
interface or automatic downstream implementation dispatch.
