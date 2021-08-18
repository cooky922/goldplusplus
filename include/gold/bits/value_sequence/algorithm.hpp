// <gold/bits/value_sequence/algorithm.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_VALUE_SEQUENCE_ALGO_HPP
#define __GOLD_BITS_VALUE_SEQUENCE_ALGO_HPP

#include <gold/bits/value_sequence/primitives.hpp>

namespace gold {

    namespace __detail {

        // value_sequence_at_impl_
        template <std::size_t, typename>
        struct value_sequence_at_impl_ {};

        template <std::size_t I, auto Head, auto... Tail>
        struct value_sequence_at_impl_<I, hetero_value_sequence<Head, Tail...>>
        : value_sequence_at_impl_<I - 1, hetero_value_sequence<Head, Tail...>> {};

        template <auto Head, auto... Tail>
        struct value_sequence_at_impl_<0, hetero_value_sequence<Head, Tail...>> {
            inline static constexpr auto value = Head;
        };

        template <std::size_t I, typename T, T... Is>
        struct value_sequence_at_impl_<I, homo_value_sequence<T, Is...>>
        : value_sequence_at_impl_<I, hetero_value_sequence<Is...>> {};

        template <std::size_t I, typename T, T... Is>
        struct value_sequence_at_impl_<I, std::integer_sequence<T, Is...>>
        : value_sequence_at_impl_<I, hetero_value_sequence<Is...>> {};

        template <std::size_t I, typename T, T... Fs>
        struct value_sequence_at_impl_<I, floating_point_sequence<T, Fs...>>
        : value_sequence_at_impl_<I, hetero_value_sequence<Fs...>> {};


        // value_sequence_offset_impl_
        template <auto, typename>
        struct value_sequence_offset_impl_ {};

        template <auto N, auto... Is>
        struct value_sequence_offset_impl_<N, hetero_value_sequence<Is...>> {
            using type = hetero_value_sequence<N + Is...>;
        };

        template <auto N, typename T, T... Is>
        struct value_sequence_offset_impl_<N, homo_value_sequence<T, Is...>> {
            using type = homo_value_sequence<T, N + Is...>;
        };

        template <auto N, typename T, T... Is>
        struct value_sequence_offset_impl_<N, std::integer_sequence<T, Is...>> {
            using type = std::integer_sequence<T, N + Is...>;
        };

        template <auto N, typename T, T... Is>
        struct value_sequence_offset_impl_<N, floating_point_sequence<T, Is...>> {
            using type = floating_point_sequence<T, N + Is...>;
        };

        // value_sequence_concat_impl_
        template <typename...>
        struct value_sequence_concat_impl_ {};

        template <auto... Is>
        struct value_sequence_concat_impl_<hetero_value_sequence<Is...>> {
            using type = hetero_value_sequence<Is...>;
        };

        template <auto... Is_0, auto... Is_1, typename... Ts_more>
        struct value_sequence_concat_impl_<
            hetero_value_sequence<Is_0...>, hetero_value_sequence<Is_1...>, Ts_more...
        > {
            using type = typename value_sequence_concat_impl_<
                hetero_value_sequence<Is_0..., Is_1...>, Ts_more...
            >::type;
        };

        template <typename T, T... Is>
        struct value_sequence_concat_impl_<homo_value_sequence<T, Is...>> {
            using type = homo_value_sequence<T, Is...>;
        };

        template <typename T, T... Is_0, T... Is_1, typename... Ts_more>
        struct value_sequence_concat_impl_<
            homo_value_sequence<T, Is_0...>, homo_value_sequence<T, Is_1...>, Ts_more...
        > {
            using type = typename value_sequence_concat_impl_<
                homo_value_sequence<T, Is_0..., Is_1...>, Ts_more...
            >::type;
        };

        template <typename T, T... Is>
        struct value_sequence_concat_impl_<std::integer_sequence<T, Is...>> {
            using type = std::integer_sequence<T, Is...>;
        };

        template <typename T, T... Is_0, T... Is_1, typename... Ts_more>
        struct value_sequence_concat_impl_<
            std::integer_sequence<T, Is_0...>, std::integer_sequence<T, Is_1...>, Ts_more...
        > {
            using type = typename value_sequence_concat_impl_<
                std::integer_sequence<T, Is_0..., Is_1...>, Ts_more...
            >::type;
        };

        template <typename T, T... Is>
        struct value_sequence_concat_impl_<floating_point_sequence<T, Is...>> {
            using type = floating_point_sequence<T, Is...>;
        };

        template <typename T, T... Is_0, T... Is_1, typename... Ts_more>
        struct value_sequence_concat_impl_<
            floating_point_sequence<T, Is_0...>, floating_point_sequence<T, Is_1...>, Ts_more...
        > {
            using type = typename value_sequence_concat_impl_<
                floating_point_sequence<T, Is_0..., Is_1...>, Ts_more...
            >::type;
        };

    } // namespace __detail

    /// value_sequence_at
    template <std::size_t I, typename T>
    inline constexpr auto value_sequence_at = __detail::value_sequence_at_impl_<I, T>::value;

    /// value_sequence_offset
    template <auto N, typename T>
    using value_sequence_offset = typename __detail::value_sequence_offset_impl_<N, T>::type;

    /// value_sequence_concat
    template <typename... Ts>
    using value_sequence_concat = typename __detail::value_sequence_concat_impl_<Ts...>::type;

} // namespace gold

#endif // __GOLD_BITS_VALUE_SEQUENCE_ALGO_HPP
