# Hierarchy experiments driven by game use cases

Direction: 2026-10-02. This is a prioritized experimental programme, not a selected
acceleration structure. Multiple options must be spiked and iterated across phases
before outcome selection. Existing H2 evidence is a baseline, not hierarchy evidence.
See [phase gates](../phases/hierarchy-research.md) and [prior art](hierarchy.md).

## Priorities and receiving operations

Priorities describe evaluation order, not permission to weaken lower-tier correctness.
Motion and both views form the first shared workload pack. Navigation contracts and
reference fixtures are defined in that same phase; navigation comparisons follow in
the next pack because terrain/traversability is not yet an implemented game service.
All four requested uses are evaluated before architecture selection. Product timing,
memory and visual-quality budgets remain explicit inputs, not invented FPS targets.

| Priority / use | Receiving owner and operation | Correctness or quality contract | Decisive measures |
|---|---|---|---|
| P0: unit motion and local interactions | Crucible Spatial/Swarm: tick-start neighbors, field influence and moving populations | Complete inclusive fine-point queries; stable IDs, deterministic reductions, no stale-summary pruning | Gather/update/rebuild + all tick queries + result ordering; p95/p99, worst batch work/time, dirty propagation and memory |
| P0: mini-map | Crucible Presentation: whole-world density, Blight/territory, structures/objectives and camera footprint | Exact counts/conserved mass where requested; explicit display binning/filter and freshness policy; important markers not lost in averaging | Full/dirty refresh cost, output/upload bytes, fixed-resolution image error, marker preservation and snapshot age |
| P0: top-down zoom in/out | Crucible Presentation: viewport selection, close units/cells, strategic density/motion | Conservative visible selection, exact picking via leaf fallback; explicit aggregate error, transition/hysteresis and label policy | Pan/zoom trace latency, visited nodes, emitted primitives, aggregate build cost, transition popping and upload volume |
| P1: navigation and movement planning | Proposed Crucible navigation consumer: reachability, routes, shared-goal fields, terrain/bridge edits | Actual fine connectivity, legal clearance/costs and no blocked route; state whether routes are optimal or bounded-suboptimal | Preprocessing + terrain-edit repair, route latency/expanded nodes, path-cost ratio, memory, many-unit shared-goal amortization |
| P1: selection and spatial commands | Crucible input/interaction adapter: click, rectangle/disk selection, brush/field edits | Complete selected identities at a declared snapshot; exact boundary/picking semantics | Candidate excess, output/order cost, brush extent, dense result backpressure |
| P1: wide counts and awareness | Proposed gameplay/inspection consumer: population/mass/territory reductions over areas | Reducer identity/composition, no double counting, boundary refinement, predicate/version validity | Fully accepted nodes vs boundary leaves, aggregate maintenance, exact reduction parity |
| P2: fog/visibility and interest scheduling | Proposed application consumer: observable regions and processing interest | Occlusion/visibility is separate from occupancy; approximation and staleness explicitly approved | Dirty-area refresh, conservative coverage, false-positive work, publication lag |
| P2: terrain/chunk streaming, broad phase and later worlds | Future application/backend consumers, not current library APIs | Residency/missing-data and swept bounds explicit; height/physics/spherical identity require separate contracts | I/O/transfer cost, working set, moving-bound refit and mixed-size behavior |

Navigation is not merely radius querying, and zoom is not navigation at a coarse
level. An occupied chunk does not prove a traversable path or clearance. Terrain
costs, bridges, portals and unit capabilities belong to the application. Visual
aggregation must not change Blight adjacency, motion rules or exact query semantics.

The receiving intent is real: Crucible's [game design](https://github.com/CraigHutchinson/Crucible/blob/7692049a2c3b596f35ab27d159efdc215b52f84e/docs/game-design.md)
describes close individual units and strategic density under orthographic pan/zoom,
and a mini-map showing territory, concentration, objectives and camera footprint.
Its current presentation is owned snapshot inspection/SVG export, not a live GPU
mini-map or navigation implementation. Synthetic adapters are evidence for algorithms;
they are not proof of shipping renderer performance or adopted game behavior.

## Competing spikes

Use the same fine assignments, samples, predicates and output contracts. Keep
representation-specific storage and algorithms private to independently runnable
spikes. A comparison adapter is experiment scaffolding, not a public virtual backend
API. Address ordering (row-major/Morton/Hilbert), tree shape and summary payload are
separate axes; compare them incrementally rather than a full combinatorial sweep.

| ID | Candidate | Question and costs to expose |
|---|---|---|
| HS-00 | Flat H2 bins/candidates; direct aggregate raster and fine search references | Baseline including gather, publication, sorting and output; identify when a hierarchy adds more maintenance than it saves |
| HS-01 | Dense axial block occupancy/count pyramid | Can regular arithmetic parents and level arrays serve empty-space pruning and zoom summaries cheaply? Sweep block size/depth; expose dense-world metadata/reset costs |
| HS-02 | Sparse linear quadtree/radix hierarchy over occupied axial blocks | Does avoiding absent nodes repay key generation/sorting and frontier cost? Compare dense vs sparse arrays under identical grouping before adding locality encoding |
| HS-03 | Linear BVH over occupied chunks, with an entity-leaf variant if justified | Do data-dependent bounds help clusters/mixed extents? Compare rebuild/refit quality, overlap, movement degradation and bounded traversal; include node footprint |
| HS-04 | Hex-native hierarchical/path addressing (aperture/central-place candidates) | Prototype sourced parent assignment and coverage separately from display hex shape; quantify rotation/overlap, addressing, locality and conservative excess against axial grouping |
| HV-01 | Fixed-resolution aggregate raster/pyramid; optional viewer-centred clipmap cache | Can a view-specific cache serve mini-map/zoom better than querying an entity tree? Compare full refresh, dirty tiles and pan/zoom reuse; separate view cache memory from gameplay index |
| HN-01 | Fine Dijkstra/A* vs region/portal abstraction | Can hierarchical navigation reduce searches without illegal paths or uncontrolled cost inflation? Fine connectivity/clearance remains the oracle; occupancy summaries alone are insufficient |
| HN-02 | Shared-goal integration/flow fields vs individual routes | How much work is amortized across many units with common goals? Measure field rebuild, dynamic edits and motion consumer cost; do not equate a player-painted vector field with a computed navigation field |

Round one implements HS-00 and at least three independent options HS-01/02/03.
HS-04 remains an explicit second-round candidate, not silently excluded by the
initial block hypothesis. It can be rejected on a documented coverage/domain failure,
but not on a guessed performance result. HV-01 and HN-01/02 are orthogonal consumer
comparisons, not extra tree variants. No option is selected before multiple spikes
and consumer packs have been evaluated. Retaining separate structures per use or
retaining the flat baseline are valid outcomes.

Additional primary references: [geometry clipmaps](https://developer.nvidia.com/gpugems/gpugems2/part-i-geometric-complexity/chapter-2-terrain-rendering-using-gpu-based-geometry)
use nested viewer-centred regular-grid caches for terrain LOD; this motivates a
view-cache comparison, not importing its historical GPU timings or square terrain
mesh into hex simulation. [HPA* research](https://webdocs.cs.ualberta.ca/~games/pathfind/)
provides a coarse/fine path abstraction to investigate, with its near-optimal rather
than universally optimal contract. [Shared-goal flow fields](https://www.redblobgames.com/pathfinding/tower-defense/)
reuse a goal-centred search across units. These are prior-art mechanisms, not measured
recommendations for Crucible. Re-derive six-neighbor, weighted/clearance and dynamic
map conventions from actual reference sources before prototype code.

## Common workload packs

W0 correctness: exhaustive small mixed-sign maps, seams/region edges, empty and
coincident populations, inclusive queries, parent coverage and deduplication.
Verify reductions against a direct fine scan, visible sets/picking against an
independent fine viewport oracle, and paths against a fine graph search. Define
zero-progress, cancellation, frontier exhaustion and snapshot invalidation fixtures.

W1 resident scale: H2 100k/500k/1m cells/entities, varying those axes independently.
Uniform, clustered, border, sparse and coincident distributions; radius/cell ratios
0/2/8 plus broad/world-covering queries. Preserve exact outputs and report unavoidable
K-output work. Small local-query gains must not hide large-query regressions.

W2 motion/update traces: 0/1/10/100% movers, coherent flows, teleports and fine/coarse
boundary crossings. Include sparse changes and clustered churn. Record old/new
membership, reset/dirty reduction and parent-bound propagation, rejected updates and
atomic leaf/summary publication. Add spawn/destroy/predicate changes only after the
identity/reuse contract freezes; mark unsupported cases explicitly.

W3 view traces: a fixed whole-world mini-map (256 and 512 pixels per axis), viewport
sizes 1280x720 and 1920x1080, deterministic zoom from fine cells to whole-world and
back, continuous pan, rapid reversals and jumps across region boundaries. These are
fixture sizes, not product requirements. Compare preserved counts/mass, important
markers, world/camera transforms, aggregate error and temporal transitions. Include
two simultaneous views; a camera-local cache cannot implicitly satisfy the mini-map.
Headless traces measure CPU selection/export and bytes; actual render/upload/GPU
claims require a named presentation backend and captured visual evidence later.

W4 navigation: uniform/weighted passable terrain, isolated islands, narrow corridors,
obstacles crossing coarse boundaries and bridge/cost changes. Compare individual and
shared goals, unit clearance classes and local repair vs complete rebuild. Costs and
passability are application fixtures, not new Sub0HexGrid state. Include no-route,
start=goal and boundary cases; report route validity, optimal reference cost and
declared suboptimality. Replanning and field computation also yield under work budgets.

## Measurement and decision rules

Prepare deterministic fixtures outside timing; all hot-path/frontier/output storage
is reused and capacity-checked. Separate construction, pending scratch, incremental
maintenance, summary computation, traversal, exact filtering, sorting and consumption.
Record whole-cycle cost for the declared query/update/view mix as well as individual
operations. Include resident and peak pending payload, occupied/total nodes, visits,
candidate excess, emitted bytes, cache-qualified behavior and snapshot age. Do not
report a CPU byte-export benchmark as rendering or an epoch median as p99 latency.

Use doctest and nanobench through the existing CPM pins. Controlled comparisons
reserve the host, build arms once, rotate/interleave runs and record thermal/frequency
conditions, seeds, actual SHAs/compiler/options, hardware and raw repetitions.
Per-operation p95/p99/max requires separately specified repeated samples. Include
confidence/noise assessment and worst observed batch duration; workload bounds do
not imply a hard deadline. CPU and external-device trials remain separate, with
transfer/synchronization/render contention included in eventual device evidence.

Use the [trial record](hierarchy-trial-template.md) for every option/use-case: hypothesis, config, exact input
and reference, correctness result, full cost/bytes, raw artifact, uncertainty, tuning
passes, disposition and next question. No measurements exist for these spikes yet.
Use a Pareto table per consumer, not one invented weighted score. Freeze acceptance
budgets from real application requirements before promotion; if unknown, publish
trade-offs and leave selection open. A winning microbenchmark is not a winning game tick.

First working measurements establish a starting point. For a performance-negative
mechanism, investigate measured bottlenecks and take two further bounded implementation
passes before a performance verdict; fix correctness failures before any timing claim.
Record changes and rerun relevant competitors. Park reproducible spikes with configs,
findings and artifacts; default builds need not keep executing parked experiments.
Compare useful combinations only after isolated costs and semantics are understood.

No production representation is chosen in the protocol ADR or first spike round.
Later decisions can choose different motion, view and navigation structures linked by
the same immutable snapshot and fine coordinates. Publish the supporting/regressing
cases, unresolved requirements, fallback and revisit triggers with each decision.
