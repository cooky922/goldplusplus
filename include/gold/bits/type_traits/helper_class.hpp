// <gold/bits/type_traits/helper_class.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_HELPER_CLASS_HPP
#define __GOLD_BITS_TYPE_TRAITS_HELPER_CLASS_HPP

#include <type_traits>

namespace gold {

    /// types_inherited
    template <typename... Ts>
    struct types_inherited : public Ts... {};

    /// size_constant
    template <std::size_t N>
    using size_constant = std::integral_constant<std::size_t, N>;

    /// size_constant_v
    template <std::size_t N>
    inline constexpr size_constant<N> size_constant_v {};

    /// index_constant
    template <std::size_t N>
    using index_constant = std::integral_constant<std::size_t, N>;

    /// index_constant_v
    template <std::size_t N>
    inline constexpr index_constant<N> index_constant_v {};

    /// always_true
    template <typename...>
    inline constexpr bool always_true = true;

    /// always_false
    template <typename...>
    inline constexpr bool always_false = false;

    /// udl operator""_ic for index_constant
    inline namespace literals {
        inline namespace index_constant_literals {

            template <char... Digits>
            consteval auto operator""_ic() {
                constexpr auto parse_ = [] consteval -> std::size_t {
                    char digits_[] = { Digits... };
                    auto res_ { 0ull };
                    for (const auto& i : digits_) {
                        res_ *= 10;
                        res_ += i - '0';
                    }
                    return res_;
                };

                return index_constant_v<parse_()>;
            }

        } // namespace index_constant_literals

    } // namespace literals

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_HELPER_CLASS_HPP
