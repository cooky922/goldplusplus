// <gold/bits/any/fwd.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ANY_FWD_HPP
#define __GOLD_BITS_ANY_FWD_HPP

namespace gold {

    /// view_any [fwd]
    class view_any;

    /// any [fwd]
    class any;

    /// unique_any [fwd]
    class unique_any;

    namespace __any {

        /// __any::base_storage [fwd]
        struct base_storage;

        /// __any::sbo_buffer [fwd]
        struct sbo_buffer;

        /// __any::manager_result [fwd]
        struct manager_result;

    } // namespace __any

    /// in_place_viewable_t
    struct in_place_viewable_t {
        constexpr explicit in_place_viewable_t() noexcept = default;
    };

    /// in_place_viewable
    inline constexpr in_place_viewable_t in_place_viewable {};

} // namespace gold

#endif // __GOLD_BITS_ANY_FWD_HPP
