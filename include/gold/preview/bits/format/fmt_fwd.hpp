// <gold/preview/bits/format/fmt_fwd.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: this is only a prototype
// note: this is an internal header

#pragma once
#ifndef __GOLD_PREVIEW_BITS_FMT_FWD_HPP
#define __GOLD_PREVIEW_BITS_FMT_FWD_HPP

namespace gold::preview {

    /// basic_format_parse_context [ forward decl ]
    template <typename CharT>
    class basic_format_parse_context;

    /// basic_format_arg [ forward decl ]
    template <typename Context>
    class basic_format_arg;

    namespace __detail {

        /// format_arg_store_ [ forward decl ]
        template <typename Context, typename... Args>
        struct format_arg_store_;

        /// make_basic_format_args_ [ forward decl ]
        template <typename Context, typename... Args>
        format_arg_store_<Context, Args...> make_basic_format_args_(const Args&...);

    } // namespace __detail

    /// formatter [ forward decl ]
    template <typename T, typename CharT = char>
    struct formatter;

} // namespace gold::preview

#endif // __GOLD_PREVIEW_BITS_FMT_FWD_HPP
