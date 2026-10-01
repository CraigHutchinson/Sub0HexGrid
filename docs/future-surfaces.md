# Future surface geometry and height

User direction, 2026-10-01. Preserve as a future requirement, not an initial API.

The receiving application views a local patch of a larger world. Its likely first
terrain extension is a planar surface with variable height, movement cost, resource
mining that creates depressions and permanent fused terrain such as bridges.
Sub0HexGrid supplies topology/geometry; application-owned height samples, resource
balances, erosion rules, support/connectivity and structural commits remain separate.

A later world may use a hexasphere-like or another near-uniform spherical subdivision
with height and terrain dynamics. A global sphere needs a separately specified
identity/adjacency model, seams/exceptional cells, metric, world projection and height
direction; the infinite axial plane and uniform six-direction API are not a universal
sphere contract. Do not wrap the planar layout in a speculative virtual Surface class.

Before a spherical facet: inspect primary geometric sources, choose the subdivision
and metric, demonstrate a receiving caller, specify capacity/numeric/ordering contracts,
then verify adjacency/manifold/area or distortion properties appropriate to that choice.
Before any height helper: demonstrate an independent product-neutral consumer and
define units, sample ownership/interpolation and finite bounds. These are distinct facets.

Initial scalar coordinates are surface-local. A later patch/world identity can contain
a planar cell or other surface coordinate without silently changing Axial semantics.
Region/candidate traversal and Crucible's planar adoption come before global spherical
code unless an actual requirement changes the phase plan.

No spherical mesh, height storage, terrain material conversion or game dynamic is
implemented here. Preserve the extension in phase reviews and the reuse catalog.
