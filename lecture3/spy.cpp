#include <iostream>
#include "spy.hpp"



int main()
{
    Spy Spy_a {}; // Construct
    Spy Spy_b {}; // Construct
    Spy_a = Spy_b; // Copy assign
    Spy_b = std::move(Spy_a); // Move assign
    Spy c(std::move(Spy_a)); // Move
    Spy Spy_a_copy = Spy_a; // Copy
    Spy Spy_c(3);
    std::cout << Spy_c << std::endl;
}