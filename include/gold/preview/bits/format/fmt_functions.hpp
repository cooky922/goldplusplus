// <gold/preview/bits/format/fmt_functions.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: this is only a prototype
// note: this is an internal header

#pragma once
#ifndef __GOLD_PREVIEW_BITS_FMT_FUNCTIONS_HPP
#define __GOLD_PREVIEW_BITS_FMT_FUNCTIONS_HPP

#include <bits/stl_algo.h>
#include <gold/preview/bits/format/fmt_parse_context.hpp>
#include <gold/preview/bits/format/fmt_make.hpp>
#include <gold/preview/bits/format/fmt_string.hpp>

namespace gold::preview {

    namespace __format::__detail {

        /// vformat_to_impl_
        template <typename ParseCtx, typename Ctx>
        typename Ctx::iterator vformat_to_impl_(ParseCtx&& parse_ctx, Ctx&& ctx) {

            using char_type = typename ParseCtx::char_type;

            static_assert(std::same_as<typename Ctx::char_type, char_type>);

            const char_type* begin_ = parse_ctx.begin();
            const char_type* end_   = parse_ctx.end();
            typename Ctx::iterator out_iter_ = ctx.out();

            while (begin_ != end_) {
                switch (*begin_) {

                  case char_type('{'):
                    ++begin_;
                    if (begin_ == end_)
                        throw gold::preview::format_error("The format string terminates at a '{'");

                    if (*begin_ != char_type('{')) [[likely]] {
                        ctx.advance_to(std::move(out_iter_));
                        string_iter_pair_t_<char_type> iter_pair { begin_, end_ };
                        begin_    = handle_replacement_field_(&iter_pair, parse_ctx, ctx);
                        out_iter_ = ctx.out();

                        // The output is written and __begin points to the next character. So
                        // start the next iteration.
                        continue;
                    }

                    break;

                  case char_type('}'):
                    ++begin_;
                    if (begin_ == end_ || *begin_ == char_type('}'))
                        throw gold::preview::format_error("The format string contains an invalid escape sequence");

                    break;
                }

                // copy the character to the output verbatim
                *out_iter_++ = *begin_++;
            }

            return out_iter_;
        }

        template <typename OutIter, typename CharT>
            requires std::output_iterator<OutIter, const CharT&>
        OutIter vformat_to_impl_(OutIter out_iter,
                                 std::basic_string_view<CharT> fmt,
                                 format_args_t<std::type_identity_t<OutIter>, CharT> args) {
            return vformat_to_impl_(
                basic_format_parse_context{fmt, args.size_()},
                gold::preview::__detail::format_context_create_(std::move(out_iter), args)
            );
        }

        template <typename OutIter, typename CharT>
            requires std::output_iterator<OutIter, const CharT&>
        OutIter vformat_to_impl_(OutIter out_iter, std::locale loc,
                                 std::basic_string_view<CharT> fmt,
                                 format_args_t<std::type_identity_t<OutIter>, CharT> args) {
            return vformat_to_impl_(
                basic_format_parse_context{fmt, args.size_()},
                gold::preview::__detail::format_context_create_(std::move(out_iter), args, std::move(loc))
            );
        }

    } // namespace __format::__detail

    //// style:
    /// function ( string_view, ... )
    /// function ( locale, string_view, ... )
    /// function ( wstring_view, ... )
    /// function ( locale, wstring_view, ... )

    /// vformat_to
    template <std::output_iterator<const char&> OutIter>
    OutIter vformat_to(OutIter out_iter, std::string_view fmt,
                       format_args_t<std::type_identity_t<OutIter>, char> args) {
        return gold::preview::__format::__detail::vformat_to_impl_(std::move(out_iter), fmt, args);
    }

    template <std::output_iterator<const char&> OutIter>
    OutIter vformat_to(OutIter out_iter, std::locale loc, std::string_view fmt,
                       format_args_t<std::type_identity_t<OutIter>, char> args) {
        return gold::preview::__format::__detail::vformat_to_impl_(std::move(out_iter), std::move(loc), fmt, args);
    }

    template <std::output_iterator<const wchar_t&> OutIter>
    OutIter vformat_to(OutIter out_iter, std::wstring_view fmt,
                       format_args_t<std::type_identity_t<OutIter>, wchar_t> args) {
        return gold::preview::__format::__detail::vformat_to_impl_(std::move(out_iter), fmt, args);
    }

    template <std::output_iterator<const wchar_t&> OutIter>
    OutIter vformat_to(OutIter out_iter, std::locale loc, std::wstring_view fmt,
                       format_args_t<std::type_identity_t<OutIter>, wchar_t> args) {
        return gold::preview::__format::__detail::vformat_to_impl_(std::move(out_iter), std::move(loc), fmt, args);
    }

    /// format_to
    template <std::output_iterator<const char&> OutIter, typename... Args>
    OutIter format_to(OutIter out_iter, std::string_view fmt, const Args&... args) {
        return gold::preview::vformat_to(
            std::move(out_iter), fmt,
            gold::preview::__detail::make_basic_format_args_<basic_format_context<OutIter, char>>(args...)
        );
    }

    template <std::output_iterator<const char&> OutIter, typename... Args>
    OutIter format_to(OutIter out_iter, std::locale loc, std::string_view fmt, const Args&... args) {
        return gold::preview::vformat_to(
            std::move(out_iter), std::move(loc), fmt,
            gold::preview::__detail::make_basic_format_args_<basic_format_context<OutIter, char>>(args...)
        );
    }

    template <std::output_iterator<const wchar_t&> OutIter, typename... Args>
    OutIter format_to(OutIter out_iter, std::wstring_view fmt, const Args&... args) {
        return gold::preview::vformat_to(
            std::move(out_iter), fmt,
            gold::preview::__detail::make_basic_format_args_<basic_format_context<OutIter, wchar_t>>(args...)
        );
    }

    template <std::output_iterator<const wchar_t&> OutIter, typename... Args>
    OutIter format_to(OutIter out_iter, std::locale loc, std::wstring_view fmt, const Args&... args) {
        return gold::preview::vformat_to(
            std::move(out_iter), std::move(loc), fmt,
            gold::preview::__detail::make_basic_format_args_<basic_format_context<OutIter, wchar_t>>(args...)
        );
    }

    /// vformat
    inline std::string vformat(std::string_view fmt, format_args args) {
        std::string result_;
        gold::preview::vformat_to(std::back_inserter(result_), fmt, args);
        return result_;
    }

    inline std::string vformat(std::locale loc, std::string_view fmt, format_args args) {
        std::string result_;
        gold::preview::vformat_to(std::back_inserter(result_), std::move(loc), fmt, args);
        return result_;
    }

    inline std::wstring vformat(std::wstring_view fmt, wformat_args args) {
        std::wstring result_;
        gold::preview::vformat_to(std::back_inserter(result_), fmt, args);
        return result_;
    }

    inline std::wstring vformat(std::locale loc, std::wstring_view fmt, wformat_args args) {
        std::wstring result_;
        gold::preview::vformat_to(std::back_inserter(result_), std::move(loc), fmt, args);
        return result_;
    }

    /// format
    template <typename... Args>
    std::string format(std::string_view fmt, const Args&... args) {
        return gold::preview::vformat(fmt, gold::preview::make_format_args(args...));
    }

    template <typename... Args>
    std::string format(std::locale loc, std::string_view fmt, const Args&... args) {
        return gold::preview::vformat(std::move(loc), fmt, gold::preview::make_format_args(args...));
    }

    template <typename... Args>
    std::wstring format(std::wstring_view fmt, const Args&... args) {
        return gold::preview::vformat(fmt, gold::preview::make_wformat_args(args...));
    }

    template <typename... Args>
    std::wstring format(std::locale loc, std::wstring_view fmt, const Args&... args) {
        return gold::preview::vformat(std::move(loc), fmt, gold::preview::make_wformat_args(args...));
    }

    /// format_to_n_result
    template <typename OutIter>
    struct format_to_n_result {
        OutIter out;
        std::iter_difference_t<OutIter> size;
    };

    /// format_to_n
    template <std::output_iterator<const char&> OutIter, typename... Args>
    format_to_n_result<OutIter> format_to_n(OutIter out_iter, std::iter_difference_t<OutIter> n, std::string_view fmt,
                                            const Args&... args) {

        // note: TODO FMT Improve PoC: using std::string is inefficient.
        std::string str_ = gold::preview::vformat(
            fmt, gold::preview::make_format_args(args...)
        );

        std::iter_difference_t<OutIter> s_ = str_.size();
        std::iter_difference_t<OutIter> m_ = std::clamp(n, std::iter_difference_t<OutIter>(0), s_);
        out_iter = std::copy_n(str_.begin(), m_, std::move(out_iter));

        return { .out = std::move(out_iter), .size = s_ };
    }

    template <std::output_iterator<const char&> OutIter, typename... Args>
    format_to_n_result<OutIter> format_to_n(OutIter out_iter, std::iter_difference_t<OutIter> n,
                                            std::locale loc, std::string_view fmt,
                                            const Args&... args) {

        // note: TODO FMT Improve PoC: using std::string is inefficient.
        std::string str_ = gold::preview::vformat(
            std::move(loc), fmt, gold::preview::make_format_args(args...)
        );

        std::iter_difference_t<OutIter> s_ = str_.size();
        std::iter_difference_t<OutIter> m_ = std::clamp(n, std::iter_difference_t<OutIter>(0), s_);
        out_iter = std::copy_n(str_.begin(), m_, std::move(out_iter));

        return { .out = std::move(out_iter), .size = s_ };
    }

    template <std::output_iterator<const wchar_t&> OutIter, typename... Args>
    format_to_n_result<OutIter> format_to_n(OutIter out_iter, std::iter_difference_t<OutIter> n, std::wstring_view fmt,
                                            const Args&... args) {

        // note: TODO FMT Improve PoC: using std::wstring is inefficient.
        std::wstring str_ = gold::preview::vformat(
            fmt, gold::preview::make_wformat_args(args...)
        );

        std::iter_difference_t<OutIter> s_ = str_.size();
        std::iter_difference_t<OutIter> m_ = std::clamp(n, std::iter_difference_t<OutIter>(0), s_);
        out_iter = std::copy_n(str_.begin(), m_, std::move(out_iter));

        return { .out = std::move(out_iter), .size = s_ };
    }

    template <std::output_iterator<const wchar_t&> OutIter, typename... Args>
    format_to_n_result<OutIter> format_to_n(OutIter out_iter, std::iter_difference_t<OutIter> n,
                                            std::locale loc, std::wstring_view fmt,
                                            const Args&... args) {

        // note: TODO FMT Improve PoC: using std::wstring is inefficient.
        std::wstring str_ = gold::preview::vformat(
            std::move(loc), fmt, gold::preview::make_wformat_args(args...)
        );

        std::iter_difference_t<OutIter> s_ = str_.size();
        std::iter_difference_t<OutIter> m_ = std::clamp(n, std::iter_difference_t<OutIter>(0), s_);
        out_iter = std::copy_n(str_.begin(), m_, std::move(out_iter));

        return { .out = std::move(out_iter), .size = s_ };
    }

    /// formatted_size
    template <typename... Args>
    std::size_t formatted_size(std::string_view fmt, const Args&... args) {
        // TODO FMT Improve PoC: using std::string is inefficient.
        return gold::preview::vformat(fmt, gold::preview::make_format_args(args...)).size();
    }

    template <typename... Args>
    std::size_t formatted_size(std::locale loc, std::string_view fmt, const Args&... args) {
        // TODO FMT Improve PoC: using std::string is inefficient.
        return gold::preview::vformat(std::move(loc), fmt, gold::preview::make_format_args(args...)).size();
    }

    template <typename... Args>
    std::size_t formatted_size(std::wstring_view fmt, const Args&... args) {
        // TODO FMT Improve PoC: using std::wstring is inefficient.
        return gold::preview::vformat(fmt, gold::preview::make_wformat_args(args...)).size();
    }

    template <typename... Args>
    std::size_t formatted_size(std::locale loc, std::wstring_view fmt, const Args&... args) {
        // TODO FMT Improve PoC: using std::wstring is inefficient.
        return gold::preview::vformat(std::move(loc), fmt, gold::preview::make_wformat_args(args...)).size();
    }

} // namespace gold::preview

#endif // __GOLD_PREVIEW_BITS_FMT_FUNCTIONS_HPP
