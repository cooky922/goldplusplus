// <gold/bits/tuple/tuple_conversion.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLE_CONVERSION_HPP
#define __GOLD_BITS_TUPLE_CONVERSION_HPP

#include <gold/bits/tuple/conversion/detail.hpp>

namespace gold {

    /// to_tuple
    template <typename TupleLike>
    constexpr decltype(auto) to_tuple(TupleLike&& t) {
        // solution: create own to get the array size
        using indices_ = std::make_index_sequence<[] consteval -> std::size_t {
            if constexpr (std::is_array_v<std::remove_reference_t<TupleLike>>) {
                return std::extent_v<std::remove_reference_t<TupleLike>>;
            } else {
                return std::tuple_size_v<std::remove_reference_t<TupleLike>>;
            }
        }()>;

        return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            if constexpr (std::is_array_v<std::remove_reference_t<TupleLike>>) {
                return std::make_tuple(std::forward<TupleLike>(t)[Is]...);
            } else if constexpr (qualified_tuple<std::remove_reference_t<TupleLike>>) {
                return std::make_tuple(std::get<Is>(std::forward<TupleLike>(t))...);
            }
        }(indices_{});
    }

    /// to_tuple_ref
    template <typename TupleLike>
    constexpr decltype(auto) to_tuple_ref(TupleLike& t) {
        using indices_ = std::make_index_sequence<[] consteval -> std::size_t {
            if constexpr (std::is_array_v<std::remove_reference_t<TupleLike>>) {
                return std::extent_v<std::remove_reference_t<TupleLike>>;
            } else {
                return std::tuple_size_v<std::remove_reference_t<TupleLike>>;
            }
        }()>;

        return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            if constexpr (std::is_array_v<std::remove_reference_t<TupleLike>>) {
                return std::tie(t[Is]...);
            } else if constexpr (qualified_tuple<std::remove_reference_t<TupleLike>>) {
                return std::tie(std::get<Is>(t)...);
            }
        }(indices_{});

    }

    /// to_variant_at
    template <typename Tuple>
    constexpr decltype(auto) to_variant_at(Tuple&& t, std::size_t i) {
        return __detail::get_to_variant_(std::forward<Tuple>(t), i);
    }

    /// to_variant_ref_at
    template <typename Tuple>
    constexpr decltype(auto) to_variant_ref_at(Tuple& t, std::size_t i) {
        return __detail::get_to_variant_ref_(t, i);
    }

    /// to_plain_array
    template <typename Tuple>
        requires __detail::are_closely_same_v_<std::remove_cvref_t<Tuple>>
    constexpr decltype(auto) to_plain_array(Tuple&& t) {
        /// example:
        /// std::tuple<int, int, int> into std::array<int, 3>
        /// if and only if all types are the same (ignoring cvref qualifiers)
        return std::apply([]<typename... Ts>(Ts&&... args){
            return std::array{std::forward<Ts>(args)...};
        }, std::forward<Tuple>(t));
    }


    /// to_array
    template <typename Tuple>
    constexpr decltype(auto) to_array(Tuple&& t) {
        using ret_type = __detail::to_array_type_t_<std::remove_cvref_t<Tuple>>;
        return [&]<std::size_t... Is>(std::index_sequence<Is...>) -> ret_type {
            return { __detail::get_to_variant_(std::forward<Tuple>(t), Is)... };
        }(std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
    }

    /// to_array_ref [[status: incomplete]]
    template <typename Tuple>
    constexpr decltype(auto) to_array_ref(Tuple& t) {
        using ret_type = __detail::to_array_type_t_<std::remove_cvref_t<Tuple>>;
        return [&]<std::size_t... Is>(std::index_sequence<Is...>) -> ret_type {
            return { __detail::get_to_variant_ref_(t, Is)... };
        }(std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
    }

    /// to_array_variant
    template <typename Tuple>
    constexpr decltype(auto) to_array_variant(Tuple&& t) {
        return to_array(std::forward<Tuple>(t));
    }

    /// to_array_any
    template <typename Tuple>
    constexpr decltype(auto) to_array_any(Tuple&& t) {
        return std::apply([]<typename... Ts>(Ts&&... args) {
            return std::array<
                std::any, std::tuple_size_v<std::remove_reference_t<Tuple>>
            >{ std::forward<Ts>(args)... };
        }, std::forward<Tuple>(t));
    }
}

#endif // __GOLD_BITS_TUPLE_CONVERSION_HPP
