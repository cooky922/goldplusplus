// <gold/bits/__util/is_ptr_in_range.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_IS_PTR_IN_RANGE_HPP
#define __GOLD_BITS_UTIL_IS_PTR_IN_RANGE_HPP

namespace gold::__util {

    /// __util::is_ptr_in_range
    // note: this function can only be used in consteval
    template <typename T>
    consteval bool is_ptr_in_range(T* begin_ptr, T* end_ptr, T* ptr) noexcept {
        if (!__builtin_constant_p(*begin_ptr) || !__builtin_constant_p(*ptr))
            return false;
        if (!__builtin_constant_p(begin_ptr <= end_ptr))
            return false;
        if (!__builtin_constant_p(begin_ptr <= ptr) || !__builtin_constant_p(ptr < end_ptr))
            return false;
        return begin_ptr <= end_ptr
            && begin_ptr <= ptr
            && ptr < end_ptr;
    }

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_IS_PTR_IN_RANGE_HPP
