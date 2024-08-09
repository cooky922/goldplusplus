// <gold/bits/tuples/tuple_element.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_TUPLE_ELEMENT_HPP
#define __GOLD_BITS_TUPLES_TUPLE_ELEMENT_HPP

#include <gold/bits/tuples/tuple_size.hpp>

namespace gold::tuples {

    /// tuples::tuple_element
    template <std::size_t I, typename T>
    struct tuple_element {};

    template <std::size_t I, typename T>
        requires requires { typename tuple_element<I, T>::type; }
    struct tuple_element<I, const T> {
        using type = std::add_const_t<typename tuple_element<I, T>::type>;
    };

    /// tuples::tuple_element<I, RawArray>
    template <std::size_t I, typename T, std::size_t N>
        requires (I < N)
    struct tuple_element<I, T[N]> {
        using type = T;
    };

    namespace __tuples {

        /// tuples::__tuple::tuple_element_impl
        // reduces class template N instantiations into 1
        // where N is the size of tuple-like
        template <typename T>
        struct tuple_element_impl {
            template <std::size_t I>
            using type = std::tuple_element<I, T>::type;
        };

        template <typename T>
            requires requires { typename tuple_element<0, T>::type; }
        struct tuple_element_impl<T> {
            template <std::size_t I>
            using type = tuple_element<I, T>::type;
        };

    } // namespace __tuples

    /// tuples::tuple_element_t
    // for universal metaprogramming, use tuples::tuple_element_t
    // instead of using tuples::tuple_element<T>::type
    template <std::size_t I, typename T>
        requires requires { tuples::tuple_size_v<T>; }
              && (I < tuples::tuple_size_v<T>)
              && ((gold::complete_type<std::tuple_element<I, T>> &&
                   requires { typename std::tuple_element<I, T>::type; })
                   || requires { typename tuple_element<I, T>::type; })
    using tuple_element_t = __tuples::tuple_element_impl<T>::template type<I>;

} // namespace gold::tuples

namespace std {

    /// tuple_element
    template <std::size_t I, typename T>
        requires requires {
            typename gold::tuples::tuple_element<I, T>::type;
        }
    struct tuple_element<I, T> {
        using type = gold::tuples::tuple_element<I, T>::type;
    };

} // namespace std

#endif // __GOLD_BITS_TUPLES_TUPLE_ELEMENT_HPP
