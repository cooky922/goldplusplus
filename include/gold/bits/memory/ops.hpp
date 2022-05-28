// <gold/bits/memory/ops.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MEMORY_OPS_HPP
#define __GOLD_BITS_MEMORY_OPS_HPP

#include <initializer_list>
#include <bits/stl_construct.h>

namespace gold {

    /// construct_at
    template <typename T, typename... Args>
    constexpr T* construct_at(T* ptr, Args&&... args)
        noexcept(noexcept(std::construct_at(ptr, std::forward<Args>(args)...)))
    {
        return std::construct_at(ptr, std::forward<Args>(args)...);
    }

    template <typename T, typename U, typename... Args>
    constexpr T* construct_at(T* ptr, std::initializer_list<U> il, Args&&... args)
        noexcept(noexcept(std::construct_at(ptr, il, std::forward<Args>(args)...)))
    {
        return std::construct_at(ptr, il, std::forward<Args>(args)...);
    }

    /// relocate_at
    template <typename T>
    constexpr void relocate_at(T* from, T* to)
        noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>)
    {
        // precondition: both 'from' and 'to' must not be NULL
        std::construct_at(to, std::move(*from));
        std::destroy_at(from);
    }

    /// destroy_at
    template <typename T>
    constexpr void destroy_at(T* ptr) noexcept(std::is_nothrow_destructible_v<T>) {
        std::destroy_at(ptr);
    }

} // namespace gold

#endif // __GOLD_BITS_MEMORY_OPS_HPP
