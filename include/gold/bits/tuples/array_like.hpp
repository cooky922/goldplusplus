// <gold/bits/tuples/array_like.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_ARRAY_LIKE_HPP
#define __GOLD_BITS_TUPLES_ARRAY_LIKE_HPP

#include <type_traits>
#include <gold/bits/tuples/fwd.hpp>
#include <gold/bits/type_traits/specialization_of.hpp>
#include <gold/bits/non_type.hpp>
#include <gold/bits/__util/uneval.hpp>

namespace std { template <typename, std::size_t> class array; } // fwd only

namespace gold::tuples {

    /// tuples::enable_array_like<std::array<T, N>>
    template <typename T, std::size_t N>
    inline constexpr bool enable_array_like<std::array<T, N>> = true;

    namespace __tuples {

        /// __tuples::array_like_extent
        template <typename T>
        struct array_like_extent {};

        template <typename T, std::size_t N>
        struct array_like_extent<T[N]> {
            inline static constexpr std::size_t value = N;
        };

        template <typename T, std::size_t N>
        struct array_like_extent<std::array<T, N>> {
            inline static constexpr std::size_t value = N;
        };

        template <typename T>
            requires requires { { gold::non_type_t<T::extent>::value } -> std::convertible_to<std::size_t>; }
                  || requires { { gold::non_type_t<T::size()>::value } -> std::convertible_to<std::size_t>; }
        struct array_like_extent<T> {
            inline static constexpr std::size_t value = [] -> std::size_t {
                if constexpr (requires { typename gold::non_type_t<T::extent>; })
                    return T::extent;
                else
                    return T::size();
            }();
        };

        /// __tuples::array_like_extent_v
        template <typename T>
        inline constexpr std::size_t array_like_extent_v = array_like_extent<T>::value;

        /// __tuples::array_like_element_type
        template <typename T>
        struct array_like_element_type {};

        template <typename T, std::size_t N>
        struct array_like_element_type<T[N]> {
            using type = T;
        };

        template <typename T, std::size_t N>
        struct array_like_element_type<std::array<T, N>> {
            using type = T;
        };

        template <typename T>
            requires (requires { typename T::value_type; }
                  || requires { typename T::element_type; })
                  && requires { std::declval<T>()[0uz]; }
        struct array_like_element_type<T> {
            using type = decltype([]{
                if constexpr (requires { typename T::value_type; })
                    return __util::uneval<typename T::value_type>;
                else
                    return __util::uneval<typename T::element_type>;
            }()());
        };

        template <typename T>
            requires (!requires { typename T::value_type; }
                  && !requires { typename T::element_type; })
                  && requires { { std::declval<T>()[0uz] } noexcept; } // requires to be noexcept
        struct array_like_element_type<T> {
            using type = std::remove_reference_t<decltype(std::declval<T>()[0uz])>;
        };

        /// __tuples::array_like_element_type_t
        template <typename T>
        using array_like_element_type_t = array_like_element_type<T>::type;

    } // namespace __tuples

    /// tuples::array_like
    template <typename T>
    concept array_like =
           std::is_bounded_array_v<T>
        || (enable_array_like<T>
         && requires {
            __tuples::array_like_extent_v<T>;
            typename __tuples::array_like_element_type_t<T>;
         });

    /// tuples::tuple_size<T is array_like but not bounded_array>
    template <typename T>
        requires (!std::is_bounded_array_v<T>)
              && array_like<T>
    struct tuple_size<T> {
        inline static constexpr std::size_t value = __tuples::array_like_extent_v<T>;
    };

    /// tuples::tuple_element<T is array_like but not bounded_array>
    template <std::size_t I, typename T>
        requires (!std::is_bounded_array_v<T>)
              && array_like<T>
    struct tuple_element<I, T> {
        using type = __tuples::array_like_element_type_t<T>;
    };

} // namespace gold::tuples

#endif // __GOLD_BITS_TUPLES_ARRAY_LIKE_HPP
