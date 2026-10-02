# X — External accelerator interoperability

Proposed consumer-gated component, not a native GPU backend. Own future interop
headers/fixtures and this brief. Depend on frozen G/R/Q contracts. First identify a
real external CUDA or Vulkan compute caller and inspect its hardware/API capabilities.

Define explicit fixed-width descriptors, buffer strides/alignment, validity, index
limits and CPU/backend precision/seam equivalence. Validate C++ and shader layouts;
do not upload optional/private-object bytes. Backend kernels, device allocation,
dispatch, barriers, queues and completion ownership stay in the receiving project.

Fixtures compare integer results exactly and geometry/candidates under the declared
policy. Capacity failures and partition boundaries must preserve completeness.
Backend measurements include transfer/synchronization and rendering contention;
nanobench CPU measurements alone cannot establish GPU speedup. Handoff descriptor
revision, actual caller/pins/capabilities, parity evidence and outstanding platform gates.
