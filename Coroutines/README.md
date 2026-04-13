# Ranges and Coroutines

Purpose:
* Try out [coroutines](https://en.cppreference.com/w/cpp/language/coroutines.html): 
  * [std::generator]() (comes with C++23)
  * [libcoro](https://github.com/jbaldwin/libcoro) (installed via [libcoro.cmake](cmake/libcoro.cmake))
  * [Boost::capy](https://github.com/cppalliance/capy) and [Boost::corosio](https://github.com/cppalliance/corosio) (installed via [corosio.cmake](cmake/corosio.cmake))
* See also:
  * [C++20 Coroutines for I/O Story](https://youtu.be/1fbKKnBhAZs) by Vinnie Falco. 
  * [How to Understand C++20 Coroutines from the Ground Up](https://www.vinniefalco.com/p/how-to-understand-c20-coroutines) by Vinnie Falco.
  * [Coroutines - The Comprehensive Concise Guide](https://simplifycpp.org/books/minibooklet/mini_booklet_Coroutines_The_Comprehensive_Concise_Guide.pdf) by Ayman Alheraki.

Build and test:
```shell
cmake -B build
cmake --build build
ctest --test-dir build
```

## Tasks
1. Inspect examples in [fibonacci_test.cpp](tests/fibonacci_test.cpp):
   - What are the use cases of `fibonacci()` function? Which C++ standard is used to create `std::vector`?
     - The fibonacci function can be used to generate infinite sequences of fibonacci numbers starting from two initial values
     - `std::from_range()`: When you pass std::from_range as the first argument to a container's constructor, 
        it acts as a signal to the compiler: "Do not treat the next argument as a single element. 
        Treat it as a range, iterate over it, and put its elements into this container."
   - Where is `fibonacci()` declared?
     - `fibonacci.hpp`
   - Where is `fibonacci()` defined/implemented?
     - `fibonacci.cpp`
   - Does `fibonacci<unsigned>()` work? Why?
     - Yes, because we use the following template `template <std::integral Number = std::size_t>`
     - The constraint `std::integral Number` is a C++20 concept that restricts the template parameter Number to be any integer type. 
     - This includes int, long, short, char, and all of their unsigned variants.
     - Since `unsigned` (which is shorthand for unsigned int) is an integral type, 
       it fully satisfies the `std::integral` constraint.
2. Examine examples in [generator_test.cpp](tests/generator_test.hpp):
   - What does `Fibonacci()` return? Compare with `fibonacci()` from above.
     - `Fibonacci()` uses a custom definition `Generator`
     - `fibonacci()` uses `std::generator()`
   - What does `Generator` do? How?
- **The Bridge (`struct promise_type`):**
  - When you write a coroutine, the compiler needs a way to communicate with it. The `promise_type` is that communication bridge.
  - **Catching the value:** When your `Fibonacci` function calls `co_yield n0;`, the compiler secretly forwards that value to `yield_value(Value value)`.
  - Your code then safely tucks that value away inside `std::optional<Value> _value`.
  - **Starting paused:** `initial_suspend()` returns `std::suspend_always`. This means when you call `Fibonacci(0, 1)`, it doesn't actually calculate the first number yet. It creates the coroutine, pauses it immediately at the very top, and waits for you to ask for the first number
- **The Remote Control (`Handle _handle`):**
    - `std::coroutine_handle` is quite literally a remote control for the frozen function.
    - Your `Generator` class stores this remote control (`_handle`).
    - Whenever the outside code wants the coroutine to do work, it presses play by calling `_handle.resume()`.
    - When your `Generator` object is finally destroyed (goes out of scope), the destructor presses the kill switch: `_handle.destroy()`, which wipes the paused function's memory clean.
- **The Disguise (`struct Iterator`):**
    - This is arguably the most clever part of the code. A coroutine is a weird, alien concept to standard C++. If you want to use it in a normal `for` loop (like `for (int n : Fibonacci())`), you have to disguise it as a standard container (like a `std::vector`).
    - Your `Iterator` struct provides that disguise:
        - **`operator*()`:** When the `for` loop asks for the current value, this function sneaks into the `promise_type` and grabs the value stored in `_value`.
        - **`operator++()`:** When the `for` loop tries to move to the "next" element, it isn't actually moving forward in memory. Instead, it hits `_handle.resume()`, waking the coroutine up to calculate the next number and hit the next `co_yield`.
        - **`operator==`:** This checks `_handle.done()` to tell the `for` loop if the coroutine has finished and the loop should end.
   - What are `static_assert` tests doing?
     - `std::weakly_incrementable<Iter>`: Requirements on types that can be incremented with ++
     - Basically all the static assert ensure we are using a valid iterator and are iterating over a container
       that can be iterated over
3. Examine examples in [coroutines_test.cpp](tests/coroutines_test.cpp):
   - What library is used to implement `Fibonacci()`?
4. Create a [Collatz](https://en.wikipedia.org/wiki/Collatz_conjecture) sequence generator coroutine for a given positive starting number. 
   * For example, the following is the sequence starting with 7:
     `7 22 11 34 17 52 26 13 40 20 10 5 16 8 4 2 1`. **IMPLEMENTED**
   * Collatz sequence is empty for `0` - **IMPLEMENTED**
   * Use these facts in your unit tests.
   * **Optional:** checkout `tests` branch with tests for Collatz functions and start from there (implement TODO items in [collatz.hpp](include/collatz.hpp) and [collatz.cpp](source/collatz.cpp) files).
5. Given a positive number, estimate the length of the Collatz sequence for that number. **IMPLEMENTED**
6. Create another coroutine generating an infinite sequence of lengths of Collatz sequences starting with 0, 1, 2, and so on. For example, the first 10 numbers: `0 1 2 8 3 6 9 17 4 20`. **IMPLEMENTED**
7. Compute the maximal length of Collatz sequences for starting numbers of up to 100. **IMPLEMENTED**
8. If time permits, add [memoization](https://en.wikipedia.org/wiki/Memoization) to Collatz length coroutine.

## Hints
See [generator](https://en.cppreference.com/w/cpp/coroutine/generator), use `coro::generator` or `Generator` if `std::generator` is not available.

See also: [take](https://en.cppreference.com/w/cpp/ranges/take_view),
[distance](https://en.cppreference.com/w/cpp/iterator/ranges/distance),
[max](https://en.cppreference.com/w/cpp/algorithm/ranges/max).
