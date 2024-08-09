// <gold/bits/__util/is_valid_ptr.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_IS_VALID_PTR_HPP
#define __GOLD_BITS_UTIL_IS_VALID_PTR_HPP

namespace gold::__util {

    /// __util::is_valid_ptr
    // note: this function can only be used in consteval
    template <typename T>
    consteval bool is_valid_ptr(T* ptr) noexcept {
        return __builtin_constant_p(*ptr);
    }

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_IS_VALID_PTR_HPP
