#ifndef COROUTINES_GENERATOR_HPP
#define COROUTINES_GENERATOR_HPP

#include <iterator>
#include <limits>
#include <optional>
#include <coroutine>
#include <concepts>

#include <cstddef>  // ptrdiff_t

/// Implements a range for coroutines from scratch (similar to std::generator from C++23, but using C++20)
template <std::movable Value>
struct Generator
{
    struct promise_type  ///< interface to coroutine (the type name is fixed)
    {
        Generator get_return_object() { return Generator{std::coroutine_handle<promise_type>::from_promise(*this)}; }
        static std::suspend_always initial_suspend() noexcept { return {}; }  // start suspended
        static std::suspend_never final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(Value value) noexcept  // called by coroutine upon co_yield
        {
            _value = std::move(value);
            return {};
        }
        static void unhandled_exception() { throw; }
        const Value& value() const noexcept { return *_value; }  // called by Iterator to extract the value

    private:
        std::optional<Value> _value;
    };  // struct Promise
    using Handle = std::coroutine_handle<promise_type>;
    /// Range interface:
    struct Iterator
    {
        /// weakly_incrementable:
        using difference_type = std::ptrdiff_t;
        Iterator& operator++()
        {
            _handle.resume();
            return *this;
        }
        Iterator operator++(int) { return operator++(); }
        /// indirectly_readable:
        using value_type = const Value;
        const Value& operator*() const noexcept { return _handle.promise().value(); }
        const Value* operator->() const noexcept { return std::addressof(_handle.promise().value()); }
        /// equality_comparable:
        bool operator==(const Iterator& other) const noexcept = default;
        bool operator==(std::default_sentinel_t /*unused*/) const noexcept { return !_handle || _handle.done(); }
        /// default_initializable:
        Iterator() = default;
        explicit Iterator(const Handle handle): _handle{handle} {}

    private:
        Handle _handle{};  ///< shared handle
    };  // struct Iterator
    Iterator begin() const
    {
        if (_handle)
            _handle.resume();
        return Iterator{_handle};
    }
    static std::default_sentinel_t end() { return {}; }
    explicit Generator(const Handle handle): _handle{handle} {}
    ~Generator()
    {
        if (_handle)
            _handle.destroy();
    }
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator(Generator&& other) noexcept { *this = std::move(other); }
    Generator& operator=(Generator&& other) noexcept
    {
        std::swap(_handle, other._handle);
        return *this;
    }

private:
    Handle _handle{};
};  // struct Generator

/// Infinite Fibonacci sequence starting with n0 and n1
template <std::integral Number = std::size_t>
Generator<Number> Fibonacci(Number n0 = 0, Number n1 = 1);

template <std::integral Number>
Generator<Number> Random(Number from = std::numeric_limits<Number>::min(),
                         Number to = std::numeric_limits<Number>::max());

#endif  // COROUTINES_GENERATOR_HPP
