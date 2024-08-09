// <gold/bits/__util/maybe_present_t.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_MAYBE_PRESENT_T_HPP
#define __GOLD_BITS_UTIL_MAYBE_PRESENT_T_HPP

#include <gold/bits/__util/empty_t.hpp>
#include <gold/bits/type_traits/constant.hpp>

namespace gold::__util {

    /// __util::maybe_present_t
    template <typename T, bool Present = true, auto Tag = 0>
    using maybe_present_t = __type_pack_element<int(Present), __util::empty_temp_t<Tag>, T>;

    /// __util::maybe_present_or_t
    template <typename T, T DefaultConstant, bool Present = true>
    using maybe_present_or_t = __type_pack_element<int(Present), gold::constant_t<DefaultConstant>>;

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_MAYBE_PRESENT_T_HPP
