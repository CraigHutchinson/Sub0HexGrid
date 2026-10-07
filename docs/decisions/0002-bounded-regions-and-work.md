# Decision 0002: bounded regions and resumable work

Accepted for H2, 2026-10-02, execution baseline d24d08a. Current consumers are the
spatial example, package consumer and workload benchmark; Crucible adoption is H3.

AxialRegion stores inclusive minimum/maximum Axial bounds and a checked uint64 cell
count. Row-major indexing uses r outer/q inner. Reversed bounds and overflowing
products fail; index/cell operations return optional on out-of-domain inputs.
It owns metadata only. Accessors expose bounds/count for algorithms and adapters.

CandidateCells::tryCreate(layout, region, center, radius) constructs conservative
clipped axial rectangle candidates, not exact disk/cell intersection. Bounding-box
false positives are accepted for the first implementation; no valid mapped point
inside the inclusive disk may be omitted. Invalid/nonfinite/unsupported arithmetic
fails before iteration. Empty intersections are valid empty ranges. Candidates are
in ascending r/q order; all metadata is owned, no buffers/layout borrows retained.
Const ranges can be used independently; modifying one cursor needs exclusive access.

CandidateCells is a forward range and supports checked index/count slicing, including
empty end slices. CandidateCursor owns a range and fills caller-owned span<Axial>
via read, reporting the written prefix; empty buffers make no progress. remaining
and isDone distinguish completion from yielding. Copying a cursor snapshots progress.
No clocks, callbacks, workers or heap allocation enter this facet. Each cell read
is constant work; output buffer size bounds work. Application time budgeting checks
a deadline between batches, and exact filtering must separately budget dense-bin
entity examinations. No hard real-time deadline is promised.

## Geometry source and completeness

The author's [implementation](https://www.redblobgames.com/grids/hexagons/implementation.html)
and [guide](https://www.redblobgames.com/grids/hexagons/) were retrieved 2026-10-02.
Our existing convention is q=x/sqrt(3)-y/3, r=y*(2/3), s=-q-r after origin/radius
normalization; cube repair and computed-double ties remain Decision 0001's contract.
We independently bound this actual algorithm, not copy reference source.

The world disk is contained in its axis-aligned square center +/- radius. Interval
endpoints use nextafter outward after each arithmetic operation, enclosing the
existing double normalization and q/r computations for every point in that square.
Reject nonfinite endpoints/intermediates and intervals outside |q|,|r| <= 2^40.
This supported domain exceeds int32 coordinate magnitude; extremely large query
extents can be rejected even when clipping would leave a representable region.

Within that domain, fl(q+r) has absolute error at most 2^-12 (binary64, round-to-nearest);
individual round residuals are at most 1/2. Repair of q or r therefore differs from
the unrounded component by at most 1+2^-12. Rounded/repaired integer-valued operands
remain below 2^53, so integer repair addition is exact. Pad outward fractional q/r
bounds by two cells (floor(lower)-2, ceil(upper)+2), then clip against region bounds
before narrowing. This is a derived conservative rounding envelope, not a guessed
epsilon. IEEE binary64 round-to-nearest without fast-math is required for this
completeness contract; independent tests cover finite boundaries and tiny/huge scales.

The first candidate rectangle may include more cells than a row-tight disk envelope.
measure amplification before refinement. It remains local O(candidates) with constant
metadata and O(1) slicing/creation, and large regions can be streamed without allocation.
G exposes immutable radius/origin observations solely for this real Q consumer.

## Plan review

R/Q each have named executable/package consumers. T -> R; T/G/R -> Q is acyclic.
Region/range/cursor own only scalar metadata; caller buffers belong to the application.
No speculative backend, cell storage or clock surface. Forward range composition and
explicit resumable read are both consumed. Bounds fail up front; no hidden partial
failure during iteration. Integrator plan self-review found no blocking L0/L1/L2 issue;
code review and independent numerical/allocation evidence remain separate gates.
