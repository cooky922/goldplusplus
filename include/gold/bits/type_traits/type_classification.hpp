// <gold/bits/type_traits/type_classification.hpp> - gold++ library

// Copyright (C) [ 2021 - 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TYPE_CLASSIFICATION_HPP
#define __GOLD_BITS_TYPE_TRAITS_TYPE_CLASSIFICATION_HPP

#include <gold/bits/type_traits/is_character_type_v.hpp>
#include <gold/bits/type_traits/is_ordering_type_v.hpp>
#include <gold/bits/type_traits/is_scalar_numeric_type_v.hpp>

namespace gold {

    /// is_character_type_v [defined in <../is_character_type_v.hpp>]
    /// is_character_type
    template <typename T>
    struct is_character_type : std::bool_constant<is_character_type_v<T>> {};

    /// is_ordering_type_v [defined in <../is_ordering_type_v.hpp>]
    /// is_ordering_type
    template <typename T>
    struct is_ordering_type : std::bool_constant<is_ordering_type_v<T>> {};

    /// is_scalar_numeric_type_v [defined in <../is_scalar_numeric_type_v.hpp>]
    /// is_scalar_numeric_type
    template <typename T>
    struct is_scalar_numeric_type : std::bool_constant<is_scalar_numeric_type_v<T>> {};

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_TYPE_CLASSIFICATION_HPP

