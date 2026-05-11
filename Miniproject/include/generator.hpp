#ifndef MINIPROJECT_GENERATOR_HPP
#define MINIPROJECT_GENERATOR_HPP

#include <coroutine>
#include <iterator>
#include <optional>
#include <utility>

// A coroutine wrapper that produces a sequence of values lazily.
// Functions returning this type can use `co_yield` to return values one at a time.
// The coroutine pauses execution after each `co_yield`
// Only calculates the next value when the caller requests it
template <std::movable T>
struct Generator
{
    struct promise_type
    {
        // Very first function the compiler calls when the coroutine is invoked
        // Establishes the communication bridge between the promise and the wrapper
        Generator get_return_object()
        {
            return Generator{Handle::from_promise(*this)};
        };

        // Dictates what happens immediately after the coroutine starts running
        // suspend_always: The coroutine pauses immediately
        // Only produces values when explicitly asked to do so
        std::suspend_always initial_suspend() noexcept { return {}; }

        // Dictates what happens when the coroutine reaches the end of its code block
        // suspend_always: keeps current_value alive after the coroutine ends
        std::suspend_always final_suspend() noexcept { return {}; }

        // Called on co_yield
        std::suspend_always yield_value(T value)
        {
            _current_value = std::move(value);
            return {};
        }

        // Tells compiler how to handle void return
        void return_void() noexcept {}

        // Tells compiler what to do on exception i coroutine
        // Just propagates the error
        void unhandled_exception() { throw; }

        const T& value() const noexcept { return *_current_value; }  // called by Iterator to extract the value

        private:
            // Stores the most recently yielded value
            std::optional<T> _current_value;
    };

    // Memory handling

    // Creates a shorter alias for the standard coroutine handle type
    // Allows the outside wrapper to interact with the hidden coroutine state
    using Handle = std::coroutine_handle<promise_type>;
    Generator() noexcept = default;
    explicit Generator(Handle h) noexcept : _handle(h) {}

    // Prevents copying to avoid double-freeing memory.
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;

    // Transfers ownership of the handle from 'other' to this generator
    Generator(Generator&& other) noexcept
    {
        std::swap(_handle, other._handle);
    }

    // Cleans up existing memory, then transfers ownership
    Generator& operator=(Generator&& other) noexcept
    {
        std::swap(_handle, other._handle);
        return *this;
    }
    ~Generator() { if (_handle) _handle.destroy(); }

    // Enables the Generator to be used in range-based for loops.
    struct iterator
    {
        // Standard boilerplate required by C++ to define an Input Iterator.
        using value_type = T;
        using reference = const T&;
        using pointer = const T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::input_iterator_tag;

        iterator() = default;
        explicit iterator(Handle h) noexcept : _handle{h} {}

        // Unpauses the coroutine to calculate the next value
        iterator& operator++()
        {
            if (_handle && !_handle.done()) _handle.resume();
            return *this;
        }
        void operator++(int) { ++(*this); }

        // Reads the generated value from the promise
        reference operator*() const noexcept { return _handle.promise().value(); }
        pointer operator->() const noexcept { return &_handle.promise().value(); }

        // End condition
        bool operator==(const iterator&) const noexcept = default;
        bool operator==(std::default_sentinel_t) const noexcept
        {
            return !_handle || _handle.done();
        }

    private:
        Handle _handle{};
    };

    // Range functions required by C++ to start and stop the loop.
    iterator begin()
    {
        // Because the coroutine starts paused, it must resume
        // once right now so it calculates the very first value.
        if (_handle) _handle.resume();  // run until the first co_yield
        return iterator{_handle};
    }

    // Returns a generic "end" marker that operator== will check against
    static std::default_sentinel_t end() noexcept { return {}; }

private:
    Handle _handle{};
};

#endif  // MINIPROJECT_GENERATOR_HPP
