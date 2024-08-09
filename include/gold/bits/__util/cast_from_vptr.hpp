// <gold/bits/__util/cast_from_vptr.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_CAST_FROM_VPTR_HPP
#define __GOLD_BITS_UTIL_CAST_FROM_VPTR_HPP

#if __cplusplus < 202400L
#error "cast from void* in consteval is not allowed until C++26"
#endif

namespace gold::__util {

    /// __util::cast_from_vptr
    // note: this function can only be used in consteval
    template <typename To>
    consteval To cast_from_vptr(void* ptr) noexcept {
        return static_cast<To>(ptr);
    }

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_CAST_FROM_VPTR_HPP
