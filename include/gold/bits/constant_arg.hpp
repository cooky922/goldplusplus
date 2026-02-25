// <gold/bits/constant_arg.hpp> - gold++ library

// Copyright (C) [ 2021 - 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONSTANT_ARG_HPP
#define __GOLD_BITS_CONSTANT_ARG_HPP

// TODO:
// renamed 'non_type[_t]' to 'constant_arg[_t]'
// to better reflect its usage as constant template argument

namespace gold {

    /// constant_arg_t
    template <auto N>
    struct constant_arg_t {

        // ctor
        explicit constant_arg_t() noexcept = default;

        // value [static]
        inline static constexpr auto value = N;

        // type [type-member]
        using type = decltype(N);

        // operator type
        constexpr operator type() const noexcept { return value; }

    };

    /// constant_arg
    template <auto N>
    inline constexpr constant_arg_t<N> constant_arg {};

} // namespace gold

#endif // __GOLD_BITS_NON_TYPE_HPP

