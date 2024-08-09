// <gold/bits/__util/type_identity.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_TYPE_IDENTITY_HPP
#define __GOLD_BITS_UTIL_TYPE_IDENTITY_HPP

namespace gold::__util {

    /// type_identity
    template <typename T>
    struct type_identity {
        using type = T;
    };

    /// type_identity_t
    template <typename T>
    using type_identity_t = type_identity<T>::type;

    /// non_deduced_t
    template <typename T>
    using non_deduced_t = type_identity<T>::type;

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_TYPE_IDENTITY_HPP
