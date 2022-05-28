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

    namespace __concepts {

        template <typename T, typename U>
        concept layout_compatible_with_impl = std::is_layout_compatible_v<T, U>;

    } // namespace __concepts

    /// layout_compatible_with
    template <typename T, typename U>
    concept layout_compatible_with =
        __concepts::layout_compatible_with_impl<T, U> &&
        __concepts::layout_compatible_with_impl<U, T>;

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

    /// relocatable
    template <typename T>
    concept relocatable = std::destructible<T> && std::move_constructible<T>;

    /// decayable
    template <typename T>
    concept decayable = requires (T op) {
        auto(op);
        auto{op};
    } && ((std::is_object_v<std::remove_cvref_t<T>> && !std::is_array_v<std::remove_cvref_t<T>>) ?
    (std::copy_constructible<T> || std::move_constructible<T>) : true);

} // namespace gold

#endif // __GOLD_BITS_CONCEPTS_TYPES_HPP
