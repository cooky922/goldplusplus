// <gold/bits/tuples/concepts.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_CONCEPTS_HPP
#define __GOLD_BITS_TUPLES_CONCEPTS_HPP

#include <gold/bits/tuples/get.hpp>
#include <bits/stl_pair.h>

namespace gold::tuples {

    /// tuples::gettable_at
    template <typename T, std::size_t I>
    concept gettable_at = requires (T&& op) { tuples::get<I>(std::forward<T>(op)); };

    namespace __tuples {

        /// tuples::__tuples::all_gettable
        template <typename T>
        concept all_gettable =
            __tuples::viable_tuple_size<std::remove_cvref_t<T>> &&
            []<std::size_t... Is>(std::index_sequence<Is...>) {
                return (tuples::gettable_at<T, Is> && ...);
            }(std::make_index_sequence<__tuples::decayed_tuple_size<T>>{});

    } // namespace __tuples

    /// tuples::gettable
    template <typename T, typename U = std::remove_cvref_t<T>>
    concept gettable = !__tuples::empty_tuple_size<U> && __tuples::all_gettable<T>;

    /// tuples::tuple_like
    // note: added std internal '__tuple_like'
    //       so that types like std::tuple, std::pair, or std::array
    //       don't have to be checked again that in fact they're tuple-like
    template <typename T, typename U = std::remove_cvref_t<T>>
    concept tuple_like = std::__tuple_like<U> || __tuples::empty_tuple_size<U> || __tuples::all_gettable<T>;

    /// tuples::tuple_like_for_size
    template <typename T, std::size_t N>
    concept tuple_like_for_size = tuple_like<T> && __tuples::decayed_tuple_size<T> == N;

    /// tuples::empty_like
    template <typename T>
    concept empty_like = std::is_empty_v<T> && tuple_like_for_size<T, 0>;

    /// tuples::single_like
    template <typename T>
    concept single_like = tuple_like_for_size<T, 1>;

    /// tuples::pair_like
    template <typename T>
    concept pair_like = tuple_like_for_size<T, 2>;

} // namespace gold::tuples

#endif // __GOLD_BITS_TUPLES_CONCEPTS_HPP
