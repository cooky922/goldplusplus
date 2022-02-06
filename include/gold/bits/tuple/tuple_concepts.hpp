// <gold/bits/tuple/tuple_concepts.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLE_CONCEPTS_HPP
#define __GOLD_BITS_TUPLE_CONCEPTS_HPP

#include <tuple>
#include <gold/bits/tuple/tuple_get.hpp>

namespace gold {

    namespace __detail {

        template <typename T, std::size_t N>
        inline constexpr bool is_tuple_elem_ = requires (T t) {
            typename std::tuple_element_t<N - 1, std::remove_const_t<T>>;
            { tuples::get<N - 1>(t) } -> std::convertible_to<std::tuple_element_t<N - 1, T>&>;
        } && is_tuple_elem_<T, N - 1>;

        template <typename T>
        inline constexpr bool is_tuple_elem_<T, 0> = true;

    } // namespace __detail

    /// convertible_to_tuple
    template <typename T, typename... Ts>
    concept convertible_to_tuple = std::convertible_to<T, std::tuple<Ts...>>;

    /// qualified_tuple
    template <typename T>
    concept qualified_tuple = (!std::is_reference_v<T> && requires {
        typename std::tuple_size<T>::type;
        requires std::same_as<decltype(std::tuple_size_v<T>), std::size_t>;
    } && __detail::is_tuple_elem_<T, std::tuple_size_v<T>>)
      || std::is_array_v<std::remove_cvref_t<T>>;

    /// qualified_pair
    template <typename T>
    concept qualified_pair = qualified_tuple<T> && (std::tuple_size_v<T> == 2);

    /// decomposable
    template <typename T>
    concept decomposable =
        (std::is_standard_layout_v<T> && !std::is_empty_v<T>)
        || std::is_array_v<std::remove_reference_t<T>>
        || qualified_tuple<T>;
}

#endif // __GOLD_BITS_TUPLE_CONCEPTS_HPP
