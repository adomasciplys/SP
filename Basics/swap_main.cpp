#include "swap.hpp"
#include <iostream>

int main()
{
    // Declare our data structures
    int a[]{1,2,3,4,5,6,7};
    std::vector<int> vec = {1,2,3,4,5,6,7};

    // Integer arrays have no way of deducing size
    int size = sizeof(a) / sizeof(a[0]);

    swap_ends(a, size);
    swap_ends(vec);

    for (int i : a) std::cout << i << " ";

    std::cout << std::endl;

    for (int i : vec) std::cout << i << " ";
}