/// Definitions (implementation) for a library with various function signatures
/// Author: Marius Mikucionis marius@cs.aau.dk

#include "passing.hpp"

// Questions:
// - Compare the implementations: how the data is stored and addressed?
// - Is the code functionally equivalent
//     Yes, functionally the functions all return the same result
// - Would it be fair to compare their performance?
//    Yes, it’s fair to compare these implementations, because they all do the same read (numbers[0]) and return it.
// - Are the results consistent when run multiple times?
//     No, the fastest time can change, but the SLOWEST is always pass_by_value Large


// RESULTS:
// 3: pass_by_cref  Large: 0.01 ± 0.00279232 μs (5000)
// 3: pass_by_cref  Small: 0.0188 ± 0.00381158 μs (5000)
// 3: pass_by_ptr   Large: 0.0222 ± 0.00413475 μs (5000)
// 3: pass_by_ptr   Small: 0.0186 ± 0.00379164 μs (5000)
// 3: pass_by_ref   Large: 0.013 ± 0.00317891 μs (5000) FASTEST
// 3: pass_by_ref   Small: 0.0188 ± 0.00381158 μs (5000)
// 3: pass_by_value Large: 0.6346 ± 0.0135605 μs (5000) SLOWEST
// 3: pass_by_value Small: 0.0188 ± 0.00381158 μs (5000)

int pass_by_value(Small arg) { return arg.numbers[0]; }
// Firstly has to copy the Small struct and then access the first element

int pass_by_ptr(Small* arg) { return arg->numbers[0]; }
// Can access the first element directly

int pass_by_ref(Small& arg) { return arg.numbers[0]; }
// Can access the first element directly

int pass_by_cref(const Small& arg) { return arg.numbers[0]; } // FASTEST
// Can access the first element directly

int pass_by_value(Large arg) { return arg.numbers[0]; } // SLOWEST
// Firstly has to copy the Large struct and then access the first element

int pass_by_ptr(Large* arg) { return arg->numbers[0]; }
// Can access the first element directly

int pass_by_ref(Large& arg) { return arg.numbers[0]; }
// Can access the first element directly

int pass_by_cref(const Large& arg) { return arg.numbers[0]; }
// Can access the first element directly
