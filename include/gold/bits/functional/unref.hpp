// <gold/bits/functional/unref.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FUNCTIONAL_UNREF_HPP
#define __GOLD_BITS_FUNCTIONAL_UNREF_HPP

#include <bits/refwrap.h>

namespace gold {

    /// unref
    // unwrapping reference wrapper object by calling get, otherwise it will be forwarded
    template <typename T>
    constexpr T&& unref(T&& t) {
        return std::forward<T>(t);
    }

    template <typename T>
    constexpr T& unref(std::reference_wrapper<T> t) {
        return t.get();
    }

}

#endif // __GOLD_BITS_FUNCTIONAL_UNREF_HPP
