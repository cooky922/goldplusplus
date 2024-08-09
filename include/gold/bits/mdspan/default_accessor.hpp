// <gold/bits/mdspan/default_accessor.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MDSPAN_DEFAULT_ACCESSOR_HPP
#define __GOLD_BITS_MDSPAN_DEFAULT_ACCESSOR_HPP

#include <type_traits>

namespace gold {

    /// default_accessor
    template <typename T>
    struct default_accessor {
        using offset_policy    = default_accessor;
        using element_type     = T;
        using reference        = element_type&;
        using data_handle_type = element_type*;

        constexpr default_accessor() noexcept = default;

        template <typename U>
            requires std::is_convertible_v<U(*)[], T(*)[]>
        [[gnu::always_inline]]
        constexpr default_accessor(default_accessor<U>) noexcept {}

        [[gnu::always_inline]]
        constexpr reference access(data_handle_type p, std::size_t i) const noexcept {
            return p[i];
        }

        [[gnu::always_inline]]
        constexpr data_handle_type offset(data_handle_type p, std::size_t i) const noexcept {
            return p + i;
        }
    };

} // namespace gold

#endif // __GOLD_BITS_MDSPAN_DEFAULT_ACCESSOR_HPP
