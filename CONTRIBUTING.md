# Contributing

Develop a small consumed increment from verified main; read AGENTS.md and the
requirements/design first. Review the plan and final C++ with cpp-review, preserving
independent numeric fixtures. Record actual checks and gaps in docs/validation.md.

```sh
cmake --preset debug
cmake --build --preset debug --parallel 4
ctest --preset debug
cmake --preset release
cmake --build --preset release --parallel 4
ctest --preset release
cmake --preset sanitize
cmake --build --preset sanitize --parallel 4
ASAN_OPTIONS=detect_leaks=1 UBSAN_OPTIONS=halt_on_error=1 ctest --preset sanitize
```

Use a VS developer prompt on Windows. The sanitize preset is for GCC/Clang.
To check the installed package, install to build/install, configure tests/consumer
in a separate build with CMAKE_PREFIX_PATH pointing to that absolute install
prefix, build and run CTest there. The regular unit build also registers a relocated
install/consumer check when testing is enabled; it uses the current CMake/toolchain.

Tests use doctest; benchmarks use nanobench. Both are fetched through CPM using
full commits in cmake/DependencyPins.cmake. The checksum-verified CPM bootstrap is
pinned to 0.40.2. Keep dependency sources read-only; use CPM_SOURCE_CACHE for reuse.
Neither development dependency is exported by the installed runtime package.

To build and run the scalar benchmark in Release:

```sh
cmake --preset release -DSUB0HEXGRID_BUILD_BENCHMARKS=ON
cmake --build --preset release --parallel 4
./build/release/benchmarks/sub0hexgrid_benchmarks
```

Setup is outside the timed loops; output is normalized per cell. CI runs a harness
smoke, with no timing pass/fail threshold. See docs/benchmarking.md for evidence rules.
Keep artifacts under build, serialize CPU-heavy checks and claim sustained work.
Publish a PR with outcome, numerical conventions, evidence and limits; exact-head
CI passes before merge. License selection remains an explicit project decision.
