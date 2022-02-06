// <gold/bits/value_sequence/algorithm.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_VALUE_SEQUENCE_ALGO_HPP
#define __GOLD_BITS_VALUE_SEQUENCE_ALGO_HPP

#include <gold/bits/value_sequence/algorithm/impl.hpp>

namespace gold::value_sequence {

    /// element_at
    template <std::size_t I, typename T>
    inline constexpr auto element_at = __detail::at_impl_<I, T>::value;

    /// front
    template <typename T>
    inline constexpr auto front = element_at<0, T>;

    /// back
    template <typename T>
    inline constexpr auto back = element_at<T::size - 1, T>;

    /// offset
    template <auto N, typename T>
    using offset = typename __detail::offset_impl_<N, T>::type;

    /// scale
    template <auto N, typename T>
    using scale = typename __detail::scale_impl_<N, T>::type;

    /// concat
    template <typename T, typename... Ts>
    using concat = typename __detail::concat_impl_<T, Ts...>::type;

    /// sum
    template <typename T>
    inline constexpr auto sum = __detail::sum_impl_<T>::value;

    /// product
    template <typename T>
    inline constexpr auto product = __detail::product_impl_<T>::value;

    /// partial_sum
    /// partial_product

    /// min


    /// max

    /// reverse


    /// sort

} // namespace gold::value_sequence

#endif // __GOLD_BITS_VALUE_SEQUENCE_ALGO_HPP
