#include <algorithm>
#include <cassert>
#include <vector>
#include <random>

auto arithmetic_progression(int a_0, int step)
{
    return [=](int index) {return a_0 + step * index; };
}

auto just_odd(const std::vector<int>& v)
{
    std::vector<int> result;
    std::copy_if(v.begin(), v.end(), std::back_inserter(result),
                 [](int x) { return x % 2 != 0; });
    return result;
}

auto arithmetic_sequence(int a_0, int step)
{
    return [current = a_0, step]() mutable
    {
        int out = current;
        current += step;
        return out;
    };
}

auto random(float from, float till)
{
    return [from, till]()
    {
        auto gen = std::mt19937{std::random_device{}()};
        auto dist = std::uniform_real_distribution<double>{from, till};
        return dist(gen);
    };
}

double ipow(double base, int p)
{
    return [=]()
    {
        if (p < 0)
        {
            return 1 / ipow(base, -p);
        }
    };
}

int main() {
    auto ap = arithmetic_progression(3,2);
    assert(ap(0) == 3);
    assert(ap(1) == 5);
    assert(ap(2) == 7);
    assert(ap(3) == 9);
    assert(ap(4) == 11);
    auto odd = just_odd({0,1,2,3,4,5,6,7,8,9});
    auto res = std::vector<int> {1,3,5,7,9};
    assert(odd == res);
    auto as = arithmetic_sequence(3,2);
    assert(as() == 3);
    assert(as() == 5);
    assert(as() == 7);
    assert(as() == 9);
    auto r10 = random(0,10);
    for (auto i = 0; i < 1000; i++) {
        auto n = r10();
        assert(0 <= n);
        assert(n < 10);
    }
    // auto p0 = ipow(2,0);
    // assert(p0 == 1);
    // auto p1 = ipow(2,1);
    // assert(p1 == 2);
    // auto p2 = ipow(2,2);
    // assert(p2 == 4);
    // auto p3 = ipow(2,3);
    // assert(p3 == 8);
    // auto p4 = ipow(2,-1);
    // assert(p4 == 0.5);
    // auto p5 = ipow(2,-2);
    // assert(p5 == 0.25);
}