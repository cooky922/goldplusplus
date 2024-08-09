// <gold/bits/type_traits/template_instantiation.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TEMPLATE_INSTANTIATION_HPP
#define __GOLD_BITS_TYPE_TRAITS_TEMPLATE_INSTANTIATION_HPP

#include <gold/bits/type_traits/helper_class.hpp>

namespace gold {

    /// is_template_instantiation_v
    template <typename>
    inline constexpr bool is_template_instantiation_v = false;

    // TP <Type...>
    template <template <typename...> typename Tp, typename... Ts>
    inline constexpr bool is_template_instantiation_v<Tp<Ts...>> = true;

    // TP <Val...>
    template <template <auto...> typename Tp, auto... Ns>
    inline constexpr bool is_template_instantiation_v<Tp<Ns...>> = true;

    // TP <Type, Val, Val...>
    template <template <typename, auto, auto...> typename Tp, typename T, auto N, auto... Ns>
    inline constexpr bool is_template_instantiation_v<Tp<T, N, Ns...>> = true;

    // TP <Val, Typ, Type...>
    template <template <auto, typename, typename...> typename Tp, auto N, typename T, typename... Ts>
    inline constexpr bool is_template_instantiation_v<Tp<N, T, Ts...>> = true;

    // TP <Template <of Type...>...>
    template <template <template <typename...> typename...> typename Tp, template <typename...> typename... Tps>
    inline constexpr bool is_template_instantiation_v<Tp<Tps...>> = true;

    // TP <Template <of Val...>...>
    template <template <template <auto...> typename...> typename Tp, template <auto...> typename... Tps>
    inline constexpr bool is_template_instantiation_v<Tp<Tps...>> = true;

    // TP <Template <of Type...>, Val...>
    template <template <template <typename...> typename, auto, auto...> typename Tp, template <typename...> typename Temp, auto N, auto... Ns>
    inline constexpr bool is_template_instantiation_v<Tp<Temp, N, Ns...>> = true;

    // TP <Template <of Type...>, Type...>
    template <template <template <typename...> typename, typename, typename...> typename Tp, template <typename...> typename Temp, typename T, typename... Ts>
    inline constexpr bool is_template_instantiation_v<Tp<Temp, T, Ts...>> = true;

    // TP <Template <of Val...>, Val...>
    template <template <template <auto...> typename, auto, auto...> typename Tp, template <auto...> typename Temp, auto N, auto... Ns>
    inline constexpr bool is_template_instantiation_v<Tp<Temp, N, Ns...>> = true;

    // TP <Template <of Val...>, Type...>
    template <template <template <auto...> typename, typename, typename...> typename Tp, template <auto...> typename Temp, typename T, typename... Ts>
    inline constexpr bool is_template_instantiation_v<Tp<Temp, T, Ts...>> = true;

    // note: this is extendable

    /// is_template_instantiation
    template <typename T>
    struct is_template_instantiation : std::bool_constant<is_template_instantiation_v<T>> {};

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_TEMPLATE_INSTANTIATION_HPP
