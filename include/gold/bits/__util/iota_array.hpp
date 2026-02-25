// <gold/bits/__util/iota_array.hpp> - gold++ library

// Copyright (C) [ 2021 - 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_IOTA_ARRAY_HPP
#define __GOLD_BITS_UTIL_IOTA_ARRAY_HPP

#include <cstddef>

namespace gold::__util {

    /// __util::iota_array
    template <std::size_t N>
    inline constexpr std::size_t iota_array[] { __integer_pack(N) ... };

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_IOTA_ARRAY_HPP
