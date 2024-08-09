// <gold/bits/to_integer.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TO_INTEGER_HPP
#define __GOLD_BITS_TO_INTEGER_HPP

#include <concepts>
#include <string_view>
#include <gold/char_class>
#include <gold/bits/unreachable.hpp>

namespace gold::__ext {

    /// __ext::to_integer
    template <std::integral T = int>
    constexpr T to_integer(std::string_view sv, std::size_t base = 10) {
        T result = 0;
        for (auto c : sv) {
            if (c == '\'')
                continue;
            result *= base;
            result += [&] {
                if (gold::char_class::is_digit(c))
                    return c - '0';
                else if (gold::char_class::is_alpha(c))
                    return gold::char_class::to_upper(c) - 'A' + 10;
                gold::unreachable();
            }();
        }
        return result;
    }

    /// __ext::to_integer_with_prefix
    template <std::integral T = int>
    constexpr T to_integer_with_prefix(std::string_view sv) {
        if (sv.starts_with("0x") || sv.starts_with("0X")) {
            sv.remove_prefix(2);
            return to_integer<T>(sv, 16);
        } else if (sv.starts_with("0b") || sv.starts_with("0B")) {
            sv.remove_prefix(2);
            return to_integer<T>(sv, 2);
        } else if (sv.starts_with('0')) {
            sv.remove_prefix(1);
            return to_integer<T>(sv, 8);
        } else {
            return to_integer<T>(sv);
        }
    }

} // gold::__ext

#endif // __GOLD_BITS_TO_INTEGER_HPP
