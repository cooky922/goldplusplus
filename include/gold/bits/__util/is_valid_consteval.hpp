// <gold/bits/__util/is_valid_consteval.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_IS_VALID_CONSTEVAL_HPP
#define __GOLD_BITS_UTIL_IS_VALID_CONSTEVAL_HPP

#include <bits/invoke.h>
#include <bits/move.h>

namespace gold::__util {

    /// __util::is_valid_consteval
    // note: this function can only be used in consteval
    template <typename F, typename... Args>
    consteval bool is_valid_consteval(F&& f, Args&&... args) noexcept {
        return __builtin_constant_p((std::__invoke(std::forward<F>(f), std::forward<Args>(args)...), true));
    }

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_IS_VALID_CONSTEVAL_HPP
