# Measurement scope

Use nanobench fetched through CPM, separately from doctest correctness tests.
The opt-in sub0hexgrid_benchmarks executable measures four scalar operations over
4096 mixed-sign inputs with setup excluded and per-cell normalization. Inputs vary
within each batch; results are protected from dead-code elimination. These are
warm scalar baselines, not large-world throughput, allocation instrumentation or FPS.

Run Release on an uncontended host, record exact commit/config/compiler/hardware,
input distribution, epochs, working-set size, residency and all reported instability.
Compare rotated/interleaved arms and repeat only as needed to resolve uncertainty.
Do not use CI shared-runner timing as an acceptance threshold; CI smoke proves the
harness executes. Check resource claims before local runs and never infer claim expiry.

H2 requires resident/peak scratch bytes, rebuild/query time, candidate amplification,
occupancy distribution and p95/p99 consumer latency at 100k/500k/1m entities where
the real caller supports them. Include empty, clustered, coincident, border-heavy
and large-radius cases. Record cell count separately from entity count. Complete
neighborhoods may have quadratic output; document application approximation limits.

External backend evidence includes transfers, dispatch, barriers, readback and
rendering contention, plus CPU/backend correctness under the declared seam policy.
Large-grid/API choices are accepted on consumer-inclusive evidence, not scalar timings.
