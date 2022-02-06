// <gold/bits/value_sequence/algorithm/impl.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_VALUE_SEQUENCE_ALGO_IMPL_HPP
#define __GOLD_BITS_VALUE_SEQUENCE_ALGO_IMPL_HPP

#include <gold/pack>
#include <gold/bits/value_sequence/primitives.hpp>

namespace gold::value_sequence::__detail {

    // non_type_wrapper_
    template <auto N>
    struct non_type_wrapper_ {
        inline static constexpr auto value = N;
    };

    // at_impl_
    template <std::size_t, typename>
    struct at_impl_ {};

    template <std::size_t I, auto... Ns>
    struct at_impl_<I, hetero_value_sequence<Ns...>> {
        inline static constexpr auto value = gold::pack_element_at<
            I, non_type_wrapper_<Ns>...
        >::value;
    };

    template <std::size_t I, typename T, T... Is>
    struct at_impl_<I, homo_value_sequence<T, Is...>>
    : at_impl_<I, hetero_value_sequence<Is...>> {};

    template <std::size_t I, typename T, T... Is>
    struct at_impl_<I, std::integer_sequence<T, Is...>>
    : at_impl_<I, hetero_value_sequence<Is...>> {};

    template <std::size_t I, typename T, T... Fs>
    struct at_impl_<I, floating_point_sequence<T, Fs...>>
    : at_impl_<I, hetero_value_sequence<Fs...>> {};

    // offset_impl_
    template <auto, typename>
    struct offset_impl_ {};

    template <auto N, auto... Is>
    struct offset_impl_<N, hetero_value_sequence<Is...>> {
        using type = hetero_value_sequence<N + Is...>;
    };

    template <auto N, typename T, T... Is>
    struct offset_impl_<N, homo_value_sequence<T, Is...>> {
        using type = homo_value_sequence<T, N + Is...>;
    };

    template <auto N, typename T, T... Is>
    struct offset_impl_<N, std::integer_sequence<T, Is...>> {
        using type = std::integer_sequence<T, N + Is...>;
    };

    template <auto N, typename T, T... Is>
    struct offset_impl_<N, floating_point_sequence<T, Is...>> {
        using type = floating_point_sequence<T, N + Is...>;
    };

    // scale_impl_
    template <auto, typename>
    struct scale_impl_ {};

    template <auto N, auto... Is>
    struct scale_impl_<N, hetero_value_sequence<Is...>> {
        using type = hetero_value_sequence<N * Is...>;
    };

    template <auto N, typename T, T... Is>
    struct scale_impl_<N, homo_value_sequence<T, Is...>> {
        using type = homo_value_sequence<T, N * Is...>;
    };

    template <auto N, typename T, T... Is>
    struct scale_impl_<N, std::integer_sequence<T, Is...>> {
        using type = std::integer_sequence<T, N * Is...>;
    };

    template <auto N, typename T, T... Is>
    struct scale_impl_<N, floating_point_sequence<T, Is...>> {
        using type = floating_point_sequence<T, N * Is...>;
    };

    // concat_impl_
    template <typename, typename...>
    struct concat_impl_ {};

    template <auto... Is>
    struct concat_impl_<hetero_value_sequence<Is...>> {
        using type = hetero_value_sequence<Is...>;
    };

    template <auto... Is_0, auto... Is_1, typename... Ts_more>
    struct concat_impl_<
        hetero_value_sequence<Is_0...>, hetero_value_sequence<Is_1...>, Ts_more...
    > {
        using type = typename concat_impl_<
            hetero_value_sequence<Is_0..., Is_1...>, Ts_more...
        >::type;
    };

    template <typename T, T... Is>
    struct concat_impl_<homo_value_sequence<T, Is...>> {
        using type = homo_value_sequence<T, Is...>;
    };

    template <typename T, T... Is_0, T... Is_1, typename... Ts_more>
    struct concat_impl_<
        homo_value_sequence<T, Is_0...>, homo_value_sequence<T, Is_1...>, Ts_more...
    > {
        using type = typename concat_impl_<
            homo_value_sequence<T, Is_0..., Is_1...>, Ts_more...
        >::type;
    };

    template <typename T, T... Is>
    struct concat_impl_<std::integer_sequence<T, Is...>> {
        using type = std::integer_sequence<T, Is...>;
    };

    template <typename T, T... Is_0, T... Is_1, typename... Ts_more>
    struct concat_impl_<
        std::integer_sequence<T, Is_0...>, std::integer_sequence<T, Is_1...>, Ts_more...
    > {
        using type = typename concat_impl_<
            std::integer_sequence<T, Is_0..., Is_1...>, Ts_more...
        >::type;
    };

    template <typename T, T... Is>
    struct concat_impl_<floating_point_sequence<T, Is...>> {
        using type = floating_point_sequence<T, Is...>;
    };

    template <typename T, T... Is_0, T... Is_1, typename... Ts_more>
    struct concat_impl_<
        floating_point_sequence<T, Is_0...>, floating_point_sequence<T, Is_1...>, Ts_more...
    > {
        using type = typename concat_impl_<
            floating_point_sequence<T, Is_0..., Is_1...>, Ts_more...
        >::type;
    };

    // sum_impl_
    template <typename>
    struct sum_impl_ {};

    template <auto... Is>
    struct sum_impl_<hetero_value_sequence<Is...>> {
        inline static constexpr auto value = (Is + ... + 0);
    };

    template <typename T, T... Is>
    struct sum_impl_<homo_value_sequence<T, Is...>> {
        inline static constexpr auto value = (Is + ... + 0);
    };

    template <typename T, T... Is>
    struct sum_impl_<std::integer_sequence<T, Is...>> {
        inline static constexpr auto value = (Is + ... + 0);
    };

    template <typename T, T... Is>
    struct sum_impl_<floating_point_sequence<T, Is...>> {
        inline static constexpr auto value = (Is + ... + 0);
    };

    // product_impl_
    template <typename>
    struct product_impl_ {};

    template <auto... Is>
    struct product_impl_<hetero_value_sequence<Is...>> {
        inline static constexpr auto value = (Is * ... * 1);
    };

    template <typename T, T... Is>
    struct product_impl_<homo_value_sequence<T, Is...>> {
        inline static constexpr auto value = (Is * ... * 1);
    };

    template <typename T, T... Is>
    struct product_impl_<std::integer_sequence<T, Is...>> {
        inline static constexpr auto value = (Is * ... * 1);
    };

    template <typename T, T... Is>
    struct product_impl_<floating_point_sequence<T, Is...>> {
        inline static constexpr auto value = (Is * ... * 1);
    };

    // min_impl_

    // max_impl_


} // namespace gold::value_sequence::__detail

#endif // __GOLD_BITS_VALUE_SEQUENCE_ALGO_IMPL_HPP
