// <gold/bits/functional/invoke.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FUNCTIONAL_INVOKE_HPP
#define __GOLD_BITS_FUNCTIONAL_INVOKE_HPP

#include <concepts>
#include <bits/invoke.h>

namespace gold {

    /// invoke
    template <typename F, typename... Args>
        requires std::invocable<F, Args...>
    constexpr std::invoke_result_t<F, Args...>
    invoke(F&& f, Args&&... args) noexcept(std::is_nothrow_invocable_v<F, Args...>) {
        return std::__invoke(
            std::forward<F>(f),
            std::forward<Args>(args)...
        );
    }

    /// invoke_r
    template <typename R, typename F, typename... Args>
        requires std::is_invocable_r_v<R, F, Args...>
    constexpr R invoke_r(F&& f, Args&&... args)
        noexcept(std::is_nothrow_invocable_r_v<R, F, Args...>) {
        return std::__invoke_r<R>(
            std::forward<F>(f),
            std::forward<Args>(args)...
        );
    }

    /// consteval_invoke
    template <typename F, typename... Args>
        requires std::invocable<F, Args...>
    consteval std::invoke_result_t<F, Args...>
    consteval_invoke(F&& f, Args&&... args) noexcept(std::is_nothrow_invocable_v<F, Args...>) {
        return std::__invoke(
            std::forward<F>(f),
            std::forward<Args>(args)...
        );
    }

    /// consteval_invoke_r
    template <typename R, typename F, typename... Args>
        requires std::is_invocable_r_v<R, F, Args...>
    consteval R consteval_invoke_r(F&& f, Args&&... args)
        noexcept(std::is_nothrow_invocable_r_v<R, F, Args...>) {
        return std::__invoke_r<R>(
            std::forward<F>(f),
            std::forward<Args>(args)...
        );
    }

} // namespace gold

#endif // __GOLD_BITS_FUNCTIONAL_INVOKE_HPP
