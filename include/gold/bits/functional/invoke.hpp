// <gold/bits/functional/invoke.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FUNCTIONAL_INVOKE_HPP
#define __GOLD_BITS_FUNCTIONAL_INVOKE_HPP

#include <type_traits>
#include <optional>
#include <bits/refwrap.h>
#include <bits/invoke.h>
#include <gold/bits/void_like.hpp>

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

    /// try_invoke
    template <typename F, typename... Args>
    constexpr auto try_invoke(F&& f, Args&&... args) noexcept {

        using result_type = std::invoke_result_t<F, Args...>;

        using wrapped_result_type = typename decltype([]{
            if constexpr (std::is_void_v<result_type>)
                return std::type_identity<std::optional<gold::void_like>>{};
            else if constexpr (std::is_lvalue_reference_v<result_type>)
                return std::type_identity<
                    std::optional<
                        std::reference_wrapper<
                            std::remove_reference_t<result_type>
                        >
                    >
                >{};
            else
                return std::type_identity<std::optional<result_type>>{};
        }())::type;

        wrapped_result_type result = std::nullopt;

        try {
            if constexpr (std::is_void_v<result_type>) {
                gold::invoke_r<void>(std::forward<F>(f), std::forward<Args>(args)...);
                result.emplace();
            } else {
                result.emplace(gold::invoke(std::forward<F>(f), std::forward<Args>(args)...));
            }
        } catch (...) {}

        return result;
    }

} // namespace gold

#endif // __GOLD_BITS_FUNCTIONAL_INVOKE_HPP
