#include "spy.hpp"

Spy make_spy() {
    auto result = Spy{};
    return result;
}

std::ostream& print(Spy spy)  { return std::cout << spy; }
//std::ostream& print(Spy& spy) { return std::cout << spy; }

int main() {
    auto spy = Spy{};
    print(spy) << std::endl;
    print(Spy{}) << std::endl;
    print(make_spy()) << std::endl;
}