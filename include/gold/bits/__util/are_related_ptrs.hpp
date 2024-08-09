// <gold/bits/__util/are_related_ptrs.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_ARE_RELATED_PTRS_HPP
#define __GOLD_BITS_UTIL_ARE_RELATED_PTRS_HPP

namespace gold::__util {

    /// __util::are_related_ptrs
    // note: this function can only be used in consteval
    template <typename T>
        requires requires (T* ptr) { ptr - ptr; }
    consteval bool are_related_ptrs(T* lhs, T* rhs) noexcept {
        return __builtin_constant_p(lhs - rhs);
    }

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_ARE_RELATED_PTRS_HPP
