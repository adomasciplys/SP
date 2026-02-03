/// Declarations for a library with various function signatures
/// Author: Marius Mikucionis marius@cs.aau.dk

#ifndef INCLUDE_PASSING_HPP
#define INCLUDE_PASSING_HPP
#include <array>
#include <vector>

struct Small { int numbers[2]; };
struct Large { std::vector<int> numbers; };

// Replace the array in Large struct with std::array<int,2> and repeat the measurements
// Compare with the performance of functions using Small, what is the overhead of std::array?
// There seems to be no overhead to using std::array
// 3: pass_by_cref  Large: 0.0136 ± 0.00325045 μs (5000)
// 3: pass_by_cref  Small: 0.0114 ± 0.00297927 μs (5000)
// 3: pass_by_ptr   Large: 0.014 ± 0.00329724 μs (5000)
// 3: pass_by_ptr   Small: 0.0146 ± 0.00336613 μs (5000)
// 3: pass_by_ref   Large: 0.0164 ± 0.00518468 μs (5000)
// 3: pass_by_ref   Small: 0.0132 ± 0.00320294 μs (5000)
// 3: pass_by_value Large: 0.0196 ± 0.00389025 μs (5000)
// 3: pass_by_value Small: 0.0148 ± 0.00338876 μs (5000)


// Replace the array in Large struct with std::vector<int> and repeat the measurements:
// Compare with the performance of functions using Small, what is the overhead of std::vector?
// Now there seems to be an overhead in regard to pass by value Large. Not that it is big but some overhead is definitely added
// For pass by reference or pointer there is almost no difference
// 3: pass_by_cref  Large: 0.014 ± 0.00329724 μs (5000)
// 3: pass_by_cref  Small: 0.0134 ± 0.00322679 μs (5000)
// 3: pass_by_ptr   Large: 0.0142 ± 0.00332037 μs (5000)
// 3: pass_by_ptr   Small: 0.012 ± 0.00305574 μs (5000)
// 3: pass_by_ref   Large: 0.0148 ± 0.00338876 μs (5000)
// 3: pass_by_ref   Small: 0.019 ± 0.00387231 μs (5000)
// 3: pass_by_value Large: 0.0372 ± 0.00531113 μs (5000)
// 3: pass_by_value Small: 0.016 ± 0.00352132 μs (5000)



// Questions:
// Find the largest array size which yields similar performance as functions with Small argument
// For an array size of numbers[100] the Small and Large performance are almost the same
// 3: pass_by_ref   Small: 0.0194 ± 0.00387074 μs (5000)
// 3: pass_by_value Large: 0.0188 ± 0.00381158 μs (5000)

int pass_by_value(Small arg);
// C++ makes a copy of the Small struct and then uses that in the function
// This is ok since the argument is small so not a lot needs to be copied

int pass_by_ptr(Small* arg);
// C++ passes the pointer to the Small struct, meaning we operate on the memory directly in the function
// This has potential side effects

int pass_by_ref(Small& arg);
// C++ passes a reference to the Small struct, meaning we operate on the memory directly in the function
// This has potential side effects

int pass_by_cref(const Small& arg);
// C++ passes a reference to the Small struct, with the promise to not modify it
// This is the optimal way to pass arguments for large data, but is not necessary here necessarily

// Overloading the same function names for a different argument type:
int pass_by_value(Large arg);
// C++ makes a copy of the Small struct and then uses that in the function
// This is now bad since Large has a size of 40.000 bytes

int pass_by_ptr(Large* arg);
// C++ passes the pointer to the Large struct, meaning we operate on the memory directly in the function
// This has potential side effects, but is cheap

int pass_by_ref(Large& arg);
// C++ passes the reference to the Large struct, meaning we operate on the memory directly in the function
// This has potential side effects, but is cheap

int pass_by_cref(const Large& arg);
// C++ passes a reference to the Large struct, with the promise to not modify it
// This is the optimal way to pass arguments

#endif // INCLUDE_PASSING_HPP
