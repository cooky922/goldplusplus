// <gold/bits/type_traits/template_instantiation.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TEMPLATE_INSTANTIATION_HPP
#define __GOLD_BITS_TYPE_TRAITS_TEMPLATE_INSTANTIATION_HPP

#include <gold/bits/type_traits/helper_class.hpp>

namespace gold {

    /// is_template_instantiation_v
    template <typename>
    inline constexpr bool is_template_instantiation_v = false;

    template <template <typename...> typename Tp, typename... Ts>
    inline constexpr bool is_template_instantiation_v<Tp<Ts...>> = true;

    template <template <auto...> typename Tp, auto... Ns>
    inline constexpr bool is_template_instantiation_v<Tp<Ns...>> = true;

    template <template <typename, auto, auto...> typename Tp, typename T, auto N, auto... Ns>
    inline constexpr bool is_template_instantiation_v<Tp<T, N, Ns...>> = true;

    template <template <auto, typename, typename...> typename Tp, auto N, typename T, typename... Ts>
    inline constexpr bool is_template_instantiation_v<Tp<N, T, Ts...>> = true;

    // note: this is extendable

    /// is_template_instantiation
    template <typename T>
    using is_template_instantiation = std::bool_constant<is_template_instantiation_v<T>>;

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_TEMPLATE_INSTANTIATION_HPP
