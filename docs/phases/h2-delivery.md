# H2 delivery: bounded spatial foundation

Date: 2026-10-02. Base: d24d08ae5577ab0cec03f0b0524bebf5944a5174.
Implementation/measured source: 96d8b15b02f784a17048be7fc2f6bb97d807fae0;
[PR 4](https://github.com/CraigHutchinson/Sub0HexGrid/pull/4) records final exact-head CI.
Execution authorized by the user, including iterative/time/workload batching.
PR and exact-head CI are the final cross-platform delivery record.

## Delivered interaction

AxialRegion provides checked inclusive q/r rectangle membership and uint64 indexing
without cell storage. CandidateCells provides conservative inclusive disk candidates,
owned forward iteration and O(1) checked slices. CandidateCursor fills reusable output
spans with retained progress. PointyLayout exposes its validated radius/origin to Q.
[Decision 0002](../decisions/0002-bounded-regions-and-work.md) freezes ordering,
ownership, failure and the outward interval/cube-rounding completeness derivation.

The application-only SpatialIndex example preallocates current/pending bins, then
resumes reset, mapping/count, prefix and scatter under work budgets. Successful
publication swaps buffers once; failure/cancel preserves committed data. Queries
yield within dense bins and count both cell visits and entity examinations. Output
storage belongs to callers. Deadlines are checked between bounded batches, without
clock/scheduler/backend dependencies in the library. Index lifetime and publication
invalidation are explicit; this example does not allow concurrent mutation.

Independent R and Q workers owned disjoint paths. Contract freeze preceded Q;
the integrator owned shared wiring and caller. Workers cross-reviewed R/Q and the
caller read-only. This execution used the shared integrator tree with exclusive
path claims rather than separate worktrees; neither worker built or committed.
Future phases should restore separate worktrees for independent Git/build ownership.

The original plan preferred tighter row intervals; H2 instead implements a
conservative rectangle with O(1) validation/count/slicing and a proved numerical
envelope. It emits false positives, not an approximation of exact entity results.
This deviation is measured and motivates HX-05. Numeric support is explicitly
bounded to normalized q/r intervals within +/-2^40 under binary64 round-to-nearest;
invalid arithmetic rejects before traversal, even if clipping could hide it.

## Validation and review

MSVC 14.51.36231 / VS 18.7: unfiltered Debug and Release pass all nine CTest checks,
including relocated installed consumers, six header-alone builds and manual scoped
C++ allocation interception (zero allocations in repeated kernel/build/query work).
Runtime-only configuration/build fetches neither doctest nor nanobench. CPM pins are
unchanged, dependencies private, one static exported target remains.

Topology: 47,765 assertions; geometry: 4,971, preserving 52,736 scalar assertions.
New regions: 4,441; candidates: 29,836; spatial caller: 25,106 assertions.
Region fixtures enumerate independent bijections/extremes. Candidate fixtures test
seams/adjacent representable values, extreme scales, partitions, owned iterators and
cursor copies. Spatial tests compare exact results against brute force for multiple
budgets, empty output, dense bins, failure/cancel and publication invalidation.

cpp-review plan self-review accepted the ADR before implementation. Independent
code review found no blocking numerical/lifetime/bounded-work findings. Resolved
SHOULD findings: constexpr accessors, accurate capacity-payload accounting, explicit
empty-output query fixture, empty/coincident/large-radius benchmark and occupancy
stats. Benchmark self-review checked timed/setup separation and honest scope.
Linux ASan/UBSan and Windows/Linux Debug/Release passed exact-head PR CI at 96d8b15.
The final documentation head is also checked in PR 4 before merge.

## Controlled baseline

The user confirmed the host free for H2 measurements. One sequential Release run
used Intel Core Ultra 9 275HX (24 cores/logical processors), Windows build 26220,
MSVC /O2 /Ob2 /DNDEBUG, without fast-math, affinity/frequency locking or GPU work.
Three nanobench epochs, minimum one iteration; all instability warnings are retained
in [raw output](../benchmarks/h2-msvc.txt). This is a baseline, not a comparison or
promotion decision; unstable rows are provisional. Setup and allocations are outside
timed loops. 4,096 deterministic centers per standard case; separate stress/empty/
large cases use 64 centers. query results are consumed as reusable row prefixes.

Each query's chrono duration is sampled once after rebuilding resident bins, before
nanobench repetitions, then sorted for nearest-rank p95/p99. Timer overhead is
included; small sub-microsecond figures are coarse. Warm here means resident
prebuilt storage, not guaranteed CPU cache residency. No independent latency tail
distribution across repeated runs or parallel game load is claimed.

Mapping, bounded rebuild, exact distance predicate, region index roundtrip,
candidate traversal and combined exact queries are timed separately. Predicate
microbench is a scan of fixture points, not a subtraction-based estimate of query
filtering time. Occupancy reports nonempty cells and maximum; full deterministic
distribution follows the generator. Candidate cells, examined entities, hits and
rejections are reported separately. Counts remain meaningful when hits are zero.

On this 64-bit host, region metadata is 32 bytes. The example's required vector
payload is 56*N + 32*C + 16 bytes, including both snapshots and rebuild scratch;
reported resident capacities match it here. At N=C=1m it is 88,000,016 bytes plus
16m caller input bytes, vector objects and allocator overhead. Pending arrays account
for 32*N + 24*C + 8 bytes of that payload; they are reused and do not grow during
processing. The benchmark additionally owns points, occupancy, centers and latency
fixtures; process RSS/OS peak memory was not measured. These byte accounting
figures must not be reported as OS residency measurements.

Representative combined nanobench medians at N=C=1m:

| Population | Radius 2 | Radius 8 | Sampled p99 at radius 8 |
|---|---:|---:|---:|
| Uniform (one per cell) | 3.87 us | 11.28 us | 16.9 us |
| Clustered (4,096 occupied, max 245) | 119.44 us, unstable | 462.37 us | 828.5 us |
| Border (2,000 occupied, max 500) | 21.16 us | 40.26 us | 78.3 us |

At 1m uniform radius 2, 330,298 cells/entities are examined for 24,550 hits across
4,096 queries: roughly 13.45 examinations per hit. Coincident 1m entities require
1m examinations/hits per query and roughly 3.4 ms at radius 2. Large radius 1000
over 100k cells/1m entities costs roughly 6.64 ms/query. Batching yields safely but
cannot avoid complete-output cost. No FPS, speedup, CPU/GPU parity or parallel
scaling claim follows. Native accelerator work and Crucible migration remain deferred.

## Next sprint and handoff

Subsequent user direction adds [hierarchy evaluation](hierarchy-research.md) as the
next research/review/architecture phase across HexGrid, Crucible and Sub0ECS.
Its initial prior-art/source and receiving-contract audit are recorded separately.
The following H2 refinements remain complementary follow-ups rather than displacing
that requested research axis.

HX-05 / Q + I: tighter conservative bounds. First preserve the independent oracle,
seam proof, slicing and cursor semantics; measure empty/small/clustered/border cases
against this baseline with rotated arms. Select row intervals or incremental iteration
only when they improve consumer cost without allocating or weakening completeness.
No candidate reduction target substitutes for exact result parity.

HX-06 / I: add stable repeated latency sampling and controlled comparison protocol
before accepting a performance improvement; qualify cache/thermal drift and tails.
HX-07 / application owner: agree Crucible physical world bounds, entity snapshot,
query/replay fixtures and frame workloads before H3 adoption. X remains deferred
until a named CUDA/Vulkan caller justifies a fixed-width descriptor/parity contract.
T/G remain consolidated maintenance; no automatic new dispatch is implied.
