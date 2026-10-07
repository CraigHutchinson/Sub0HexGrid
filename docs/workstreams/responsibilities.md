# Responsibility and handoff map

This map applies to hierarchy and acceleration without selecting a representation.
Architecture owns numerical contracts; stream briefs own component scope; phase
plans assign agents and private spike paths. [The experiment programme](../phases/hierarchy-research.md)
freezes a protocol before spikes and selects production architecture after evidence.
Application responsibilities are recorded in Crucible's corresponding
[workstream map](https://github.com/CraigHutchinson/Crucible/blob/main/docs/workstreams/hierarchy-boundaries.md).
These documents define project boundaries, not an installed dependency or dispatched team.
They are current assignments, not a ban on a later evidence-backed extraction.
Any reusable borrowed traversal/reducer moved upstream needs an explicit ADR and
updated H/Spatial ownership; no spike can imply that transfer by itself.

## Sub0HexGrid owners

| Stream | Owns / hands off | Uses / receiving caller | Does not own |
|---|---|---|---|
| T | Fine Axial/direction arithmetic, exact distance, checked integer domain | G/R, proposed H; scalar callers | Parent addressing, occupied nodes, navigation graph/costs or game adjacency |
| G | World/layout transforms, finite-precision assignment and primitive geometry semantics | T; Q, proposed H/X and world mapping callers | Camera projection, LOD thresholds, composed descendant coverage or occupancy |
| R | Fine finite-region membership, ordering and coordinate/index bijection | T; Q, proposed H and caller storage addressing | Parent keys, sparse chunk directory, entity bins, allocation/residency policy |
| Q | Complete fine-cell candidate range/slices/cursor, query validity and deterministic cell order | G/R; example, package and proposed application query adapter | Occupancy pruning, exact entity filtering, ID sorting, aggregates, owned frontier or LOD |
| H | Parent/group addressing and composed conservative descendant-coverage rules; source proofs and experiment hypotheses | T/G/R conventions; compatible with Q reference; proposed spatial/view experiments | Occupied tree storage/build/refit/traversal, summary meanings, view/nav/game policy or ECS storage |
| X | Consumer-gated fixed-width interchange, buffer layout and declared CPU/backend parity | Selected G/R/Q contracts and H only if hierarchical data is exchanged; named external adapter | Native kernels, device allocator/queue/dispatch/barriers, backend selection or scheduling |
| I | Protocol/common fixtures, root inventories/exports/pins/CI, shared contract review, measured comparison coordination and delivery | All stream handoffs and named package/application callers | Redefining owner semantics, choosing a hierarchy before evidence or editing worker paths without handoff |

H's conservative coverage composes G's world conventions; a primitive transform
change belongs to G, while parent assignment/enclosing-descendant derivation belongs
to H. R indices still address the fine region. H parent keys are a distinct domain,
not a reinterpretation of R indices. Q's baseline remains usable independently of H.
An application may skip an occupied subtree using validated H coverage and descend
to fine candidates, but this does not transfer occupancy ownership into Q/H.

## Dependencies and authority

Arrows mean provider to consumer: T -> G/R; G/R -> Q; T/G/R -> proposed H;
selected G/R/Q/H contracts -> X; applications compose these facets. H and Q do not
call each other. H uses Q's semantics/oracles as a compatibility reference, not as
a source dependency. X never becomes a dependency of the scalar facets. I is a
coordination role, not a runtime module. Navigation and Presentation do not become
dependencies of Sub0HexGrid.

Every shared contract change names its defining owner, all consumers and the I
integration patch. Acceptance involves affected owners; it does not grant them
joint edit ownership of the same file. New consumed primitives are proposed to
their defining stream before a worker duplicates transforms/indexing privately.
Private experiment variants can differ deliberately, but declare differences and
retain oracle comparability; they cannot silently rewrite production contracts.

## Application and infrastructure boundaries

| Artifact / decision | Accountable owner | Handoff and lifetime |
|---|---|---|
| ECS values, handle/row invalidation, executor capability | Sub0ECS; Crucible I audits exact pin | Borrow rules and capabilities; no spatial tree or application ID policy imported into ECS |
| Gathered stable-ID/position input and tick publication | Crucible I/Simulation | One frozen tick epoch, copied values or explicit leases; movement/structural barrier until gather ends |
| Occupied nodes, bins, bounds cache, base counts, update/query frontier | Crucible Spatial | Pending build/update and coherent committed leaves+summaries; bounded node AND leaf work; exact query outputs |
| Domain reducers and derived summaries | Respective Fields/Blight/Interactions owner | Meaning, units, composition and invalidation; Spatial supplies addressing/traversal, not domain policy |
| Mini-map/zoom view cache, visual reducers and picking | Crucible Presentation | Reads owned committed snapshots; owns approximation, transforms, markers and GPU upload lifetime; emits commands through Runtime |
| Motion math and ordered neighbor consumption | Crucible Swarm | Immutable tick input + spatial/field observations -> next-state proposals; no index mutation or renderer dependence |
| Route/flow/portal experiments | Phase-assigned navigation researcher, accountable to Crucible I until an ADR names a production module | Terrain/connectivity/capability and route-quality contracts; no implicit transfer to H, Fields or Swarm |
| When work runs / what constitutes a completed tick | Crucible Runtime + I | Admission, deadlines, pause/replay and publication policy; domain cursors merely report progress/completion |
| How work is executed and joined | Crucible Scheduling / named backend adapter | Access sets, executor DAG, completion and failure joins; adapters own device resources and barriers |
| Counters / conclusions | Telemetry owns bounded records; Validation owns comparison protocol | Versioned observations and raw artifacts; no logging-thread ECS/tree traversal or benchmark thresholds invented by a spike |

Spatial's base occupancy/count summaries are index data. Application mass, terrain,
field integration and Blight display reductions have distinct semantic owners.
Presentation may own a copied/view-specific hierarchy; it does not modify the
authoritative spatial index or borrow live ECS rows. Separate structures per use
remain possible, joined by explicit snapshot identity and fine coordinates.

## Interface handoff checklist

Every owner records its top-down receiving behavior and bottom-up capability/limit,
then hands off one reconciled contract. Presentation supplies visible mini-map/zoom/
picking and interaction requirements; Spatial/H supply exact coverage and cost limits.
Runtime/Contracts reconcile feedback, latency and snapshot lifetime. Applicable audio
or background-fidelity research receives a W0-assigned application owner; it does not
move into geometry or silently become implemented. A phase closes with the next
balanced consumer slice, not only completion of independent foundations. Concept,
synthetic, native graphics/input/audio and participant evidence remain separate.

Sub0HexGrid I owns its standalone spike protocol/harness; Crucible Validation owns
consumer extensions, replay/render/navigation evidence and artifacts in Crucible.
They agree shared inputs/counters through the integrators, not joint edit ownership
of one file. Each timed run has one resource coordinator.

For each proposed interaction record one defining owner and one receiving caller,
input/output identity domains, numeric/boundary/order policy, read/write sets,
capacity/overflow/failure, borrow expiry or lease, snapshot generation and publication
barrier. Include work units, zero-budget behavior, dense-leaf/frontier progress,
cancellation and who schedules the next batch. Add exact source/config/input hashes,
oracle, raw evidence and shared patches to the phase handoff.

A version detects replacement but does not extend storage lifetime. A query work
budget does not authorize concurrent mutation or guarantee a wall-clock deadline.
A display aggregate cannot certify exact membership or navigation connectivity.
Native hardware execution is not implied by portable descriptors or host emulation.

## Audit findings and resolution

2026-10-02: refreshed every T/G/R/Q/H/X/I brief; removed pre-H2 shared-test claims,
assigned H grouping vs G primitive geometry vs R fine addressing vs Q candidates,
and added conditional H input to X. Clarified application occupied traversal,
summary semantics, view/nav ownership, Runtime/Scheduling separation and ECS pin/lifetime
review. Experimental common adapters remain I-owned and private spike paths receive
one phase-assigned owner. Self-review found no unresolved ownership cycle or overlap;
no public API, production hierarchy, new navigation module or dispatch is claimed.
