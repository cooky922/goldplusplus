// <gold/bits/unreachable.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UNREACHABLE_HPP
#define __GOLD_BITS_UNREACHABLE_HPP

namespace gold {

    /// unreachable
    [[noreturn]] constexpr void unreachable() { __builtin_unreachable(); }

} // namespace gold

#endif // __GOLD_BITS_UNREACHABLE_HPP
