// Exploration of C-style arrays and std::array.
// Each section groups one area of functionality so it is clear what is being tested.

#include <array>
#include <iostream>
#include <algorithm>
#include <numeric>

// Prints every element of a container on a single line.
template <typename Container>
void print_all(const Container& c)
{
    for (const auto& item : c)
    {
        std::cout << item << ' ';
    }
    std::cout << std::endl;
}

int main()
{
    std::cout << std::boolalpha;

    // ========================================================================
    //  Part 1: C-style arrays
    // ========================================================================

    // --- Construction -------------------------------------------------------
    std::cout << "=== C-style arrays ===" << std::endl;
    int c_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // size deduced from list
    int zeroed[5] = {};                              // all elements zero-initialized
    int partial[5] = {1, 2};                         // {1, 2, 0, 0, 0}

    // --- Size ---------------------------------------------------------------
    // C arrays don't track their own size; compute it from sizeof.
    std::size_t count = sizeof(c_array) / sizeof(c_array[0]);
    std::cout << "size: " << count << std::endl;

    // --- Element access -----------------------------------------------------
    std::cout << "first: " << c_array[0] << ", last: " << c_array[count - 1] << std::endl;

    // --- Pointer decay ------------------------------------------------------
    // An array name decays to a pointer to its first element.
    int* ptr = c_array;
    std::cout << "via pointer ptr[0]: " << ptr[0] << std::endl;
    std::cout << "via pointer *(ptr + 6): " << *(ptr + 6) << std::endl; // pointer arithmetic

    // --- Iteration ----------------------------------------------------------
    for (std::size_t i = 0; i < count; i++)
    {
        c_array[i] += 1;
    }
    print_all(c_array); // range-based for works on C arrays of known size

    print_all(zeroed);
    print_all(partial);

    // --- 2D arrays ----------------------------------------------------------
    int grid[2][3] = {{1, 2, 3}, {4, 5, 6}};
    std::cout << "grid[1][2]: " << grid[1][2] << std::endl;

    // ========================================================================
    //  Part 2: std::array
    // ========================================================================

    // --- Construction -------------------------------------------------------
    std::cout << "=== std::array ===" << std::endl;
    std::array<int, 5> arr = {1, 2, 3, 4, 5}; // fixed compile-time size
    std::array<int, 3> filled;
    filled.fill(7);                            // set every element to 7

    print_all(arr);
    print_all(filled);

    // --- Capacity -----------------------------------------------------------
    std::cout << "size: " << arr.size() << std::endl;       // number of elements
    std::cout << "empty: " << arr.empty() << std::endl;     // always false for non-zero size
    std::cout << "max_size: " << arr.max_size() << std::endl;

    // --- Element access -----------------------------------------------------
    std::cout << "front: " << arr.front() << std::endl;     // first element
    std::cout << "back: " << arr.back() << std::endl;       // last element
    std::cout << "at(2): " << arr.at(2) << std::endl;       // bounds-checked
    std::cout << "[2]: " << arr[2] << std::endl;            // unchecked
    int* data = arr.data();                                 // pointer to underlying storage
    std::cout << "data()[0]: " << data[0] << std::endl;

    // --- Iterators ----------------------------------------------------------
    std::cout << "begin: " << *arr.begin() << std::endl;
    std::cout << "rbegin: " << *arr.rbegin() << std::endl;  // reverse: last element
    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;

    // --- Operations ---------------------------------------------------------
    std::array<int, 3> x = {1, 2, 3};
    std::array<int, 3> y = {9, 8, 7};
    x.swap(y);                                  // exchange contents (same type/size only)
    print_all(x);
    print_all(y);

    // --- Comparison ---------------------------------------------------------
    std::array<int, 3> p = {1, 2, 3};
    std::array<int, 3> q = {1, 2, 4};
    std::cout << "p == q: " << (p == q) << std::endl;
    std::cout << "p < q: " << (p < q) << std::endl; // lexicographical

    // --- Structured bindings ------------------------------------------------
    auto [first, second, third] = p; // unpack a std::array
    std::cout << "unpacked: " << first << ' ' << second << ' ' << third << std::endl;

    // --- Use with <algorithm> -----------------------------------------------
    std::array<int, 5> nums = {5, 3, 1, 4, 2};
    std::sort(nums.begin(), nums.end());
    print_all(nums);
    std::cout << "sum: " << std::accumulate(nums.begin(), nums.end(), 0) << std::endl;
}
