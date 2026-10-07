# Sub0 family style audit

## Style audit: Sub0HexGrid, baseline `9791beeb98c3`, 2026-10-07 (run 1)

Style profile: `sub0` (declared in `STYLE_GUIDE.md`). Overlay: none.
Configuration: `LIB=sub0hexgrid`, `LIBU=SUB0HEXGRID`, `CMAKE_LIB=Sub0HexGrid`,
`SCOPE=include src tests examples benchmarks`, `PUBLIC=include`.
Coverage: 33 C++ files scanned mechanically; all 9 public headers read and
reviewed. The audit covered interface documentation, API names, file/include
conventions, ownership members, umbrellas, CMake options, iterator protocols and
open family decisions.

The issue explicitly defers control-flow brace placement, enum/constant prefixes,
namespace naming, template spacing and include order. Those remain unchanged where
applicable. The installed profile labels S17 include ordering as decided, while the
issue marks it open; this audit follows the issue and records the current spread.

### Tally

| Rule | Status | Tier | Hits / exceptions | Files | Kind | Examples |
|---|---|---|---:|---:|---|---|
| S01 method-case | decided | STYLE/SHOULD | 0 | 0 | api-breaking, fixed | Previous public names listed below |
| S02 doxygen-interfaces | decided | STYLE/SHOULD | 0 | 0 | judgement, fixed | All 9 public headers reviewed |
| S03 autobrief | decided | STYLE/SHOULD | 0 | 0 | mechanical | No `@brief` tags remain |
| S04 include-style | decided | STYLE/SHOULD | 0 | 0 | mechanical | All library includes are quoted and rooted |
| S05 pragma-once | decided | STYLE/SHOULD | 0 | 0 | mechanical | All 9 public headers use `#pragma once` |
| S06 file-names | decided | STYLE/SHOULD | 0 | 0 | api-breaking, fixed | Public headers and implementations now snake_case |
| S07 one-primary-type | decided | STYLE/SHOULD | 0 | 0 | judgement | `Axial` and `Direction` remain a coupled coordinate contract |
| S08 folders-and-umbrella | decided | STYLE/NICE | 0 | 0 | mechanical | Both concept folders and the library root have umbrellas |
| S09 test-names | decided | STYLE/NICE | 0 | 0 | mechanical | `tests/main.cpp` is the shared doctest entry point |
| S10 layout | mixed | STYLE/NICE / QUESTION | 65 open choices | 11 | judgement, partly deferred | All control-flow braces remain on their lines |
| S11 type-case | decided | STYLE/SHOULD | 0 | 0 | mechanical | Project types use PascalCase |
| S12 member-names | decided | STYLE/SHOULD | 0 violations; 4 exceptions | 2 | judgement | `Axial::{q,r}`, `Point::{x,y}` |
| S13 interface-prefix | decided | STYLE/SHOULD | 0 | 0 | mechanical | No virtual interfaces |
| S14 macro-and-cmake-prefix | decided | STYLE/SHOULD | 0 violations; 1 exception | 1 | mechanical | `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` |
| S15 cmake-alias | decided | STYLE/SHOULD | 0 | 0 | mechanical | One `Sub0HexGrid::Sub0HexGrid` alias |
| S16 nodiscard | provisional | STYLE/CANDIDATE | 0 candidates | 0 | judgement | All result-bearing pure queries/statuses are checked |
| S17 include-order | deferred by issue | STYLE/QUESTION | 8 files | 8 | judgement | Source-first includes retained per issue direction |

The S12 exceptions preserve conventional coordinate field names on public value
types: q/r identify axial axes and x/y identify Cartesian components. All other
project data members use trailing underscores. The S14 macro is a doctest
configuration hook required before its third-party header.

The 65 control-flow brace sites remain same-line, with no own-line control-flow
braces. All namespace, type and function opening braces use the decided Allman
form. `.clang-format` now encodes those declaration rules, keeps the existing
control-flow placement, and disables include sorting.

### Files per rule with exceptions or deferred items

- S07: `include/sub0hexgrid/axial.hpp` groups `Axial` and its `Direction` enum.
- S09: `tests/main.cpp` supplies the shared doctest main and is not a test case.
- S10: 65 control-flow sites in `benchmarks/kernel.cpp`,
  `benchmarks/spatial.cpp`, `examples/mapping.cpp`, `examples/spatial.cpp`,
  `examples/spatial/SpatialIndex.hpp`, `tests/candidates/test_candidates.cpp`,
  `tests/geometry/test_geometry.cpp`, `tests/regions/test_regions.cpp`,
  `tests/spatial/test_spatial.cpp`, `tests/test_allocation.cpp` and
  `tests/topology/test_topology.cpp`; their placement is deferred by the issue.
- S12: `include/sub0hexgrid/axial.hpp` (`q`, `r`) and
  `include/sub0hexgrid/point.hpp` (`x`, `y`).
- S14: `tests/main.cpp` (`DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN`).
- S17: `examples/spatial.cpp`, `src/axial.cpp`,
  `src/candidates/candidate_cells.cpp`, `src/candidates/candidate_cursor.cpp`,
  `src/pointy_layout.cpp`, `src/regions/axial_region.cpp`,
  `tests/candidates/test_candidates.cpp`, and `tests/regions/test_regions.cpp`.

### Exempt, verify

- `begin` and `end` implement the standard iterator protocol.
- `operator*`, `operator++` and `operator==` retain conventional operator names.
- `value_type`, `difference_type`, `reference`, `iterator_concept` and
  `iterator_category` are standard iterator associated types.
- `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` is a third-party configuration macro.

### Questions and open family items

- Control-flow braces: current spread is 65 same-line and 0 own-line. The issue
  defers the choice; should the family select one form in a later cross-library
  review?
- Constant and enumerator prefixes: no `k`/`c` prefixed names occur here; the three
  enums use lowercase unprefixed values. Should a family rule be selected later?
- Namespace scheme: `sub0hexgrid` and `sub0hexgrid::example` are used. Should the
  family retain library-specific roots or move to nested `sub0::<library>` names?
- Template spacing: no project template declarations were found. Which form should
  be used if a template is introduced?
- Include order: 8 files place their own/helper header before a system header. The
  issue defers changes; should family ordering become a rule after the call sites
  and standalone-header checks are reviewed?
- Error handling: this C++23 project uses `noexcept` operations and one example
  `std::length_error` path; it has no `std::expected` API. Should the family retain
  per-library error policies?
- Language level: the project selects C++23. Should the family permit library-level
  variation?
- Test dependencies: doctest and nanobench are fetched with CPM. Should the family
  standardize on CPM, vendoring, or permit both?
- Multi-line Doxygen form: `pointy_layout.hpp` has one adjacent two-line `///` class
  comment. Should the family require `/** */` for multi-line comments?
- Formatter configuration: `.clang-format` and `.clang-tidy` exist. The formatter
  now matches decided declaration braces; should formatting remain configured per
  repository?
- Internal helper includes: four source/test/benchmark files include the example
  helper by a relative path. They are not library headers. Should the example move
  to a shared internal include root, or remain local to the example tree?

### Breaking API and path migration

Public function/method names changed to camelCase:

| Old name | New name |
|---|---|
| `TryNeighbor` | `tryNeighbor` |
| `ComputeDistance` | `computeDistance` |
| `TryCreate` | `tryCreate` |
| `TryCellCenter` | `tryCellCenter` |
| `TryCellAt` | `tryCellAt` |
| `GetRadius` | `getRadius` |
| `GetOrigin` | `getOrigin` |
| `GetMinimum` | `getMinimum` |
| `GetMaximum` | `getMaximum` |
| `GetCellCount` | `getCellCount` |
| `Contains` | `contains` |
| `TryIndex` | `tryIndex` |
| `TryCell` | `tryCell` |
| `Read` | `read` |
| `Remaining` | `remaining` |
| `IsDone` | `isDone` |
| `TrySlice` | `trySlice` |

Public header paths changed:

| Old path | New path |
|---|---|
| `sub0hexgrid/Axial.hpp` | `sub0hexgrid/axial.hpp` |
| `sub0hexgrid/Point.hpp` | `sub0hexgrid/point.hpp` |
| `sub0hexgrid/PointyLayout.hpp` | `sub0hexgrid/pointy_layout.hpp` |
| `sub0hexgrid/regions/AxialRegion.hpp` | `sub0hexgrid/regions/axial_region.hpp` |
| `sub0hexgrid/candidates/CandidateCells.hpp` | `sub0hexgrid/candidates/candidate_cells.hpp` |
| `sub0hexgrid/candidates/CandidateCursor.hpp` | `sub0hexgrid/candidates/candidate_cursor.hpp` |

Crucible currently has 24 references to the renamed methods in
`src/spatial/Grid.cpp`, `include/crucible/spatial/Grid.hpp`,
`tests/spatial/Grid.cpp` and `tests/spatial/GridHexTests.cpp`, plus three old header
includes in `src/spatial/Grid.cpp` and `include/crucible/spatial/Grid.hpp`. Crucible
remains pinned to the existing API until its consumer migration is reviewed.

### Verification

MSVC 19.51 Debug and Release both built and passed all 9 CTest entries, including
the standalone public-header checks and relocated package consumer. In both
configurations, doctest counts match the recorded baseline exactly: topology
47,765; geometry 4,971; regions 4,441; candidates 29,836; spatial 25,106.
The original scalar total remains 52,736 (topology plus geometry). Allocation
instrumentation, examples and package checks also passed in both configurations.
GitHub exact-head workflow [37616561347](https://github.com/CraigHutchinson/Sub0HexGrid/actions/runs/37616561347)
passed Linux and Windows Debug/Release plus sanitizer jobs. PR #8 merged at
`3994e900cdb609f4b497efc7d6819616e3aeb888` on 2026-10-07.

### Machine-readable tally

```tsv
rule	hits	files
S01	0	0
S02	0	0
S03	0	0
S04	0	0
S05	0	0
S06	0	0
S07	0	0
S08	0	0
S09	0	0
S10	65-deferred-control-flow	11
S11	0	0
S12	0-violations;4-exceptions	2
S13	0	0
S14	0-violations;1-exception	1
S15	0	0
S16	0-candidates	0
S17	8-deferred	8
```
