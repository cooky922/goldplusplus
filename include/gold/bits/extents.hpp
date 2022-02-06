// <gold/bits/extents.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_EXTENTS_HPP
#define __GOLD_BITS_EXTENTS_HPP

#include <cstddef>

namespace gold {

    /// dynamic_extent_t
    struct dynamic_extent_t {
        enum class token_t : bool {};
        constexpr explicit dynamic_extent_t(token_t) noexcept {}
    };

    /// dynamic_extent
    inline constexpr dynamic_extent_t dynamic_extent { dynamic_extent_t::token_t{} };

    /// extent_t
    struct extent_t {
        std::size_t value;
        bool dynamic;

        constexpr extent_t(gold::dynamic_extent_t)
        : value(), dynamic(true) {}

        constexpr extent_t(std::size_t n)
        : value(n), dynamic(false) {}

        constexpr bool operator==(const extent_t&) const noexcept = default;
    };


} // namespace gold

#endif // __GOLD_BITS_EXTENTS_HPP
