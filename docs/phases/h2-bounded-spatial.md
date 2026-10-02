# Next sprint: H2 bounded spatial foundation

Date: 2026-10-02. Planning baseline: 0953973accb2e33cb052b5272ee8922ab5933b09
(PR 2 merged). Status: defined, not dispatched. This plan authorizes no worker launch
or implementation by itself. Refresh main and active claims at execution start.
This is an outcome-bounded sprint; no calendar estimate or game frame budget is assumed.

## Outcome

Deliver compact finite-region indexing and allocation-free, complete inclusive
world-radius candidate traversal, consumed together by a standalone spatial example
and installed-package test. Supply independent doctest correctness and nanobench
storage/query baselines at meaningful sizes. The APIs should let an application
build contiguous bins and execute independent queries without shared query scratch.

Crucible's spatial Grid is the named eventual receiving module. Its phase-3 proposal
already identifies Sub0HexGrid H2 as a separate upstream increment; no mandatory
Crucible migration is part of that phase. H2's immediate executable caller is a
Sub0HexGrid spatial example using application-owned bins and exact point filtering.
That proves the reusable interaction, not Crucible replay/adoption. H3 retains those gates.

## Reassessed team and exclusive ownership

Use an integrator and at most two implementation workers when execution is requested.
Do not create workers now. Keep T/G component boundaries but consolidate their
maintenance under I; unchanged scalar APIs do not justify independent assignments.

| Owner / stream | Disposition and deliverable | Exclusive paths | Prerequisite / handoff |
|---|---|---|---|
| I: integrator | Retain: contract ADR, build/test separation, standalone spatial caller, package and combined validation | Root CMake, cmake/, presets/CI, central docs, examples/, tests/consumer/, shared fixtures until separated | Owns contract freeze and shared patches; integrates R before Q and combined caller |
| A: R regions | Split: one compact finite-region descriptor with checked membership and coordinate/index bijection | include/sub0hexgrid/regions/, src/regions/, tests/regions/, benchmarks/regions/, docs/workstreams/regions.md | Frozen shape/index ADR and local manifests; hands descriptor/ordering/index contract to Q/I |
| B: Q candidates | Split: independent completeness oracle, then conservative clipped traversal | include/sub0hexgrid/candidates/, src/candidates/, tests/candidates/, benchmarks/candidates/, docs/workstreams/candidates.md | Oracle work can start from G conventions; public implementation waits for R contract freeze |
| T/G | Consolidate with I: preserve topology/geometry and review numerical needs | Existing scalar headers/sources; tests/topology/, tests/geometry/ when separated | Workers request changes rather than editing these shared prerequisites |
| X | Defer backend descriptor/kernels | No implementation paths claimed | Requires stable R/Q and a named external CUDA/Vulkan caller |

Use per-worker branches/worktrees/build trees based on the execution baseline;
record exact bases, owners and resources in ACTIVE_WORK_LOG.md at dispatch. Planned
names are h2-regions and h2-candidates; their creation is not this sprint-plan task.
Workers submit shared CMake/header/ADR patch requests to I. Neither edits Crucible.

## Work packages and sequence

### H2-0 / HX-01: freeze the finite patch and query contracts

I reviews these concrete starting choices with the receiving spatial requirements
and records an accepted ADR before R/Q public code:

- Initial finite shape: an axial q/r rectangle (world-space parallelogram), with
  inclusive int32 coordinate bounds, constant-size metadata and row-major indexing
  (r outer, q inner). This is a proposal to freeze, not a new renderer/world boundary.
  Reject reversed/empty bounds. Derive extents/counts/indexes in checked wide arithmetic;
  full-domain products can overflow uint64 and must fail before multiplication.
- Coordinate values, compact indices and application entity IDs remain distinct.
  Proposed library counts/indices are uint64; host allocation adapters separately
  check size_t/byte limits. A future GPU adapter may narrow only after proving its
  configured region fits its device index width. No universal GPU ABI is promised here.
- The application supplies/clamps its physical world bounds and derives an axial
  covering region for every admitted world point. Freeze that covering construction
  and boundary fixtures in the spatial example. Do not equate axial bounds with
  Crucible's axis-aligned world rectangle or omit edge-mapped points.
- Queries accept finite world center and finite nonnegative radius. Radius zero is
  inclusive. A center outside the region may still overlap it; a valid disjoint disk
  returns an empty traversal. Unrepresentable arithmetic returns explicit failure.
- Candidate traversal is conservative: all in-region cells assigned by the existing
  TryCellAt that can hold a point in the disk are visited. False positives are allowed;
  false negatives, duplicates and silent clipping of valid in-region candidates are not.
- Prefer a lazy range of clipped row intervals, ascending r then q, with no heap,
  retained caller buffers or shared scratch. Freeze value/borrow lifetime, partitioning,
  iterator category, failure and numeric behavior before choosing exact signatures.
  Q must identify the geometry data it needs from G; request the smallest reviewed
  shared access/descriptor change through I rather than bypassing private layout state.
  Bounds validation finishes before traversal begins; do not silently stop on a late error.
- Exact entity filtering, stable IDs, bin storage, output ordering and results are
  application-owned. If materialization is offered, capacity failure must be explicit;
  the first API need not materialize candidates at all.

Assess primary sources and re-derive candidate bounds for our radius/axes/rounding
conventions. The existing numerical ADR/reference guide is the starting point, not
a proof that a lattice disk covers a world-space disk. Finite-precision completeness
needs either outward-conservative bounds with a reasoned error envelope or explicitly
validated numeric limits; no arbitrary epsilon. cpp-review plan mode checks the ADR
and proposed signatures before authoring; cpp-write precedes C++ implementation.

Record representative query radius/cell-radius ratios, occupancy and admitted world
sizes for fixtures. Unknown game time/occupancy budgets remain explicit H3 inputs;
they do not prevent a bounded H2 correctness/baseline deliverable or justify FPS claims.

### H2-1 / HX-02: enable independent development

I separates topology and geometry test registration into local manifests while
preserving the existing independent oracles and 52,736 assertions in aggregate.
Wire R/Q local source/test/benchmark manifests only as real code arrives. Keep public
scalar paths stable, explicit source lists, doctest/nanobench private and CPM pins unchanged.
Add header-alone/installed-consumer coverage for new exports. No placeholder targets.

### H2-2 / HX-03A: deliver regions

R implements the accepted descriptor, membership, count, coordinate-to-index and
index-to-coordinate operations. No cell objects, per-cell neighbor tables or owning
storage are added. Tests independently verify bijection/order, negative bounds,
one-cell/narrow regions, reversed bounds, out-of-region values, final index and
dimension/product limits. Benchmark index/coordinate conversion and contiguous
iteration at 100k, 500k and 1m cells; report metadata separately from caller arrays.

Handoff to Q/I: accepted contract revision, exact commits, real callers/fixtures,
ordering/index guarantees, command results, shared patches and unresolved limits.
Q may implement against the reviewed frozen contract while R is completed, but
combined validation uses the integrated R commit rather than a guessed interface.

### H2-3 / HX-03B: deliver complete candidates

Q first writes an independent tiny-region oracle: exhaustive admitted samples and
exact disk membership, plus analytically derived seam/edge cases. Once R's contract
freezes, implement conservative clipped traversal without graph search or whole-region
enumeration for a small local query. Work should scale with intersected row intervals
and emitted candidates; large queries covering the region necessarily visit its cells.

Test translated/scaled layouts, mixed-sign cells, disks centered off-cell, zero and
large radii, edge/vertex ties and their adjacent representable values, outside centers,
finite-region borders, invalid/nonfinite input and numeric extremes. Verify partition
union equals sequential traversal with no omissions/duplicates. Sampled/random tests
supplement the geometric/numeric derivation; they cannot alone prove completeness.

Handoff to I: derivation/ADR, exact commits and R prerequisite, independent fixtures,
query order/failure/lifetime contract, candidate counts and benchmark limitations.

### H2-4: consume, measure and integrate

I adds a standalone application-owned count/offset/bin example that gathers immutable
point samples, rebuilds pre-sized bins, traverses candidates and performs exact inclusive
distance filtering. Return/consume snapshot row indices; stable ID policy is optional
example data, never a library dependency. Query outputs/scratch are caller-owned.
Compare exact results with brute-force scanning on tiny worlds and adversarial cases.
The installed-package consumer exercises both new facets without source-tree paths.

Benchmark deterministic uniform, clustered and border-heavy point populations at
100k/500k/1m entities, recording cell count and occupancy separately. Use 4,096 query
centers per standard measurement rather than an unbounded all-pairs test; include
small and larger radius/cell ratios, and a separately bounded coincident stress case.
Measure mapping, bin rebuild, candidate traversal, exact filtering and combined query
time separately; document whether result consumption is included. Fixtures are prepared
outside timed loops and all measured hot-path buffers are reused. Record resident and
peak scratch bytes, bytes/cell/entity, candidates/query, true hits and candidate excess;
for zero true hits report counts rather than an undefined amplification ratio.

Nanobench supplies repeated operation/batch timings. Per-query p95/p99 requires a
separately described sampling protocol; do not label epoch medians as latency percentiles.
Run controlled measurements only on a reserved uncontended host. CI verifies harness
execution, not timing thresholds. This establishes H2 costs, not a speedup over
Crucible, full-tick throughput or 60 FPS. Missing benchmark hardware is reported as
an unmet evidence gate rather than fabricated results.

## Sprint exit

- R/Q are used together by the standalone caller and relocated installed consumer;
  header-alone builds pass and runtime-only configuration still fetches no test/bench deps.
- Independent doctest fixtures and numerical derivation establish complete inclusive
  candidates, checked bijection, capacity/border behavior and sequential/partition parity.
- Scalar regression assertions remain 52,736; new cases are counted separately.
  No allocation in R/Q hot paths; verify with scoped allocation instrumentation that
  excludes doctest/nanobench/setup allocation, plus code review.
- Run unfiltered Debug/Release, supported ASan/UBSan and strict package checks on
  Linux/Windows; cpp-review issues are resolved before integration. Runtime workers
  are not introduced, so no thread-safety evidence is implied by partitionable ranges.
- Controlled nanobench/storage evidence is recorded with exact build/hardware/workload
  and limitations. Do not set an invented throughput target before a baseline exists.
- Publish the consumed increment, verify exact-head CI, merge, record baseline and
  phase review/follow-up IDs, release claims and preserve useful artifacts.

## Deferred and bounded stretch

Core completion takes precedence. No owning generic grid, sparse streaming/chunk
directory, terrain/height, pathfinding, spherical cells, native GPU code, scheduler,
Crucible bin replacement or Blight adjacency change. HX-04 remains deferred.
Bulk mapping is the only stretch, and only if the measured standalone caller shows
it matters, the API is independently reviewed and all core gates already pass.
Otherwise carry it to H4 without speculative signatures or flags.

At close, reassess whether H3 Crucible adoption or a measured H2 refinement is the
next useful sprint. H3 requires explicit world-boundary/replay agreement and real
application evidence; this plan does not declare that migration complete.

## Planning review

Plan self-review checked the named standalone caller for R/Q, acyclic dependencies,
value/output ownership, exclusive paths, staged contract handoff and acceptance scope.
Shape/index choices remain proposed until the H2-0 ADR is accepted; no exact public
signatures or API scaffolding are introduced by this planning increment. No blocking
planning finding remains. Implementation requires its own plan/code reviews and evidence.
