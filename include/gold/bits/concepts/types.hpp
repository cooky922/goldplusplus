// <gold/bits/concepts/types.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

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

    /// complete_type
    template <typename T>
    concept complete_type = is_complete_type_v<T>;

    /// structural_type
    template <typename T>
    concept structural_type = is_structural_v<T>;

    /// qualified_numeric
    template <typename T>
    concept qualified_numeric = (std::integral<T> || std::floating_point<T>) && (!char_type<T> && !std::same_as<T, bool>);

    namespace __concepts {

        /// __concepts::layout_compatible_with_impl
        template <typename T, typename U>
        concept layout_compatible_with_impl = std::is_layout_compatible_v<T, U>;

        /// __concepts::different_from
        template <typename T, typename U>
        concept different_from = !std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

    } // namespace __concepts

    /// layout_compatible_with
    template <typename T, typename U>
    concept layout_compatible_with =
        __concepts::layout_compatible_with_impl<T, U> &&
        __concepts::layout_compatible_with_impl<U, T>;

    /// similar_to
    template <typename T, typename U>
    concept similar_to = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

    /// same_all
    template <typename T, typename... Ts>
    concept same_all = (std::same_as<T, Ts> && ...);

    /// different_from
    template <typename T, typename U>
    concept different_from = !std::same_as<T, U>;

    /// brace_constructible_from
    template <typename T, typename... Args>
    concept brace_constructible_from = std::destructible<T> && requires {
        T { std::declval<Args>() ... };
    };

    /// relocatable
    template <typename T>
    concept relocatable = std::destructible<T> && std::move_constructible<T>;

    namespace __concepts {

        /// __concepts::decayable_impl
        template <typename T>
        concept decayable_impl = std::copy_constructible<T> || std::move_constructible<T>;

    } // namespace __concepts

    /// decayable
    template <typename T>
    concept decayable = std::convertible_to<T, std::decay_t<T>>
        && __concepts::decayable_impl<std::decay_t<T>>
        && requires {
            auto (std::declval<T>());
            auto {std::declval<T>()};
        };

    namespace __concepts {

        /// __concepts::static_castable_to
        template <typename From, typename To>
        concept static_castable_to = requires { static_cast<To>(std::declval<From>()); };

    } // namespace __concepts

    /// implicitly_convertible_to
    // considers only implicit conversions
    template <typename From, typename To>
    concept implicitly_convertible_to =
           std::is_convertible_v<From, To>
        && requires { std::declval<void(&)(To)>()(std::declval<From>()); };

    /// explicitly_convertible_to
    // considers only explicit conversions and does not allow implicit conversions
    template <typename From, typename To>
    concept explicitly_convertible_to =
          !implicitly_convertible_to<From, To>
        && __concepts::static_castable_to<From, To>;

    /// trivially_destructible
    template <typename T>
    concept trivially_destructible =
           std::destructible<T>
        && std::is_trivially_destructible_v<T>;

    /// trivially_default_initializable
    template <typename T>
    concept trivially_default_initializable =
           std::default_initializable<T>
        && std::is_trivially_default_constructible_v<T>;

    /// trivially_move_constructible
    template <typename T>
    concept trivially_move_constructible =
           std::move_constructible<T>
        && std::is_trivially_move_constructible_v<T>;

    /// trivially_copy_constructible
    template <typename T>
    concept trivially_copy_constructible =
           std::copy_constructible<T>
        && std::is_trivially_copy_constructible_v<T>;

    /// trivially_relocatable
    template <typename T>
    concept trivially_relocatable =
           relocatable<T>
        && trivially_destructible<T>
        && trivially_move_constructible<T>;

    /// move_assignable
    template <typename T>
    concept move_assignable =
           std::assignable_from<T&, T>
        && std::swappable<T>
        && std::is_move_assignable_v<T>;

    /// copy_assignable
    template <typename T>
    concept copy_assignable =
           move_assignable<T>
        && std::assignable_from<T&, T&>
        && std::assignable_from<T&, const T&>
        && std::assignable_from<T&, const T>
        && std::is_copy_assignable_v<T>;

    /// trivially_move_assignable
    template <typename T>
    concept trivially_move_assignable =
           move_assignable<T>
        && std::is_trivially_move_assignable_v<T>;

    /// trivially_copy_assignable
    template <typename T>
    concept trivially_copy_assignable =
           copy_assignable<T>
        && std::is_trivially_copy_assignable_v<T>;

    /// trivially_copyable
    template <typename T>
    concept trivially_copyable =
           trivially_move_constructible<T>
        && trivially_copy_constructible<T>
        && trivially_move_assignable<T>
        && trivially_copy_assignable<T>
        && trivially_destructible<T>
        && std::is_trivially_copyable_v<T>;

    /// trivial_type
    template <typename T>
    concept trivial_type =
           trivially_default_initializable<T>
        && trivially_copyable<T>
        && std::is_trivial_v<T>;

} // namespace gold

#endif // __GOLD_BITS_CONCEPTS_TYPES_HPP
