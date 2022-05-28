// <gold/bits/non_type.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_NON_TYPE_HPP
#define __GOLD_BITS_NON_TYPE_HPP

namespace gold {

    /// non_type_t
    template <auto N>
    struct non_type_t {
        /// ctor
        explicit constexpr non_type_t() noexcept = default;

        /// value [static]
        inline static constexpr auto value = N;

        /// type [type-member]
        using type = decltype(N);

        /// operator type
        constexpr operator type() const noexcept { return value; }
    };

    /// non_type
    template <auto N>
    inline constexpr non_type_t<N> non_type {};

} // namespace gold

#endif // __GOLD_BITS_NON_TYPE_HPP
