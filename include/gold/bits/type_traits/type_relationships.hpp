// <gold/bits/type_traits/type_relationships.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TYPE_RELATIONSHIPS_HPP
#define __GOLD_BITS_TYPE_TRAITS_TYPE_RELATIONSHIPS_HPP

#include <type_traits>

namespace gold {

    /// is_not_same
    template <typename T, typename U>
    struct is_not_same
    : std::bool_constant<
        std::negation_v<
            std::is_same<T, U>
        >
    > {};

    /// is_not_same_v
    template <typename T, typename U>
    inline constexpr bool is_not_same_v = is_not_same<T, U>::value;

    /// are_same
    template <typename T, typename... Ts>
    struct are_same
    : std::bool_constant<
        std::conjunction_v<
            std::is_same<T, Ts>...
        >
    > {};

    /// are_same_v
    template <typename T, typename... Ts>
    inline constexpr bool are_same_v = are_same<T, Ts...>::value;

    /// are_not_same
    template <typename T, typename... Ts>
    struct are_not_same
    : std::bool_constant<
        std::conjunction_v<
            is_not_same<T, Ts>...
        >
    > {};

    /// are_not_same_v
    template <typename T, typename... Ts>
    inline constexpr bool are_not_same_v = are_not_same<T, Ts...>::value;

    /// is_any
    template <typename T, typename... Ts>
    struct is_any
    : std::bool_constant<
        std::disjunction_v<
            std::is_same<T, Ts>...
        >
    > {};

    /// is_any_v
    template <typename T, typename... Ts>
    inline constexpr bool is_any_v = is_any<T, Ts...>::value;

    /// is_specialization_of
    template <typename, template <typename...> typename>
    struct is_specialization_of : std::false_type {};

    template <template <typename...> typename Primary, typename... Ts>
    struct is_specialization_of<Primary<Ts...>, Primary> : std::true_type {};

    /// is_specialization_of_v
    template <typename T, template <typename...> typename Primary>
    inline constexpr bool is_specialization_of_v = is_specialization_of<T, Primary>::value;

    namespace __detail {
        template <typename From, typename To, typename = void>
        struct is_narrowing_convertible_impl_ : std::true_type {};

        template <typename From, typename To>
        struct is_narrowing_convertible_impl_<From, To, std::void_t<
            decltype(To{std::declval<From>()})
        >> : std::false_type {};

    } // namespace __detail

    /// is_narrowing_convertible
    template <typename From, typename To>
    struct is_narrowing_convertible : __detail::is_narrowing_convertible_impl_<From, To> {};

    /// is_narrowing_convertible_v
    template <typename From, typename To>
    inline constexpr bool is_narrowing_convertible_v = is_narrowing_convertible<From, To>::value;

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_TYPE_RELATIONSHIPS_HPP

