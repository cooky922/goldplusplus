// <gold/bits/memory/ops.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MEMORY_OPS_HPP
#define __GOLD_BITS_MEMORY_OPS_HPP

#include <initializer_list>
#include <type_traits>
#include <new>
#include <bits/move.h>

namespace gold {

    /// construct_at
    template <typename T, typename... Args>
    [[gnu::always_inline]]
    inline constexpr T* construct_at(T* ptr, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<T, Args...>)
    { return ::new (ptr) T (std::forward<Args>(args)...); }

    template <typename T, typename U, typename... Args>
    [[gnu::always_inline]]
    inline constexpr T* construct_at(T* ptr, std::initializer_list<U> il, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>, Args...>)
    { return ::new (ptr) T (il, std::forward<Args>(args)...); }

    /// default_construct_at
    template <typename T>
    [[gnu::always_inline]]
    inline constexpr T* default_construct_at(T* ptr)
        noexcept(std::is_nothrow_default_constructible_v<T>)
    { return ::new (ptr) T; }

    /// aggregate_construct_at
    template <typename T, typename... Args>
    [[gnu::always_inline]]
    inline constexpr T* aggregate_construct_at(T* ptr, Args&&... args)
        noexcept(noexcept(T {std::forward<Args>(args)...}))
    { return ::new (ptr) T {std::forward<Args>(args)...}; }

    /// construct_at_from
    template <typename T, typename F>
    [[gnu::always_inline]]
    inline constexpr T* construct_at_from(T* ptr, F&& f)
    { return ::new (ptr) T (std::forward<F>(f)()); }

    /// relocate_at
    template <typename T>
    [[gnu::always_inline]]
    inline constexpr void relocate_at(T* from, T* to)
        noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>)
    {
        // precondition: both 'from' and 'to' must not be nullptr
        struct guard {
            T* ptr;
            constexpr ~guard() { ptr->~T(); }
        } _ (from);
        ::new (to) T(std::move(*from));
    }

    /// relocate
    template <typename T>
    [[gnu::always_inline]]
    inline constexpr T relocate(T* source)
        noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>)
    {
        // precondition: 'source' must not be nullptr
        T result = std::move(*source);
        source->~T();
        return result;
    }

    /// destroy_at
    template <typename T>
    [[gnu::always_inline]]
    inline constexpr void destroy_at(T* ptr) noexcept(std::is_nothrow_destructible_v<T>) {
        ptr->~T();
    }

} // namespace gold

#endif // __GOLD_BITS_MEMORY_OPS_HPP
