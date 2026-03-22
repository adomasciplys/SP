/**
 * Purpose: try out various API designs using lambda expressions.
 * @author Marius Mikucionis <marius@cs.aau.dk>
 */
#include <sstream>
#include <iostream>
#include "storage.h"

std::string try_printer()
{
    auto os = std::ostringstream{};
    auto d = storage<int>{100};
    auto printer = [&os](auto&& e) { os << e << " "; };

    // Method 1: ranged-loop via get_values()
    // get_values() allocates a brand new std::vector<int> and copies every element into it (dereferencing each unique_ptr).
    // We then iterate that copy with a range-for loop.
    // Pros: Very readable — looks like a normal loop over plain values.
    // Cons: Allocates and copies the entire collection just to iterate it — wasteful for large data.
    for (auto&& e : d.get_values())
        printer(e);
    os << std::endl;

    // Method 2: apply() — template function, passes fn by forwarding reference
    // apply() takes any callable via a template parameter (no type-erasure overhead).
    // Internally it calls std::for_each on the raw unique_ptr vector, dereferencing each pointer
    // before calling fn, so the caller never sees pointers.
    // Pros: Zero overhead — the lambda is inlined at compile time. No copies, no allocation.
    // Cons: Slightly more complex to implement; requires a template, so the function body
    //       must live in the header.
    d.apply(printer);
    os << std::endl;

    // Method 3: for_each() — takes fn as std::function<void(T)>
    // std::function is a type-erased wrapper: it can hold any callable with the right signature,
    // but it pays for that flexibility with a heap allocation and a virtual-call-like dispatch
    // every time fn is invoked.
    // Pros: Cleaner API — the signature documents exactly what the function expects (void(T)).
    //       No template needed, so the implementation can live in a .cpp file.
    // Cons: Noticeably slower than apply() for large collections because of type-erasure overhead.
    //       The lambda is NOT inlined.
    d.for_each(printer);
    os << std::endl;
    return os.str();
    // Preferred: apply() (Method 2) — it is the most efficient and still easy to call.
    // get_values() is fine for small data or when you genuinely need a copy.
    // for_each() is useful when you want a stable, non-template API (e.g. virtual interfaces).
}

std::string try_adder()
{
    auto os = std::ostringstream{};
    auto d = storage<double>{100};
    auto adder = [](auto&& e1, auto&& e2) { return e1 + e2; };
    auto sum1 = 0.0;

    // Method 1: ranged-loop via get_values()
    // get_values() copies all elements into a plain vector<double>, then the loop manually
    // adds each value into sum1 with +=.
    // Pros: Simple and easy to read.
    // Cons: Copies the entire collection — wasteful for large data. The summing logic is
    //       written by hand instead of reusing a standard algorithm.
    for (auto&& e : d.get_values())
        sum1 += e;
    os << sum1 << std::endl;

    // Method 2: for_each() with a capturing lambda
    // Instead of copying the data, we pass a lambda that captures sum2 by reference.
    // for_each() visits each element and calls the lambda, which adds the value directly
    // into sum2 — no copy of the collection is made.
    // Pros: No allocation/copy. The accumulation logic is neatly contained in the lambda.
    // Cons: Uses std::function internally (type-erasure overhead). The lambda has a side
    //       effect (mutating sum2 via capture), which is less clean than a pure fold.
    auto sum2 = 0.0;
    d.for_each([&sum = sum2](auto&& e) { sum += e; });
    os << sum2 << std::endl;

    // Method 3: accumulate() — template, passes fn by forwarding reference
    // accumulate() uses std::accumulate internally: it starts from init (0) and repeatedly
    // calls adder(runningTotal, currentElement), building up the sum step by step.
    // The adder lambda is inlined at compile time — no type-erasure, no overhead.
    // Pros: Clean, expressive, and zero-cost. The intent (fold/reduce) is obvious.
    // Cons: Template, so implementation must be in the header.
    os << d.accumulate(0, adder) << std::endl;

    // Method 4: combine() — same idea as accumulate(), but takes fn as std::function
    // Internally does the same std::accumulate fold, but wraps fn in std::function<T(T,T)>,
    // which adds type-erasure overhead on every call to adder.
    // Pros: Stable, non-template API — the exact signature is documented in the function header.
    // Cons: Slower than accumulate() due to std::function dispatch on every element.
    os << d.combine(0, adder) << std::endl;
    return os.str();
    // Preferred: accumulate() (Method 3) — zero overhead, clean fold semantics.
    // for_each() (Method 2) works but is a roundabout way to sum things.
    // get_values() (Method 1) is fine for small data but needlessly copies the collection.
}

int main()
{
    try_printer();
    try_adder();
}
