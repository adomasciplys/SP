#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <numeric>
#include <random>

/**
Storage class encapsulates some data, where the storage mechanism is hidden from the user.
Instead of giving direct access, the class provides a number of methods to access the data.
*/
template <typename T>
class storage
{
    std::vector<std::unique_ptr<T>> data;

public:
    storage(size_t size)
    {
        if constexpr (std::is_arithmetic_v<T>) {
            auto rng = std::mt19937(std::random_device{}());
            using Dist = std::conditional_t<std::is_integral_v<T>, std::uniform_int_distribution<T>,
                                            std::uniform_real_distribution<T>>;
            auto dist = Dist{-100, 100};
            data.resize(size);
            std::generate(std::begin(data), std::end(data), [&dist, &rng] { return std::make_unique<T>(dist(rng)); });
        }
    }

    const std::vector<T> get_values() const
    {
        auto copy = std::vector<T>{};
        copy.reserve(data.size());
        for (auto&& e : data)
            copy.push_back(*e);
        return copy;
    }

    template <typename Fn>
    void apply(Fn&& fn) const
    {
        static_assert(std::is_invocable_v<Fn, T>, "expecting function over (T)");
        std::for_each(std::begin(data), std::end(data), [fn = std::forward<Fn>(fn)](auto&& ptr) { fn(*ptr); });
    }

    void for_each(std::function<void(T)> fn) const
    {
        std::for_each(std::begin(data), std::end(data), [&fn](auto&& ptr) { fn(*ptr); });
    }

    template <typename Fn>
    auto accumulate(T init, Fn&& fn) const
    {
        static_assert(std::is_invocable_v<Fn, T, T>, "expecting function over (T,T)");
        return std::accumulate(
            std::begin(data), std::end(data), init,
            [fn = std::forward<Fn>(fn)](const T& v, const std::unique_ptr<T>& p) { return fn(v, *p); });
    }
    // In std::accumulate, the lambda gets called with two arguments each step:
    // - v — the running total so far (starts as init)
    // - p — the current element from the data, but it's a unique_ptr<T>, so *p dereferences it to get the actual value

    auto combine(T init, std::function<T(T, T)> fn) const
    {
        return std::accumulate(std::begin(data), std::end(data), init,
                               [&fn](const T& v, const std::unique_ptr<T>& p) { return fn(v, *p); });
    }
};
