#ifndef CONCURRENCY_MTQUEUE_H
#define CONCURRENCY_MTQUEUE_H

#include <optional>
#include <list>
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T> // The multi-threaded queue can hold different types
class mtqueue
{
    // std::deque = A container of items.
    // You can add items to the front. You can add items to the back.
    // You can remove items from the front. You can remove items from the back.

    // std::queue = A container of items.
    // You can only add items to the back.
    // You can only remove items from the front.

    // std::list = A container of items.
    // Every item connects to the item before it.
    // Every item connects to the item after it.
    // You can add items anywhere in the container.
    // You can remove items anywhere in the container.
    std::queue<T, std::deque<T>> container;  // Before there was std::list used here
public:
    void put(T&& item)
    {
        auto lock = std::scoped_lock{m};
        container.push(std::move(item));
        cond.notify_all();
    } // rvalue reference (move)
    
    void put(const T& item)
    {
        auto lock = std::scoped_lock{m};   
        container.push(item);
        cond.notify_all();    
    } // lvalue reference (makes a copy)

    // returns an optional value
    T get()
    {
        auto lock = std::unique_lock{m};
        while (container.empty()) {
            cond.wait(lock); 
        }
        T res = std::move(container.front()); // try to read the item from the front of the queue
        container.pop(); // only pop after having secured the item
        return res;
    }

    private:
        std::mutex m;
        std::condition_variable cond;
};

/*
    Code before locks

    std::optional<T> get()
    {
        auto res = std::optional<T>{}; // std::optional either returns valid value or not
        if (!container.empty()) {
            res = std::move(container.front()); // try to read the item from the front of the queue
            container.pop(); // only pop after having secured the item
        }
        return res;
    }
*/

#endif  // CONCURRENCY_MTQUEUE_H
