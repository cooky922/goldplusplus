// <gold/bits/algo/str_trim.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_STR_TRIM_HPP
#define __GOLD_BITS_ALGO_STR_TRIM_HPP

#include <string_view>
#include <gold/bits/algo/min_max.hpp>

namespace gold::__algo {

    /// __algo::trim_left
    constexpr std::string_view trim_left(std::string_view sv, std::string_view targets = " ") {
        sv.remove_prefix(__algo::min(sv.find_first_not_of(targets), sv.size()));
        return sv;
    }

    /// __algo::trim_right
    constexpr std::string_view trim_right(std::string_view sv, std::string_view targets = " ") {
        sv.remove_suffix(__algo::min(sv.size() - sv.find_last_not_of(targets) - 1, sv.size()));
        return sv;
    }

    /// __algo::trim
    constexpr std::string_view trim(std::string_view sv, std::string_view targets = " ") {
        return trim_right(trim_left(sv, targets), targets);
    }

} // namespace gold::__algo

#endif // __GOLD_BITS_ALGO_STR_TRIM_HPP
