# Initialization validation

Date: 2026-10-01. H0 architecture baseline: 9a4e604. H1 scalar kernel, example,
header checks, installed consumer and CI are implemented on initial-kernel.

| Environment | Commands | Result |
|---|---|---|
| Windows VS 18 Community, MSVC 19.51 | debug configure/build/ctest | 3/3 passed |
| Same Windows toolchain | release configure/build/ctest | 3/3 passed |
| Ubuntu-24.04 WSL, GCC 15.2 | CXX=g++-15 configure/build sanitize; ASAN_OPTIONS=detect_leaks=1 UBSAN_OPTIONS=halt_on_error=1 ctest | 3/3 passed; no reported sanitizer/leak failure |

Tests: kernel uses an independent breadth-first graph oracle for integer distance,
an independent nearest-center search for geometry, analytic directions/centers,
negative/extreme coordinates, nonfinite inputs, overflow and precision-collapse
rejection. Mapping example is a real executable consumer. Package test installs
to a temporary build prefix, relocates it, configures a separate strict-warning
find_package project and executes it. Three individual translation units compile
each public header as the first/only include.

One initial analytic edge expectation was wrong: half-away-from-zero rounding
selects r=1 at the positive r/s half-edge under our documented repair rule. The
fixture was corrected by deriving the rounded cube tuple, and both signs/sides
now pass. Vertex fixtures check the nearest valid set and stable computed repeat;
they do not promise identical platform choices near floating seams.

Architect plan/code self-review records are in review.md. During code review,
center output was strengthened to reject precision collapse by checking that it
maps back to the requested cell. No heap-backed storage, mutable globals or hidden
borrow exist in the scalar kernel; allocation instrumentation was not run.
Strict kernel/test/consumer warnings are enabled. clang-tidy was configured but
not executed; no independent reviewer, performance benchmark or race-sanitizer
claim is made.

CI adds Linux/Windows Debug/Release plus ASan/UBSan and the relocated consumer.
Record final exact-head CI and merge in the PR. Region enumeration, radius candidate
completeness, finite map indexing, Crucible adoption, height storage/deformation,
sphere subdivision and gameplay remain subsequent gates. License selection is open.
