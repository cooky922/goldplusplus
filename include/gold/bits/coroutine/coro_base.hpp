// <gold/bits/coroutine/coro_base.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_COROUTINE_BASE_HPP
#define __GOLD_BITS_COROUTINE_BASE_HPP

#include <coroutine>
#include <stdexcept>

namespace gold {

    /// always_suspend
    std::suspend_always always_suspend() noexcept { return {}; }

    /// never_suspend
    std::suspend_never never_suspend() noexcept { return {}; }

    /// suspend_maybe
    struct suspend_maybe {
        bool ready;
        explicit suspend_maybe(bool ready_)
        : ready(ready_) {}
        bool await_ready() const noexcept { return ready; }
        void await_suspend(std::coroutine_handle<>) const noexcept {}
        void await_resume() const noexcept {}
    };

    /// suspend_if
    suspend_maybe suspend_if(bool b) noexcept {
        return suspend_maybe{b};
    }

    /// coroutine_error
    class coroutine_error : public std::runtime_error {
      public:
        explicit coroutine_error(const char* s)
        : std::runtime_error(s) {}

        virtual ~coroutine_error() {}
    };

} // namespace gold

#endif // __GOLD_BITS_COROUTINE_BASE_HPP
