#pragma once
#include "polyline/point.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <memory>

namespace poly {

struct Polyline
{
    void operator+=(const Point& new_point)
    {
        if (points.use_count() > 1) {
            points = std::make_shared<std::vector<Point>>(*points);
        }
        points->push_back(new_point);
    }

    explicit operator bool() const { return !points->empty(); }

private:
    std::shared_ptr<std::vector<Point>> points = std::make_shared<std::vector<Point>>();

    friend std::ostream& operator<<(std::ostream& os, const Polyline& polyline)
    {
        os << "Polyline@" << polyline.points << "[";
        for (std::size_t i = 0; i < polyline.points->size(); ++i) {
            if (i > 0) {
                os << ", ";
                os << polyline.points->at(i);
            }
        }
        os << "]";
        return os;
    }
};

}  // namespace poly

namespace std {
inline std::string to_string(const poly::Polyline& p)
{
    std::ostringstream oss;
    oss << p;  // Uses the existing operator<<
    return oss.str();
}
}  // namespace std
