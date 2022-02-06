// <gold/bits/tuple/tuple_get.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLE_GET_HPP
#define __GOLD_BITS_TUPLE_GET_HPP

#include <concepts>
#include <gold/utility>

namespace gold {

    namespace tuples {

        namespace __cust_access {

            template <typename T, std::size_t I>
            concept partially_tuple_like_ = requires {
                typename std::tuple_element<I, std::remove_cvref_t<T>>;
                typename std::tuple_size<std::remove_cvref_t<T>>;
            };

            template <typename T, std::size_t I>
            concept member_get_ = requires (T tup) {
                { gold::decay_copy(tup.template get<I>()) } -> std::convertible_to<std::tuple_element_t<I, std::remove_cvref_t<T>>>;
            } && partially_tuple_like_<T, I>;

            template <std::size_t I>
            void get(auto&) = delete;

            template <std::size_t I>
            void get(const auto&) = delete;

            template <typename T, std::size_t I>
            concept adl_get_ = (std::is_class_v<std::remove_cvref_t<T>> || std::is_enum_v<std::remove_cvref_t<T>>) && requires (T tup) {
                { gold::decay_copy(get<I>(tup)) } -> std::convertible_to<std::tuple_element_t<I, std::remove_cvref_t<T>>>;
            } && partially_tuple_like_<T, I>;

            template <typename T, std::size_t I>
            concept array_get_ = std::is_array_v<std::remove_cvref_t<T>>;

            template <typename T, std::size_t I>
            concept valid_tuple_index_ = [] {
                if constexpr (array_get_<T, I>)
                    return I < std::extent_v<std::remove_cvref_t<T>>;
                else if constexpr (partially_tuple_like_<T, I>)
                    return I < std::tuple_size_v<std::remove_cvref_t<T>>;
                else
                    return false;
            }();

            template <typename T, std::size_t I>
            concept valid_get_arguments_ = []{
                if constexpr (valid_tuple_index_<T, I>)
                    return member_get_<T, I> || adl_get_<T, I> || std::is_array_v<std::remove_cvref_t<T>>;
                else
                    return false;
            }();

            template <std::size_t I>
            struct get_fn_ {

                template <typename T>
                    requires valid_get_arguments_<T, I>
                constexpr decltype(auto) operator()(T&& tup) const noexcept {
                    if constexpr (member_get_<T, I>)
                        return std::forward<T>(tup).template get<I>();
                    else if constexpr (adl_get_<T, I>)
                        return get<I>(std::forward<T>(tup));
                    else if constexpr (std::is_array_v<std::remove_cvref_t<T>>)
                        return std::forward<T>(tup)[I];
                    else
                        static_assert(gold::always_false<T>, "gold::tuples::get is not supported for this type");
                }
            };

        } // namespace __cust_access

        inline namespace __cust {

            template <std::size_t I>
            inline constexpr __cust_access::get_fn_<I> get { };

            /// benefits:
            /// better support for working with tuples
            /// example:
            /// can now use std::apply and std::make_from_tuple for user-defined types

        } // inline namespace __cust

    } // namespace tuples

    /// tuple_like_size_v
    template <typename T>
    inline constexpr std::size_t tuple_like_size_v = [] {
        if constexpr (std::is_array_v<std::remove_cvref_t<T>>)
            return std::extent_v<std::remove_cvref_t<T>>;
        else if constexpr (tuples::__cust_access::partially_tuple_like_<T, 0>)
            return std::tuple_size_v<std::remove_cvref_t<T>>;
    }();


} // namespace gold

#endif // __GOLD_BITS_TUPLE_GET_HPP
