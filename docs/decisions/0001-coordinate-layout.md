# Decision 0001: axial values and one pointy-top layout

Date: 2026-10-01. Accepted for the first library increment, not Crucible migration.

Use signed 32-bit q/r values and derive s = -q-r in wide arithmetic. Six steps use
axial deltas (1,0), (1,-1), (0,-1), (-1,0), (-1,1), (0,1). Compute distance from
the maximum absolute cube-coordinate difference, with uint64 result.

Choose one pointy-top regular layout, positive double circumradius R, finite origin
O and positive y aligned with increasing r. Center mapping uses
x = O.x + R*sqrt(3)*(q+r/2), y = O.y + R*3*r/2.
Inverse coordinates use normalized x/y:
q = x/sqrt(3)-y/3, r = 2*y/3, s = -q-r.

These conventions were checked against the author's
[hex guide](https://www.redblobgames.com/grids/hexagons/) and
[implementation](https://www.redblobgames.com/grids/hexagons/implementation.html),
retrieved 2026-10-01. The reference uses scaled pointy matrices and cube coordinates;
our uniform R is its equal x/y size, origin is subtracted before normalization,
and only q/r are stored. Mathematical formulas are reimplemented, not copied source.

Round each fractional cube component half away from zero, then repair the component
with the largest rounding residual. On equal computed residuals, our explicit
priority is q, then r, then s. This is a deliberate tie-policy difference from the
reference's strict-comparison branch order. check finite intermediates and corrected
q/r representability before integer casts. Center conversion also rejects precision collapse when its result does not map back to the requested cell. No epsilon bias or cross-platform seam
identity is promised. Fixtures pin analytic edge/vertex choices where representable.

Invalid factories/conversions return nullopt. Extremely large finite layout/point
values may produce an unrepresentable result, which is rejected. No map clipping,
hash/storage address, dual orientation, ECS dependency or allocation is added.
Later candidate traversal must be designed independently, not inferred from six steps.
