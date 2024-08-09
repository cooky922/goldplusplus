// <gold/bits/functional/unref.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FUNCTIONAL_UNREF_HPP
#define __GOLD_BITS_FUNCTIONAL_UNREF_HPP

#include <bits/refwrap.h>

namespace gold {

    /// unwrap_ref
    // unwrapping reference wrapper object by calling get, otherwise it will be forwarded
    template <typename T>
    [[gnu::always_inline]]
    constexpr T&& unwrap_ref(T&& t) noexcept {
        return std::forward<T>(t);
    }

    template <typename T>
    [[gnu::always_inline]]
    constexpr T& unwrap_ref(std::reference_wrapper<T> t) noexcept {
        return t.get();
    }

}

#endif // __GOLD_BITS_FUNCTIONAL_UNREF_HPP
