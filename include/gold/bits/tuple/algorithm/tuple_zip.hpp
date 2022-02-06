// <gold/bits/tuple/algorithm/tuple_zip.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLE_ALGO_ZIP_HPP
#define __GOLD_BITS_TUPLE_ALGO_ZIP_HPP

#include <gold/bits/type_traits/detection.hpp>
#include <gold/bits/tuple/tuple_util.hpp>
#include <gold/bits/min.hpp>

namespace gold {

    namespace __detail {

        template <typename T>
        using get_ref_wrap_type_t_ = typename T::type;

        template <std::size_t I, typename... Tuple>
        using zip_tuple_at_index_t_ = std::tuple<
            std::conditional_t<
                is_ref_wrapper_v_<std::decay_t<Tuple>>,
                std::reference_wrapper<
                    std::conditional_t<
                        std::is_const_v<gold::detected_or_t<Tuple, get_ref_wrap_type_t_, Tuple>>,
                        const std::tuple_element_t<I, std::decay_t<maybe_unwrap_ref_wrapper_t_<Tuple>>>,
                        std::tuple_element_t<I, std::decay_t<maybe_unwrap_ref_wrapper_t_<Tuple>>>
                    >
                >,
                std::tuple_element_t<I, std::decay_t<maybe_unwrap_ref_wrapper_t_<Tuple>>>
            >...
        >;

        template <std::size_t I, typename... Tuple>
        constexpr zip_tuple_at_index_t_<I, Tuple...> zip_tuple_at_index_(Tuple&&... ts) {
            // return { std::get<I>(std::forward<Tuple>(ts))... };

            return { []<typename T>(T&& t) {
                if constexpr (is_ref_wrapper_v_<std::remove_cvref_t<T>>) {
                    using type_ = std::remove_reference_t<typename T::type>; // reference_wrapper<T>::type
                    if constexpr (std::is_const_v<type_>)
                        return std::cref(std::get<I>(std::forward<T>(t).get()));
                    else
                        return std::ref(std::get<I>(std::forward<T>(t).get()));
                } else
                    return std::get<I>(std::forward<T>(t));
            }(std::forward<Tuple>(ts)) ... };
        }

        template <typename... Tuple, std::size_t... Is>
        constexpr std::tuple<zip_tuple_at_index_t_<Is, Tuple...>...>
        tuple_zip_impl_(std::index_sequence<Is...>, Tuple&&... ts) {
            return { zip_tuple_at_index_<Is>(std::forward<Tuple>(ts)...)... };
        }

    } // namespace __detail

    /// tuple_zip
    template <typename... Tuple> requires (sizeof...(Tuple) > 0)
    constexpr decltype(auto) tuple_zip(Tuple&&... ts) {
        constexpr auto min_size_ = gold::__detail::min_({
            std::tuple_size_v<std::decay_t<__detail::maybe_unwrap_ref_wrapper_t_<Tuple>>>...
        });
        return __detail::tuple_zip_impl_(
            std::make_index_sequence<min_size_>{},
            std::forward<Tuple>(ts)...
        );
    }

} // namespace gold

#endif // __GOLD_BITS_TUPLE_ALGO_ZIP_HPP
