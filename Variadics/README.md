# Template Exercises

Learning objectives:
 * Learn function templates (tasks 1-3)
 * Learn class templates (tasks 4-7).

## Tasks

1. Open [type_name_test.cpp](src/type_name_test.cpp):
   - Compile and run it, observe all tests fail
   - Fix the test by entering the names of respective types
   - Recompile and run, observe tests passing.
   - Inspect how type_name is implemented in [type_name.hpp](include/type_name.hpp).
   - Extra: inspect [type_name_boost.cpp](src/type_name_boost.cpp) how to use Boost implementation.

2. Open [minmax.cpp](src/minmax.cpp):
   - Observe various ways of finding min and max values in container (functions if_cond, minmax, element).
   - Inspect test_* functions: what are the types of arguments used in function calls and what are the (formal) parameter types?
   - Run `minmax_test` and observe the test passing.
   - Refactor all `test_*` *functions* into one `test` *function* **template** taking a function to be tested as a parameter.
   - Check that `minmax_test` is still passing.
   - Run `minmax_bm`, observe output and determine the fastest way to find min and max values (use Release config!).
   - Optional: recompile the project using a different compiler and see if the fastest method is the same over there.

3. Open [minmax_bm.cpp](src/minmax_bm.cpp):
   - Observe how `measure` function is used: what are the types of arguments and the types of (formal) parameters?
   - Open [measure.hpp](include/measure.hpp) and refactor `measure` by solving TODO items.

4. Implement compile-time Fibonacci sequence [fibonacci.hpp](include/fibonacci.hpp)
    - benchmark against run-time implementation of Fibonacci function
    - Extra: Implement compile-time table for Fibonacci sequence (tip: use std::make_index_sequence)

5. Implement compile-time Power by integer meta-function [power.hpp](include/power.hpp)
    - benchmark against runtime power function implementation
    - benchmark against [std::pow](https://en.cppreference.com/w/cpp/numeric/math/pow)

6. Open [kindof_test.cpp](src/kindof_test.cpp):
   - Follow the TODO comments and uncomment tests
   - Compile and run it, observe all tests fail
   - Follow the TODO comments and implement kind_t template
   - Recompile and run it again, observer the test pass
   - Fix the kind_t template as necessary for the tests to pass.

7. Extra: Implement compile-time prime factorization
    - Implement compile-time integer square-root
    - Implement compile-time prime check
