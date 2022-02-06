// <gold/bits/concepts/types.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONCEPTS_TYPES_HPP
#define __GOLD_BITS_CONCEPTS_TYPES_HPP

#include <concepts>

#include <gold/bits/type_traits/type_classification.hpp>
#include <gold/bits/type_traits/type_properties.hpp>

namespace gold {

    /// char_type
    template <typename T>
    concept char_type = is_char_v<T>;

    /// ordering_type
    template <typename T>
    concept ordering_type = is_ordering_type_v<T>;

    /// qualified_numeric
    template <typename T>
    concept qualified_numeric = (std::integral<T> || std::floating_point<T>) && (!char_type<T> && !std::same_as<T, bool>);

    /// complete_type
    template <typename T>
    concept complete_type = is_complete_type_v<T>;

    /// structural
    template <typename T>
    concept structural = is_structural_v<T>;

    /// same_all
    template <typename T, typename... Ts>
    concept same_all = (std::same_as<T, Ts> && ...);

    /// different_from
    template <typename T, typename U>
    concept different_from = !std::same_as<T, U>;

    /// brace_constructible_from
    template <typename T, typename... Args>
    concept brace_constructible_from = std::destructible<T> && requires (Args... args) {
        T{args...};
    };

}

#endif // __GOLD_BITS_CONCEPTS_TYPES_HPP
