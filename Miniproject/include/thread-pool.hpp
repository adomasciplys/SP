#ifndef MINIPROJECT_THREAD_POOL_HPP
#define MINIPROJECT_THREAD_POOL_HPP

#include <thread>       // std::jthread — a thread that auto-joins when destroyed
#include <mutex>        // std::mutex, std::unique_lock — protect the shared task queue
#include <future>       // std::future, std::promise — carry a result back to the caller
#include <functional>   // std::function — type-erased "any callable taking no args"
#include <queue>        // std::queue — FIFO list of pending tasks
#include <algorithm>    // std::ranges::transform (used in dispatch / collect)
#include <ranges>       // the range overloads of the above

// A fixed set of worker threads sharing one task queue.
// Submitting more tasks than there are threads simply queues them, so the number
// of threads actually running never exceeds the pool size (no over-subscription).
// This code has been snatched from the solutions in Lecture 11, but I have added comments for myself
class thread_pool
{
    std::vector<std::jthread> threads;  // the worker threads
    std::mutex mutex;              // guards tasks
    std::condition_variable cond;  // signals between threads about changes in tasks
    std::queue<std::function<void()>> tasks;  // pending jobs, run in FIFO order
    std::atomic<bool> terminate{false};       // set on shutdown to wake + stop workers

    // Take the next task off the queue, blocking until one exists or we are told to stop.
    std::function<void()> pop()
    {
        auto task = std::function<void()>{};
        auto lock = std::unique_lock(mutex);  // lock the queue before touching it
        // Nothing to do yet: sleep. wait() unlocks while asleep and re-locks on wake
        while (tasks.empty() && !terminate)
            cond.wait(lock);  // release,wait,acquire
        // Take the front task
        if (!terminate && !tasks.empty()) {
            task = std::move(tasks.front());
            tasks.pop();
        }
        return task;
    }

    // Add a task to the queue and wake a worker to run it.
    void push(std::function<void()> task)
    {
        auto lock = std::unique_lock(mutex);  // lock before modifying the queue
        tasks.push(std::move(task));
        cond.notify_all();  // wake sleeping workers so one picks it up
    }

    // Ask all workers to finish: raise the flag and wake everyone so they can exit.
    void stop()
    {
        auto lock = std::unique_lock(mutex);
        terminate = true;
        cond.notify_all();
    }

public:
    // Start `number_of_threads` workers (defaults to the machine's core count).
    explicit thread_pool(size_t number_of_threads = std::jthread::hardware_concurrency())
    {
        // spawn the number of threads:
        while (number_of_threads-- > 0)
            // Each worker loops forever: grab a task, run it, repeat
            threads.emplace_back([this] {
                while (!terminate) {
                    auto task = pop();  // blocks until there is work or we shut down
                    if (task)
                        task();  // execute the task
                }
            });
    }

    /** asynchronous dispatch over the pool, C++20 implementation */
    // Queue `fn(args...)` to run on some worker; hand back a future for its result.
    template <typename Fn, typename... Args, typename Ret = std::result_of_t<std::decay_t<Fn>(std::decay_t<Args>...)>>
    std::future<Ret> async(Fn&& fn, Args&&... args)
    {
        // promise is not copyable, thus wrap into a smart pointer:
        auto ret = std::make_shared<std::promise<Ret>>();
        // The job: run fn(args...), then store the result (or any exception) in the promise.
        push([ret, fn = std::forward<Fn>(fn), ... args = std::forward<Args>(args)]() mutable {
            try {
                if constexpr (std::is_same_v<Ret, void>) {
                    // void result: just run it and mark the future ready.
                    fn(std::forward<decltype(args)>(args)...);
                    ret->set_value();
                } else {
                    // non-void: run it and store the returned value.
                    ret->set_value(fn(std::forward<decltype(args)>(args)...));
                }
            } catch (...) {
                // Capture the exception; it is re-thrown when the caller calls future.get().
                ret->set_exception(std::current_exception());
            }
        });
        return ret->get_future();  // caller waits on this for the result
    }

    // Convenience: run `fn` once per input in parallel, returning a future per input.
    template <typename Fn, typename In, typename Out = std::invoke_result_t<Fn, In>>
    std::vector<std::future<Out>> dispatch(Fn&& fn, const std::vector<In>& inputs)
    {
        auto res = std::vector<std::future<Out>>(inputs.size());  // one future per input
        // Submit each input as its own task; keep the futures in input order.
        std::ranges::transform(inputs, res.begin(), [this, &fn](const In& input) { return async(fn, input); });
        return res;
    }

    // On destruction: tell workers to stop, then wait for each to finish.
    ~thread_pool()
    {
        stop();
        for (auto& thread : threads)
            thread.join();
    }
};

// Wait on every future and gather the results into a plain vector, in order.
// Takes the futures by rvalue because each future can only be get() once (it's consumed).
template <typename T>
std::vector<T> collect(std::vector<std::future<T>>&& futs)
{
    auto res = std::vector<T>(futs.size());
    // For each future, call .get() (blocks until ready) and store its value.
    std::ranges::transform(futs, res.begin(), &std::future<T>::get);
    return res;
}

#endif //MINIPROJECT_THREAD_POOL_HPP
