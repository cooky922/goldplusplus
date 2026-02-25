// <gold/preview/bits/format/fmt_make.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: this is only a prototype
// note: this is an internal header

#pragma once
#ifndef __GOLD_PREVIEW_BITS_FMT_MAKE_HPP
#define __GOLD_PREVIEW_BITS_FMT_MAKE_HPP

#include <array>
#include <gold/pack>

#include <gold/preview/bits/format/fmt_fwd.hpp>
#include <gold/preview/bits/format/fmt_arg.hpp>
#include <gold/preview/bits/format/fmt_context.hpp>

namespace gold::preview {

    namespace __detail {

        /// format_arg_store_
        template <typename Context, typename... Args>
        struct format_arg_store_ {
            std::array<basic_format_arg<Context>, sizeof...(Args)> args_;
        };

        /// deduction guide for format_arg_store_
        template <typename Context, typename... Args>
        format_arg_store_(Args...) -> format_arg_store_<typename gold::pack_at_front<Args...>::context_type, Args...>;

        /// basic_format_arg_identity_
        template <typename T>
        decltype(auto) basic_format_arg_identity_(T&& arg) {
            return std::forward<T>(arg);
        }

        /// make_basic_format_args_
        template <typename Context, typename... Args>
        format_arg_store_<Context, Args...> make_basic_format_args_(const Args&... args) {
            return { basic_format_arg<Context>(args)... };
        }

    } // namespace __detail

    /// make_format_args
    template <typename... Args>
    auto make_format_args(const Args&... args) {
        return __detail::make_basic_format_args_<format_context>(args...);
    }

    /// make_wformat_args
    template <typename... Args>
    auto make_wformat_args(const Args&... args) {
        return __detail::make_basic_format_args_<wformat_context>(args...);
    }

} // namespace gold::preview

#endif // __GOLD_PREVIEW_BITS_FMT_MAKE_HPP
