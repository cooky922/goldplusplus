// <gold/bits/type_traits/type_relationships.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TYPE_RELATIONSHIPS_HPP
#define __GOLD_BITS_TYPE_TRAITS_TYPE_RELATIONSHIPS_HPP

#include <type_traits>

namespace gold {

    /// is_not_same_v
    template <typename T, typename U>
    inline constexpr bool is_not_same_v = !std::is_same_v<T, U>;

    /// is_not_same
    template <typename T, typename U>
    struct is_not_same : std::bool_constant<is_not_same_v<T, U>> {};

    /// are_same_v
    template <typename T, typename... Ts>
    inline constexpr bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

    /// are_same
    template <typename T, typename... Ts>
    struct are_same : std::bool_constant<are_same_v<T, Ts...>> {};

    /// are_not_same_v
    template <typename T, typename... Ts>
    inline constexpr bool are_not_same_v = std::conjunction_v<is_not_same<T, Ts>...>;

    /// are_not_same
    template <typename T, typename... Ts>
    struct are_not_same : std::bool_constant<are_not_same_v<T, Ts...>> {};

    /// is_any_of_v
    template <typename T, typename... Ts>
    inline constexpr bool is_any_of_v = (std::is_same_v<T, Ts> || ...);

    /// is_any_of
    template <typename T, typename... Ts>
    struct is_any_of : std::bool_constant<is_any_of_v<T, Ts...>> {};

    /// is_convertible_without_narrowing_v
    template <typename From, typename To>
    inline constexpr bool is_convertible_without_narrowing_v =
        __is_convertible(From, To) &&
        requires (From from) { To { from }; };

    /// is_convertible_without_narrowing
    template <typename From, typename To>
    struct is_convertible_without_narrowing : std::bool_constant<is_convertible_without_narrowing_v<From, To>> {};

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_TYPE_RELATIONSHIPS_HPP

