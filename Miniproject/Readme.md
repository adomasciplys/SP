# Benchmark results

Measures the cost of running **100 SEIHR simulations** (population N = 100.000, end_time = 100.0) under two configurations:

- `bm_sequential`: 100 runs on a single thread, no thread pool.
- `bm_parallel/T`: same 100 runs dispatched across `T` worker threads via `parallel_runs`.

## Hardware

- 8-core CPU (macOS, `std::thread::hardware_concurrency() == 8`).

## Results

| Benchmark            | Time (ms) | Speedup vs `parallel/1` |
| -------------------- | --------: | ----------------------: |
| `bm_sequential`      |     6 618 |                   1.00× |
| `bm_parallel/1`      |     6 654 |                   1.00× |
| `bm_parallel/2`      |     3 473 |                   1.92× |
| `bm_parallel/4`      |     1 809 |                   3.68× |
| `bm_parallel/8`      |     1 178 |                   5.65× |

## Conclusions

1. **The parallel implementation seems correct.** `bm_parallel/1` matches `bm_sequential`
   to within 0.5 % (6 654 ms vs 6 618 ms). Dispatching one task at a time through the
   `thread_pool` adds essentially no measurable overhead. Sanity check

2. **Scaling is near-linear up to the physical core count.** Going from 1 to 2 to 4
   threads gives 1.92× and 3.68× speedups, about 96 % and 92 % of the ideal linear
   line.
3. **Returns diminish at 8 threads.** 5.65× rather than ~8×, about 71 % efficiency.

4. **Over-subscription was successfully avoided.** Pool size is capped at
   `hardware_concurrency()`, so even though I queue 100 tasks the live thread count
   never exceeds 8. If I spawned one `std::thread` per simulation, the 100
   threads would have contended for 8 cores.
## Caveats

- Each measurement is **1 iteration** (Google Benchmark stops doubling iterations once
  the shortest run already exceeds `--benchmark_min_time=0.5s`). The
  reported numbers are therefore a single sample each
