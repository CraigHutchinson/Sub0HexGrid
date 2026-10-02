# Hierarchy trial record template

Copy into the experiment's findings artifact and link it from the phase close.
Status starts as planned; never fill timings or correctness from assumptions.

| Field | Record |
|---|---|
| Trial / phase / owner | HS/HV/HN ID, tuning pass, exclusive paths |
| Status / question | Planned, correctness failed, measured, iterate, parked or promoted; falsifiable hypothesis |
| Consumer / semantics | Motion, mini-map, zoom, navigation or other; exact vs visual/route-quality allowance |
| Source / input identity | Commit/tree hash, primary algorithm reference, input/trace hash, seed, actual configuration |
| Reference / comparison arms | Same fine data, predicate, output and update/refresh policy; differences explicit |
| Correctness / quality | Oracle result, assertions, seams, conservation, route validity/cost, visual error/transition evidence |
| Budgets / lifetimes | Application target or unknown; batch units/frontier capacity, epoch, publication and cancellation |
| Platform / reservation | Hardware, OS/compiler/options, device/backend, uncontended reservation, thermal/frequency/cache conditions |
| Protocol / repetitions | Rotation, setup exclusion, timed scope, repeated latency samples, noise/uncertainty |
| Full cycle | Gather + build/update + summaries + queries + order/output + publication; declared workload mix |
| Query / view / navigation | Per-operation distribution, p95/p99/max where sampled; nodes/leaves/outputs, screen/route quality |
| Memory / transfers | Resident/pending/frontier payload, total/occupied nodes, emitted/upload/readback bytes; RSS only if measured |
| Bounded processing | Maximum observed step time/work, zero budget, dense leaf resume, failure/cancel/overflow |
| Raw evidence / review | Artifact paths, exact commands, code/numeric review, supported and regressing cases |
| Disposition / follow-up | Bottleneck, next tuning pass, why parked/promoted, remaining uncertainty and revisit trigger |

Architecture close compares records per use case. A preliminary win does not promote
a production API; a failed initial timing does not settle a mechanism's value.
