// <gold/bits/type_traits/type_properties.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TYPE_PROPERTIES
#define __GOLD_BITS_TYPE_TRAITS_TYPE_PROPERTIES

#include <gold/bits/type_traits/helper_class.hpp>

namespace gold {

    //// Type Properties
    /// is_complete_type
    template <typename T>
    struct is_complete_type : std::bool_constant<
        requires { { sizeof(T) }; }
    > {};

    /// is_complete_type_v
    template <typename T>
    inline constexpr bool is_complete_type_v = is_complete_type<T>::value;

    //// Type Property Queries
    /// pointer_rank
    template <typename T>
    struct pointer_rank : size_constant<0> {};

    template <typename T>
    struct pointer_rank<T*> : size_constant<pointer_rank<T>::value + 1> {};

    /// pointer_rank_v
    template <typename T>
    inline constexpr auto pointer_rank_v = pointer_rank<T>::value;

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_TYPE_PROPERTIES
