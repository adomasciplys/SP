#ifndef MINIPROJECT_THREAD_POOL_HPP
#define MINIPROJECT_THREAD_POOL_HPP

#include <thread>
#include <mutex>
#include <future>
#include <functional>
#include <queue>
#include <algorithm>
#include <ranges>

class thread_pool
{
    std::vector<std::jthread> threads;
    std::mutex mutex;              // guards tasks
    std::condition_variable cond;  // signals between threads about changes in tasks
    std::queue<std::function<void()>> tasks;
    std::atomic<bool> terminate{false};

    std::function<void()> pop()
    {
        auto task = std::function<void()>{};
        auto lock = std::unique_lock(mutex);
        while (tasks.empty() && !terminate)
            cond.wait(lock);  // release,wait,acquire
        if (!terminate && !tasks.empty()) {
            task = std::move(tasks.front());
            tasks.pop();
        }
        return task;
    }

    void push(std::function<void()> task)
    {
        auto lock = std::unique_lock(mutex);
        tasks.push(std::move(task));
        cond.notify_all();
    }

    void stop()
    {
        auto lock = std::unique_lock(mutex);
        terminate = true;
        cond.notify_all();
    }

public:
    explicit thread_pool(size_t number_of_threads = std::jthread::hardware_concurrency())
    {
        // spawn the number of threads:
        while (number_of_threads-- > 0)
            threads.emplace_back([this] {
                while (!terminate) {
                    auto task = pop();
                    if (task)
                        task();  // execute the task
                }
            });
    }

    /** asynchronous dispatch over the pool, C++20 implementation */
    template <typename Fn, typename... Args, typename Ret = std::result_of_t<std::decay_t<Fn>(std::decay_t<Args>...)>>
    std::future<Ret> async(Fn&& fn, Args&&... args)
    {
        // promise is not copyable, thus wrap into a smart pointer:
        auto ret = std::make_shared<std::promise<Ret>>();
        push([ret, fn = std::forward<Fn>(fn), ... args = std::forward<Args>(args)]() mutable {
            try {
                if constexpr (std::is_same_v<Ret, void>) {
                    fn(std::forward<decltype(args)>(args)...);
                    ret->set_value();
                } else {
                    ret->set_value(fn(std::forward<decltype(args)>(args)...));
                }
            } catch (...) {
                ret->set_exception(std::current_exception());
            }
        });
        return ret->get_future();
    }

    template <typename Fn, typename In, typename Out = std::invoke_result_t<Fn, In>>
    std::vector<std::future<Out>> dispatch(Fn&& fn, const std::vector<In>& inputs)
    {
        auto res = std::vector<std::future<Out>>(inputs.size());
        std::ranges::transform(inputs, res.begin(), [this, &fn](const In& input) { return async(fn, input); });
        return res;
    }

    ~thread_pool()
    {
        stop();
        for (auto& thread : threads)
            thread.join();
    }
};

template <typename T>
std::vector<T> collect(std::vector<std::future<T>>&& futs)
{
    auto res = std::vector<T>(futs.size());
    std::ranges::transform(futs, res.begin(), &std::future<T>::get);
    return res;
}

#endif //MINIPROJECT_THREAD_POOL_HPP