// <gold/bits/type_traits/is_ordering_type_v.hpp> - gold++ library

// Copyright (C) [ 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_IS_ORDERING_TYPE_V_HPP
#define __GOLD_BITS_TYPE_TRAITS_IS_ORDERING_TYPE_V_HPP

namespace std {
    class strong_ordering;
    class weak_ordering;
    class partial_ordering;
}

namespace gold {

    /// is_ordering_type_v
    template <typename T>
    inline constexpr bool is_ordering_type_v = false;

    template <>
    inline constexpr bool is_ordering_type_v<std::strong_ordering> = true;

    template <>
    inline constexpr bool is_ordering_type_v<std::weak_ordering> = true;

    template <>
    inline constexpr bool is_ordering_type_v<std::partial_ordering> = true;

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_IS_ORDERING_TYPE_V_HPP
