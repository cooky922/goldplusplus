// <gold/bits/casts.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CASTS_HPP
#define __GOLD_BITS_CASTS_HPP

#include <concepts>
#include <bits/move.h>
#include <gold/bits/type_traits/conditional.hpp>

namespace gold {

    /// to_unsigned
    [[gnu::always_inline]]
    inline constexpr auto to_unsigned(std::integral auto op) noexcept {
        return static_cast<std::make_unsigned_t<decltype(op)>>(op);
    }

    /// to_signed
    [[gnu::always_inline]]
    inline constexpr auto to_signed(std::integral auto op) noexcept {
        return static_cast<std::make_signed_t<decltype(op)>>(op);
    }

    /// as_consteval
    [[gnu::always_inline]]
    consteval auto as_consteval(const auto& arg) noexcept { return arg; }

    /// as_mutable [ be careful! ]
    template <typename T>
    [[gnu::always_inline]]
    inline constexpr T& as_mutable(const T& op) noexcept {
        return const_cast<T&>(op);
    }

    constexpr void as_mutable(const auto&&) = delete;

    /// as_const
    template <typename T>
    [[gnu::always_inline]]
    inline constexpr const T& as_const(T& op) noexcept { return op; }

    constexpr void as_const(const auto&&) = delete;

    /// decay_copy
    template <typename T>
        requires std::copy_constructible<std::decay_t<T>>
    [[nodiscard, gnu::always_inline]]
    inline constexpr auto decay_copy(T&& t)
        noexcept(std::is_nothrow_convertible_v<T, std::decay_t<T>>)
    {
        return auto(std::forward<T>(t));
    }

    /// decay_move
    template <typename T>
        requires std::move_constructible<std::decay_t<T>>
    [[nodiscard, gnu::always_inline]]
    inline constexpr auto decay_move(T&& t)
        noexcept(std::is_nothrow_convertible_v<T, std::decay_t<T>>)
    {
        return auto(std::move(t));
    }

} // namespace gold

#endif // __GOLD_BITS_CASTS_HPP
