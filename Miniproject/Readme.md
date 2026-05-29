# SEIHR peak hospitalization

Produced by `examples/seihr.cpp` (`mean_peak_hospitilzations(...)`), simulating up to
time `t = 100` with seed `42`. The single-run column drives one trajectory per
population; the 100-run column averages the per-run peak H count over 100 independent
simulations dispatched in parallel via `parallel_runs`.

| Population                 | Single run (peak H) | Mean over 100 runs (peak H) |
|----------------------------|--------------------:|----------------------------:|
| 10.000                     |                   2 |                        3.54 |
| 592.800 (Northern Jutland) |                 119 |                      129.14 |
| 6.023.000 (Denmark)        |               1 200 |                    1 232.79 |

---

# Benchmark results

Measures the cost of running **100 SEIHR simulations** under two configurations
at two population sizes, with 5 repetitions per cell.

- `bm_sequential`: 100 runs on a single thread, no thread pool.
- `bm_parallel/T`: same 100 runs dispatched across `T` worker threads via `parallel_runs`.

## Hardware

- 8 X 24 MHz CPUs (macOS, `std::thread::hardware_concurrency() == 8`).

## Results (population N = 100.000)

| Benchmark           | Mean (ms) | Stddev (ms) |     CV | Speedup vs `parallel/1` |
| ------------------- | --------: | ----------: | -----: | ----------------------: |
| `bm_sequential`     |     6 563 |         7.5 |  0.11% |                       — |
| `bm_parallel/1`     |     6 830 |       213.0 |  3.12% |                   1.00× |
| `bm_parallel/2`     |     3 500 |        30.7 |  0.88% |                   1.95× |
| `bm_parallel/4`     |     1 891 |        83.8 |  4.43% |                   3.61× |
| `bm_parallel/8`     |     1 329 |        96.5 |  7.26% |                   5.14× |
| `bm_parallel/16`    |     1 338 |       316.0 | 23.66% |                   5.10× |
| `bm_parallel/32`    |     1 170 |       120.0 | 10.27% |                   5.84× |
| `bm_parallel/100`   |     1 181 |        55.7 |  4.71% |                   5.78× |

## Results (population N = 10.000)

| Benchmark           | Mean (ms) | Stddev (ms) |     CV | Speedup vs `parallel/1` |
| ------------------- | --------: | ----------: | -----: | ----------------------: |
| `bm_sequential`     |       651 |         3.0 |  0.46% |                       — |
| `bm_parallel/1`     |       653 |         2.8 |  0.44% |                   1.00× |
| `bm_parallel/2`     |       340 |         2.1 |  0.62% |                   1.92× |
| `bm_parallel/4`     |       183 |         1.7 |  0.94% |                   3.57× |
| `bm_parallel/8`     |       118 |         1.0 |  0.88% |                   5.53× |
| `bm_parallel/16`    |       115 |         1.8 |  1.54% |                   5.68× |
| `bm_parallel/32`    |       115 |         2.4 |  2.10% |                   5.68× |
| `bm_parallel/100`   |       117 |         3.4 |  2.85% |                   5.58× |

## Conclusions

1. **Measurements are stable.** With 5 repetitions per cell, most CVs are below 5 %.

2. **The parallel implementation matches the sequential baseline.** `bm_parallel/1`
   is within noise of `bm_sequential` at N = 10.000 (653 vs 651 ms, ~0.3 %). At
   N = 100.000 there is a small ~4 % overhead from the pool (6 830 vs 6 563 ms),
   submitting 100 tasks through a mutex-protected queue is not free

3. **Scaling is near-linear up to the physical core count.** From 2 to 4 to 8 threads
   I get 1.92×, 3.57×, 5.53× at N = 10.000 and 1.95×, 3.61×, 5.14× at N = 100.000.
   About 69 % and 64 % of the ideal linear line at 8 threads

4. **Over-subscription is not free, but on this workload it does not punish either.**
   - At N = 10.000, going past 8 threads stalls at ~115 ms: no further speedup
     (no more cores) and no measurable cost.
   - At N = 100.000, `parallel/32` is actually slightly *faster* than `parallel/8`
     (median 1 129 vs 1 303 ms).
5. **Population scales the work, not the shape of the conclusion.** N = 100.000 is
   ~10× longer than N = 10.000. The speedup curve has the same shape at both
   sizes. The workload is CPU-bound and embarrassingly parallel either way.
