#ifndef CONCURRENCY_MTQUEUE_H
#define CONCURRENCY_MTQUEUE_H

#include <optional>
#include <list>
#include <queue>

template <typename T>
class mtqueue
{
    // std::deque = general-purpose double-ended sequence container
    // std::queue = queue interface built on top of another container
    std::queue<T, std::deque<T>> container;  // TODO: check performance
public:
    void put(T&& item) { container.push(std::move(item)); }
    void put(const T& item) { container.push(item); }
    std::optional<T> get()
    {
        auto res = std::optional<T>{};
        if (!container.empty()) {
            res = std::move(container.front());
            container.pop();
        }
        return res;
    }
};

#endif  // CONCURRENCY_MTQUEUE_H
