// <gold/bits/__util/empty_t.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_EMPTY_T_HPP
#define __GOLD_BITS_UTIL_EMPTY_T_HPP

namespace gold::__util {

    /// __util::empty_t
    struct empty_t {};

    /// __util::empty_temp_t
    template <auto Tag = 0>
    struct empty_temp_t {};

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_EMPTY_T_HPP
