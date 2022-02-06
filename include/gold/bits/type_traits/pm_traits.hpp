// <gold/bits/type_traits/pm_traits.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TYPE_PM_TRAITS_HPP
#define __GOLD_BITS_TYPE_TRAITS_TYPE_PM_TRAITS_HPP

#include <type_traits>

namespace gold {

    /// pm_traits
    template <typename T>
    struct pm_traits {};

    template <typename T, typename U>
    struct pm_traits<U T::*> {
        using member_type = U;
        using class_type  = T;
    };

    /// pm_class_type_t
    template <typename T>
    using pm_class_type_t = pm_traits<T>::class_type;

    /// pm_class_type
    template <typename T>
    using pm_class_type = std::type_identity<pm_class_type_t<T>>;

    /// pm_member_type_t
    template <typename T>
    using pm_member_type_t = pm_traits<T>::member_type;

    /// pm_member_type
    template <typename T>
    using pm_member_type = std::type_identity<pm_member_type_t<T>>;

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_TYPE_PM_TRAITS_HPP
