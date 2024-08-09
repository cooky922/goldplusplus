// <gold/bits/tuples/get.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_GET_HPP
#define __GOLD_BITS_TUPLES_GET_HPP

#include <gold/bits/concepts/class_or_union_or_enum.hpp>
#include <gold/bits/concepts/not_void.hpp>
#include <gold/bits/tuples/tuple_element.hpp>
#include <gold/bits/tuples/tuple_size.hpp>
#include <gold/bits/tuples/array_like.hpp>
#include <gold/bits/non_type.hpp>

namespace gold::tuples {

    namespace __tuples {

        /// tuples::__tuples::empty_tuple_size
        template <typename T>
        concept empty_tuple_size = __tuples::viable_tuple_size<T> && tuples::tuple_size_v<T> == 0;

        /// tuples::__tuples::partially_tuple_like
        template <typename T, std::size_t I>
        concept partially_tuple_like = requires {
            typename gold::non_type_t<tuples::tuple_size_v<T>>;
            typename tuples::tuple_element_t<I, T>;
        };

        /// tuples::__tuples::valid_get_result_of
        template <typename R, typename Tuple, std::size_t I>
        concept valid_get_result_of =
               __concepts::not_void<std::remove_cvref_t<R>>
            && requires {
                { std::declval<tuples::tuple_element_t<I, std::remove_cvref_t<Tuple>>>() } -> std::same_as<R&&>;
            };

    } // namespace __tuples

    namespace __cpo_access {

        /// tuples::__cpo_access::has_member_get
        template <typename T, std::size_t I>
        concept has_member_get = __tuples::partially_tuple_like<T, I>
            && requires (T&& op) {
                { std::forward<T>(op).template get<I>() } noexcept -> __tuples::valid_get_result_of<T, I>;
            };

        /// adl get = deleted
        void get(auto&&) = delete;

        /// tuples::__cpo_access::has_adl_get
        template <typename T, std::size_t I>
        concept has_adl_get =
               __tuples::partially_tuple_like<T, I>
            && __concepts::class_or_union_or_enum<T>
            && requires (T&& op) {
                { get<I>(std::forward<T>(op)) } noexcept -> __tuples::valid_get_result_of<T, I>;
            };

        /// tuples::__cpo_access::has_array_get
        template <typename T, std::size_t I>
        concept has_array_get =
                tuples::array_like<T>
            && (I < __tuples::array_like_extent_v<T>);

        /// tuples::__cpo_access::get_fn
        template <std::size_t I>
        struct get_fn {

            // note: get is always noexcept
            template <typename T, typename U = std::remove_cvref_t<T>>
                requires has_array_get<U, I>
                      || has_member_get<U, I>
                      || has_adl_get<U, I>
            static constexpr decltype(auto) operator()(T&& op) noexcept {
                if constexpr (has_array_get<U, I>)
                    return std::forward<T>(op)[I];
                else if constexpr (has_member_get<U, I>)
                    return std::forward<T>(op).template get<I>();
                else
                    return get<I>(std::forward<T>(op));
            }
        };

    } // namespace __cpo_access

    inline namespace __cpo {

        /// tuples::get
        template <std::size_t I>
        inline constexpr __cpo_access::get_fn<I> get {};

        /// tuples::get_element
        // alias to tuples::get
        template <std::size_t I>
        inline constexpr auto get_element = get<I>;

        /// tuples::get_key
        inline constexpr auto get_key = get_element<0>;

        /// tuples::get_value
        inline constexpr auto get_value = get_element<1>;


    } // inline namespace __cpo

    namespace __tuples {

        /// tuples::__tuples::get_result_t
        template <typename Tuple, std::size_t I>
            requires requires { tuples::get<I>(std::declval<Tuple>()); }
        using get_result_t = decltype(tuples::get<I>(std::declval<Tuple>()));

    } // namespace __tuples

} // namespace gold::tuples

#endif // __GOLD_BITS_TUPLES_GET_HPP
