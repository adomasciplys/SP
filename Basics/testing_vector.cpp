// Exploration of the std::vector interface.
// Each section groups one area of functionality so it is clear what is being tested.

#include <vector>
#include <iostream>
#include <list>
#include <algorithm>
#include <numeric>
#include <cassert>

template <typename T>
void print(const T& item)
{
    std::cout << item << std::endl;
}

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
    std::cout << std::boolalpha; // print bools as true/false

    // --- Construction -------------------------------------------------------
    std::vector<int> empty_vec;                  // empty
    std::vector<int> from_list = {1, 2, 3, 4, 5}; // initializer list
    std::vector<int> filled(3, 100);             // 3 copies of 100
    std::vector<int> sized(4);                    // 4 value-initialized ints (0)
    std::vector<int> copy_vec(from_list);         // copy construction
    std::vector<int> range_vec(from_list.begin(), from_list.end()); // from iterator range

    std::cout << "--- Construction ---" << std::endl;
    print_all(from_list);
    print_all(filled);
    print_all(sized);
    print_all(range_vec);

    // --- Capacity -----------------------------------------------------------
    std::cout << "--- Capacity ---" << std::endl;
    print(from_list.size());      // number of elements
    print(from_list.empty());     // is it empty?
    print(empty_vec.empty());
    print(from_list.max_size());  // theoretical maximum number of elements
    empty_vec.reserve(10);        // request capacity for 10 elements
    print(empty_vec.capacity());  // current allocated capacity
    from_list.shrink_to_fit();    // reduce capacity to fit size

    // --- Element access -----------------------------------------------------
    std::cout << "--- Element access ---" << std::endl;
    print(from_list.front());     // first element
    print(from_list.back());      // last element
    print(from_list.at(2));       // bounds-checked access (throws std::out_of_range)
    print(from_list[2]);          // unchecked access
    int* raw = from_list.data();  // pointer to the underlying array
    print(raw[0]);

    // --- Iterators ----------------------------------------------------------
    std::cout << "--- Iterators ---" << std::endl;
    print(*from_list.begin());    // iterator to first element
    print(*(from_list.end() - 1));// end() is one-past-the-last
    print(*from_list.rbegin());   // reverse iterator -> last element
    print(*from_list.cbegin());   // const iterator
    for (auto it = from_list.begin(); it != from_list.end(); ++it)
    {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;

    // --- Modifiers: adding --------------------------------------------------
    std::cout << "--- Modifiers: adding ---" << std::endl;
    std::vector<int> mod;
    mod.push_back(1);             // append a copy
    mod.push_back(2);
    mod.emplace_back(3);          // construct element in place at the end
    mod.insert(mod.begin(), 0);   // insert single element at position
    mod.insert(mod.end(), 2, 9);  // insert 2 copies of 9 at position
    auto pos = std::next(mod.begin(), 2);
    mod.emplace(pos, 42);         // construct in place at position
    print_all(mod);

    // --- Modifiers: range insertion (C++23) ---------------------------------
    std::cout << "--- Range insertion ---" << std::endl;
    std::vector<int> container = {1, 2, 3, 4};
    const std::list<int> rg = {-1, -2, -3};
    auto insert_at = std::next(container.begin(), 2);
    assert(*insert_at == 3);
    container.insert_range(insert_at, rg);   // insert another range at a position
    container.append_range(rg);              // append a range at the end
    print_all(container);

    // --- Modifiers: removing ------------------------------------------------
    std::cout << "--- Modifiers: removing ---" << std::endl;
    mod.pop_back();               // remove last element
    mod.erase(mod.begin());       // erase single element
    mod.erase(mod.begin(), mod.begin() + 2); // erase a range
    print_all(mod);

    // --- Modifiers: assigning / replacing -----------------------------------
    std::cout << "--- Modifiers: assigning ---" << std::endl;
    std::vector<int> assigned;
    assigned.assign(4, 7);        // replace contents with 4 copies of 7
    print_all(assigned);
    assigned.assign({10, 20, 30});// replace contents with an initializer list
    print_all(assigned);

    // --- Modifiers: resize / clear / swap -----------------------------------
    std::cout << "--- resize / clear / swap ---" << std::endl;
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {9, 8};
    a.resize(5);                  // grow, new elements value-initialized (0)
    print_all(a);
    a.resize(2);                  // shrink, drops trailing elements
    print_all(a);
    a.swap(b);                    // exchange contents with another vector
    print_all(a);
    print_all(b);
    b.clear();                    // remove all elements (size becomes 0)
    print(b.empty());

    // --- Comparison ---------------------------------------------------------
    std::cout << "--- Comparison ---" << std::endl;
    std::vector<int> lhs = {1, 2, 3};
    std::vector<int> rhs = {1, 2, 4};
    print(lhs == rhs);
    print(lhs < rhs);             // lexicographical comparison

    // --- Use with <algorithm> -----------------------------------------------
    std::cout << "--- Algorithms ---" << std::endl;
    std::vector<int> nums = {5, 3, 1, 4, 2};
    std::sort(nums.begin(), nums.end());
    print_all(nums);
    print(std::accumulate(nums.begin(), nums.end(), 0)); // sum of elements
}
