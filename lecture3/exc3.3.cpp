#include "spy.hpp"

Spy make_spy() {
    auto result = Spy{};
    return result;
}

int main ()
{
    //std::cout << "Spy{}: " << Spy{} << std::endl;

    std::cout << "make_spy(): " << make_spy() << std::endl;

    // auto spy2 = make_spy();
    // std::cout << "spy2: " << spy2 << std::endl;
}