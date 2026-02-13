#ifndef LECTURE3_SPY_HPP
#define LECTURE3_SPY_HPP
#include <iostream>


struct Spy
{
    Spy (){ std::cout << "Spy constructor" << std::endl; data = 0; } // Exercise 3.1.1
    Spy (int x){ data = x; }
    ~Spy (){ std::cout << "Spy destruct" << std::endl; } // Exercise 3.1.2
    Spy (const Spy& spy){ std::cout << "Spy copy" << std::endl; data = spy.data;} // Exercise 3.1.3
    Spy (Spy&& spy)  { std::cout << "Spy move" << std::endl; data = spy.data;} // Exercise 3.1.4
    Spy& operator=(const Spy& spy){ std::cout << "Spy copy assign" << std::endl; return *this;} // Exercise 3.1.5
    Spy& operator=(Spy&& spy) { std::cout << "Spy move assign" << std::endl; return *this;} // Exercise 3.1.6
private:
    int data; // Exercise 3.1.7

    friend std::ostream& operator<<(std::ostream& os, const Spy& s)
    {
        os << "{" << s.data << s.data << s.data << "}";
        return os;
    }
};

#endif //LECTURE3_SPY_HPP