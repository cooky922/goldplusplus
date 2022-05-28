// <gold/bits/casts.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CASTS_HPP
#define __GOLD_BITS_CASTS_HPP

#include <concepts>
#include <bits/move.h>
#include <gold/bits/type_traits/type_transformations.hpp>

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

    /// unmove [ dangerous! ]
    template <typename T>
    [[nodiscard, deprecated("'gold::unmove' is dangerous")]]
    constexpr std::remove_reference_t<T>& unmove(T&& t) { return t; }

    /// unforward [ dangerous! ]
    template <typename T>
        requires std::constructible_from<std::remove_reference_t<T>, T&&>
    [[nodiscard, deprecated("'gold::unforward' is dangerous")]]
    constexpr std::remove_reference_t<T>&
    unforward(T&& t)
        noexcept(std::is_nothrow_constructible_v<std::remove_reference_t<T>, T&&>)
    {
        return std::forward<T>(t);
    }

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

    namespace __utility {

        template <typename T, typename U>
        using forwarded_like_type = std::conditional_t<
            std::is_lvalue_reference_v<T>,
            std::remove_reference_t<U>&,
            std::remove_reference_t<U>&&
        >;

    } // namespace __utility

    /// forward_like
    template <typename T, typename U>
    [[gnu::always_inline]]
    inline constexpr __utility::forwarded_like_type<T, U> forward_like(U&& arg) noexcept {
        return std::forward<__utility::forwarded_like_type<T, U>>(std::forward<U>(arg));
    }

} // namespace gold

#endif // __GOLD_BITS_CASTS_HPP
