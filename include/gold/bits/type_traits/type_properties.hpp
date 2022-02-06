// <gold/bits/type_traits/type_properties.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TYPE_PROPERTIES
#define __GOLD_BITS_TYPE_TRAITS_TYPE_PROPERTIES

#include <gold/bits/type_traits/helper_class.hpp>

namespace gold {

    //// Type Properties
    /// is_complete_type_v
    template <typename T>
    inline constexpr bool is_complete_type_v = requires { { sizeof(T) }; };

    /// is_complete_type
    template <typename T>
    using is_complete_type = std::bool_constant<is_complete_type_v<T>>;

    namespace __detail {

        template <auto> struct nttp_test_ {};

    } // namespace __detail

    /// is_structural_v
    template <typename T>
    inline constexpr bool is_structural_v = requires { []<T N>(__detail::nttp_test_<N>){}; };

    /// is_structural
    template <typename T>
    using is_structural = std::bool_constant<is_structural_v<T>>;

    //// Type Property Queries
    /// pointer_rank_v
    template <typename T>
    inline constexpr std::size_t pointer_rank_v = 0;

    template <typename T>
    inline constexpr std::size_t pointer_rank_v<T*> = pointer_rank_v<T> + 1;

    /// pointer_rank
    template <typename T>
    using pointer_rank = size_constant<pointer_rank_v<T>>;

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_TYPE_PROPERTIES
