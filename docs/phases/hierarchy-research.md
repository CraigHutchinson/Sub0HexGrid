# Hierarchy programme: research, competing spikes and measured architecture

Requested 2026-10-02 during H2 delivery and expanded by user direction to prioritize
mini-map, top-down zoom, unit motion/navigation and other game uses. Outcome:
evaluate multiple competing representations over several phases before selecting
whether and how they integrate across Sub0HexGrid, Crucible and Sub0ECS. Initial
[research](../research/hierarchy.md) and cross-repository read-only review are complete.
The [use-case/experiment programme](../research/hierarchy-experiments.md) specifies
priorities, spike options, workload packs and evidence rules. No tree API is selected.
Use the [responsibility map](../workstreams/responsibilities.md) before assigning
each spike or consumer: defining semantics and production paths stay with their
stream, even when a private experiment implements several layers for measurement.
H2 remains the fine-grid reference. HX-05 tighter local candidates is a separate
complementary refinement, not a substitute for this hierarchy investigation.

## Division and sequence

| Package / owner | Exclusive work and handoff | Gate |
|---|---|---|
| HH-01 / H geometry researcher | Source/algorithm comparison in docs/research/hierarchy.md and H brief; exact logical vs geometric nesting, negative addressing, bounds, index widths | Primary sources and explicit remapping to planar q/r; initial pass complete |
| HH-02 / Crucible spatial reviewer | Read-only workload/snapshot/LOD requirements; proposed application tree/summary layout and update policy | Real receiving scenarios, fixed ECS pin, deterministic IDs/replay and memory/time limits |
| HH-03 / ECS reviewer | Audit bounded gathering, stable IDs, structural invalidation and executor barriers against Crucible's actual dependency | Narrow upstream gap report; no speculative hierarchy ownership in ECS |
| HH-04 / integrator + H | Freeze experimental protocol, fine-reference invariants and temporary adapter/snapshot contracts | Plan/numeric review; permits private spikes, does not select production architecture |
| HH-05 / spike owners + integrator | Round one: flat reference plus dense pyramid, sparse linear tree and chunk BVH | Shared W0-W3 fixtures, doctest correctness, CPM nanobench full-cycle/byte evidence; independent paths and serial measurements |
| HH-06 / geometry, presentation and navigation reviewers | Round two: hex-native addressing, mini-map/zoom cache and fine/portal/shared-goal navigation comparisons | W0-W4, actual consumer semantics; navigation connectivity is not occupancy, rendered evidence distinct from headless export |
| HH-07 / spike owners + integrator | Iterative refinement, selected combinations and moving/dynamic terrain cases | At least two measured rounds; bounded tuning passes, regressions and noise recorded; parked options reproducible |
| HH-08 / application/backend owners | Integrate surviving options in real view/motion/navigation traces; external device trial only with named caller | End-to-end tick/frame cost, quality/replay, uploads/barriers, memory ceiling and scheduler contention |
| HH-09 / integrator + affected owners | Accept/defer per-use-case architecture and next implementation increment | Evidence-led ADR after competing spikes and consumer packs; may choose several structures or flat fallback |

HH-01 and HH-02/03 can research independently. HH-04 waits for both geometry and
receiving contracts; implementation workers do not guess parent or snapshot APIs.
Integrator owns central docs/catalog and cross-repo proposals. Read-only sibling
research is complete; changes to Crucible/ECS require their own active claims,
instructions and reviewed work packages. Record exact bases and fresh host resources
before prototypes. Native compute remains an external adapter activity.

Phase A is HH-01..04: protocol and receiving contracts. Phase B is HH-05: first
competing CPU spikes. Phase C is HH-06/07: broaden consumer comparisons and iterate
with measured bottlenecks. Phase D is HH-08/09: application evidence and architecture
selection. Each phase closes with raw evidence, review, remaining questions and next
assignments. The initial architecture review freezes safe experimentation boundaries;
it cannot require choosing the winning structure before those experiments exist.

Retain one integrator; consolidate shared fixtures/protocol under I. Split workers
by competing spike only after common inputs/outputs freeze; each owns its private
spike sources/tests/docs and requests shared wiring from I. Reassess team size per
round and serialize timed runs. Presentation/navigation own consumer adapters, not
generic tree policy. Defer production APIs and native backends until evidence gates.
This documentation iteration defines the programme; it does not dispatch prototype
workers or report measurements that have not been taken.

## Protocol invariants and eventual architecture decision

Compare aperture/hex path schemes, dense axial block summaries, sparse linear trees
and chunk/entity BVHs. Consider locality ordering separately from hierarchy shape.
The protocol ADR must specify each experiment's fine-to-parent assignment and
coverage, logical vs display
geometry, count/address limits, clipping, overlap/deduplication and order. Decide
which reusable geometry is consumed from Sub0HexGrid and which occupancy/state is
owned in Crucible for the temporary comparison adapters. Freeze snapshot generation,
row-to-ID mapping, invalidation or
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

Mini-map and top-down zoom run alongside motion in the first consumer pack;
navigation reference/contracts start in Phase A and route/flow comparisons run in
Phase C. All are mandatory before selection. Wide counts/selection are next;
fog/streaming/terrain and later worlds are explicitly deferred consumers, not ignored.
Use [the workload and spike matrix](../research/hierarchy-experiments.md) rather than
substituting one radius-query benchmark for every game operation.

Reuse H2 exact brute-force fixtures and 100k/500k/1m sizes, adding sparse worlds,
moving fractions, mixed query radii and coarse zoom/aggregate outputs. Include uniform,
clustered, border, empty, coincident and world-wide queries. Measure construction,
incremental update, publication, node/leaf visits, exact predicates, output ordering,
peak pending/scratch payload and consumer p95/p99 separately. Exact K-output work
is unavoidable; wide aggregate queries and empty-region pruning are different wins.
Rotate controlled arms and qualify thermal/cache drift; CI smoke is not timing proof.

HH-04 exits with reviewed protocol and private-spike contracts. HH-05..08 exit with
comparisons and follow-up questions, not a premature default. HH-09 exits with an
accepted/deferred architecture ADR, comparison rationale, explicit
cross-repo ownership, actual receiving caller, stable follow-ups and a bounded next
implementation sprint. Any production public API waits for this gate. Unknown workload/time/LOD
inputs remain visible; do not fabricate a frame budget or claim hierarchy pays merely
because prior art exists. Initial fine-cell grouping is one testable hypothesis;
no specific representation, single shared hierarchy or hex parent shape is preferred
as the outcome. Final selection remains open over multiple measured phases.

## Initial plan review

Independent geometry/source and receiving-integration reviewers found no blocking
findings. The integrator incorporated their precision improvements: explicit identity
domains, liveness/reuse policy and frozen gather epochs. Research, architecture
selection and measured prototype remain separate gates; there is no guessed public
interface or automatic downstream implementation dispatch.

The use-case iteration received a plan self-review against repository/parallel-workstream
and cpp-review boundary principles. It corrects selection-before-spikes sequencing,
distinguishes proposed navigation/render consumers from implemented snapshot export,
assigns disjoint private experiment paths and separates exact/visual/path-quality gates.
No blocking plan finding remains. This is not an independent review of prototype
code or evidence that any hierarchy performs better; those gates remain in each round.
