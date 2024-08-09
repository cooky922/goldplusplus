// <gold/bits/format/fwd.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_FWD_HPP
#define __GOLD_BITS_FORMAT_FWD_HPP

#include <type_traits>

namespace gold {

    /// format_parse_context [fwd decl]
    class format_parse_context;

    /// format_context [fwd decl]
    class format_context;

    /// format_arg [fwd decl]
    class format_arg;

    /// format_args [fwd decl]
    class format_args;

    /// make_format_args [fwd decl]
    template <typename... Args>
    constexpr auto make_format_args(Args&&...) noexcept;

    /// visit_format_arg [fwd decl]
    template <typename F>
    constexpr decltype(auto) visit_format_arg(F&&, format_arg);

    /// format_to_n_result [fwd decl]
    template <typename>
    struct format_to_n_result;

    /// formatter
    // primary template definition is disabled by default
    // specialize one and conform the following requirements
    // to model the concept formattable
    template <typename>
    struct formatter {
        formatter() = delete;
        formatter(const formatter&) = delete;
        formatter& operator=(const formatter&) = delete;
    };

    namespace __format {

        /// __format::sink [fwd decl]
        struct sink;

        /// __format::sink_iterator [fwd decl]
        struct sink_iterator;

        /// __format::format_arg_store [fwd decl]
        template <typename... Args>
        struct format_arg_store;

        /// __format::create_format_context [fwd decl]
        constexpr format_context create_format_context(const format_args*, sink_iterator) noexcept;

        /// __format::create_artificial_format_context [fwd decl]
        constexpr format_context create_artificial_format_context(format_context&, __format::sink_iterator) noexcept;

        /// __format::fmt_string [fwd decl]
        template <typename... Args>
        struct fmt_string;

    } // namespace __format

    /// format_string
    // validates format string at compile-time
    template <typename... Args>
    using format_string = __format::fmt_string<std::type_identity_t<Args>...>;

    /// unchecked_format_string
    struct unchecked_format_string;

} // namespace gold

#endif // __GOLD_BITS_FORMAT_FWD_HPP
