#pragma once
#include <iostream>
#include <sstream>

namespace poly {

struct Point
{
    Point(int x, int y): x(x), y(y) {}
    int get_x() const { return x; }
    int get_y() const { return y; }
    void set_x(int new_x) { x = new_x; };
    void set_y(int new_y) { y = new_y; };

private:
    int x = 0;
    int y = 0;

    friend std::ostream& operator<<(std::ostream& os, const Point& p)
    {
        return os << "{" << p.x << ", " << p.y << "}" << std::endl;
    }
};
}  // namespace poly