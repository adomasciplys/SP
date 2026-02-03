# Basics

Learning goals:
1. Fundamental types and basic ways of creating custom types.
2. Different ways of passing parameters to functions.
3. Measure the performance with [measure.hpp](measure.hpp) library.

## Commands Without Clion/CMake:
* Compile and run `types` with Debug info:
```shell
c++ -std=c++23 -g -Wpedantic -Wall -Wextra -o types types.cpp
./types
```
* Compile and run `passing_bm` with Release optimizations:
```shell
c++ -std=c++23 -O3 -Wpedantic -Wall -Wextra -c -o passing.o passing.cpp
c++ -std=c++23 -O3 -Wpedantic -Wall -Wextra -o passing_bm passing_bm.cpp passing.o
./passing_bm 5
```
* Clean:
```shell
rm -f types passing_bm
```
The structure of the project:
- [type.cpp](type.cpp) -- self-contained executable program.
- [passing.hpp](passing.hpp) -- library header contains declarations.
- [passing.cpp](passing.cpp) -- library implementation contains definitions.
- [passing_bm.cpp](passing_bm.cpp) -- executable program which links with `passing` library.
- [measure.hpp](measure.hpp) -- header-only library for simple benchmarking.
- [measure_test.cpp](measure_test.cpp) -- executable program testing the [measure.hpp](measure.hpp) library.
- [CMakeLists.txt](CMakeLists.txt) -- CMake build script which tells how to put everything together.

## Types Exercise

1. Open [types.cpp](types.cpp) and solve `TODO` items: either one by one or all at once.
2. Choose target `types`, compile and run it.
3. If some assertion fails, fix the code according to `TODO` instructions
4. Recompile and run again until program completes successfully.

Tips:
- The value of expression `expr` can be printed using either statement:
  - `std::cout << "expr: " << expr << "\n";` (old C++)
  - `std::println("expr: {}", expr);` (C++23)
- Sometimes runtime C `assert` can be converted into compile time C++ `static_assert` where the compiler gives more diagnostics, including the answers.



## Functions Exercise

IMPORTANT: when measuring *performance*, always use **Release** preset (CMake configuration), as Debug preset includes debugging instrumentation which distorts the performance. 

1. Open [passing.hpp](passing.hpp) and inspect function signatures: 
   - How do they handle their arguments? What gets copied?
   - Which one(s) do you prefer?
2. Open [passing.cpp](passing.cpp) and inspect the function implementations:
   - Compare the implementations: how the data is stored and addressed?
   - Is the code functionally equivalent? 
   - Would it be fair to compare their performance?
3. Make predictions and write them down:
   - Which function is the fastest?
   - Which function is the slowest?
4. Open [passing_bm.cpp](passing_bm.cpp) and inspect the benchmarking code for each functions:
   - Are the benchmark code equivalent and fair for each function?
5. Run `passing_bm` in `Release` preset/configuration and inspect the results:
   - Is the error margin (±) smaller than the measurement in all cases? If not, increate the number of iterations (value of `COUNT`).
   - Which function takes the shortest time? the longest?
   - Are the results consistent when run multiple times?
   - Compare the results with your predictions.
6. Change the size of the array in `Large` struct (line 8 in [passing.hpp](passing.hpp)), rerun `passing_bm` and inspect the performance:
   - Find the largest array size which yields similar performance as functions with `Small` argument.
7. Replace the array in `Large` struct with `std::array<int,2>` and repeat the measurements:
   - Compare with the performance of functions using `Small`, what is the overhead of `std::array`?
8. Replace the array in `Large` struct with `std::vector<int>` and repeat the measurements:
   - Compare with the performance of functions using `Small`, what is the overhead of `std::vector`?
9. What are you going to choose to use and under what circumstances?
   - For data structures: `int[]`, `std::array` or `std::vector`?
   - For parameter passing: value, pointer or reference?

## Extra

1. Write two functions which swaps the halves of a sequences:
   - `[1,2,3,4,5,6,7]` -> `[5,6,7,4,1,2,3]`
   - First: `void swap_ends(int[] arr, size_t size);`
   - Second: `void swap_ends(std::vector<int>& vec);`
2. Compare the code syntax in function bodies:
   - What are similarities?
   - Differences?
   - Compare the data layout.
3. Measure and compare performance:
   - Which one is faster?

Tips:
- Create a new file for benchmark executable, say `swap_bm.cpp`, then append the following to [CMakeLists.txt](CMakeLists.txt) and reload CMake and run `swap_bm` target:
```cmake
add_executable(swap_bm swap_bm.cpp)
add_test(NAME swap_bm COMMAND swap_bm)
set_tests_properties(swap_bm swap_bm PROPERTIES RUN_SERIAL ON)
```
- Generate random data for the benchmark inputs as compiler could not optimize and precompute the results at compile time.
```cpp
#include <random>
#include <iostream>
void swap_ends(int arr[], size_t size){ ... }
void swap_ends(std::vector<int>& arr) { ... }

Measurements meas;
constexpr unsigned COUNT = 1000;
void swap_ends_array_bm(int input[], size_t size, const size_t count = COUNT) { ... }
void swap_ends_vector_bm(std::vector<int>& input, const size_t count = COUNT) { ... }
int main()
{
    constexpr unsigned SIZE = 1000;
    int ai[SIZE];
    std::mt19937 gen{std::random_device{}()}; // random number generator
    std::uniform_int_distribution<int> dist{0, 10}; // distribution
    for (unsigned i = 0; i < SIZE; ++i)
        ai[i] = dist(gen); // generates random numbers 0..10
    std::vector vi(ai, ai+SIZE); // reates a copy
    swap_ends_array_bm(ai, SIZE);
    swap_ends_vector_bm(vi, SIZE);
    std::cout << meas;
}
```
