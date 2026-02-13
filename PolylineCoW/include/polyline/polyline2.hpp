#pragma once
#include "polyline/point.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <memory>

namespace poly {

struct Polyline2
{
    Polyline2() : points(new std::vector<Point>{}), shared(true) {}

    ~Polyline2() noexcept
    {
        if (!shared) {
            delete points;
        }
    }

    Polyline2& operator=(const Polyline2& poly)
    {
        if (this != &poly) {

                if (!shared) {
                    delete points;
                }
            points = poly.points;
            shared = true;
        }
        return *this;
    }

    Polyline2& operator=(Polyline2&& poly) noexcept
    {
        std::swap(shared, poly.shared);
        std::swap(points, poly.points);
        return *this;
    }

    Polyline2(const Polyline2& other) { *this = other; }
    Polyline2(Polyline2&& poly) noexcept { *this = std::move(poly); }

    void operator+=(const Point& new_point)
    {
        if (shared) {
            points = clone();
            shared = false;
        }
        points->push_back(new_point);
    }

    explicit operator bool() const { return points && !points->empty(); }

private:
    std::vector<Point>* clone()
    {
        return points ? new std::vector<Point>{*points} : new std::vector<Point>{};
    } // Copy points

    std::vector<Point>* points = nullptr;
    bool shared = true;

    friend std::ostream& operator<<(std::ostream& os, const Polyline2& polyline)
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
inline std::string to_string(const poly::Polyline2& p)
{
    std::ostringstream oss;
    oss << p;  // Uses the existing operator<<
    return oss.str();
}
}  // namespace std
