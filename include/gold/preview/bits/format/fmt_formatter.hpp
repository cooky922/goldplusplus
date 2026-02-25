// <gold/preview/bits/format/fmt_formatter.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: this is only a prototype
// note: this is an internal header

#pragma once
#ifndef __GOLD_PREVIEW_BITS_FMT_FORMATTER_HPP
#define __GOLD_PREVIEW_BITS_FMT_FORMATTER_HPP

#include <gold/preview/bits/format/fmt_error.hpp>
#include <charconv>

namespace gold::preview {

    /// formatter [ unstable, not yet defined ]
    template <typename T, typename CharT>
    struct formatter {

        /// parse
        auto parse(auto& parse_ctx) -> decltype(parse_ctx.begin()) { throw; }

        /// format
        auto format(T, auto& ctx) -> decltype(ctx.out()) { throw; }

    };

    namespace __format::__detail {

        /// formatter_char_
        template <typename T, typename CharT>
        struct formatter_char_ {

            auto parse(auto& parse_ctx) -> decltype(parse_ctx.begin()) {
                // todo: fmt implement this function
                return parse_ctx.begin();
            }

            auto format(T c, auto& ctx) -> decltype(ctx.out()) {
                // todo: fmt implement the parsed formatting arguments
                auto out_iter_ = ctx.out();
                *out_iter_++ = CharT(c);
                return out_iter_;
            }
        };

        /// formatter_c_string_
        template <typename CharT>
        struct formatter_c_string_ {

            auto parse(auto& parse_ctx) -> decltype(parse_ctx.begin()) {
                return parse_ctx.begin();
            }

            auto format(const CharT* str, auto& ctx) -> decltype(ctx.out()) {
                auto out_iter_ = ctx.out();
                while (*str)
                    *out_iter_++ = *str++;
                return out_iter_;
            }

        };

        /// formatter_string_
        template <typename CharT>
        struct formatter_string_ {

            auto parse(auto& parse_ctx) -> decltype(parse_ctx.begin()) {
                return parse_ctx.begin();
            }

            auto format(std::basic_string_view<CharT> str, auto& ctx) -> decltype(ctx.out()) {
                auto out_iter_ = ctx.out();
                for (const auto c : str)
                    *out_iter_++ = c;
                return out_iter_;
            }

        };

        /// formatter_arithmetic_
        template <typename T, typename CharT>
            requires (std::is_arithmetic_v<T> && !std::same_as<T, bool>)
        struct formatter_arithmetic_ {

            auto parse(auto& parse_ctx) -> decltype(parse_ctx.begin()) {
                return parse_ctx.begin();
            }

            auto format(T value, auto& ctx) -> decltype(ctx.out()) {
                // TODO FMT Implement using formatting arguments
                // TODO FMT Improve PoC since using std::to_string is inefficient.
                // Note the code doesn't use std::string::iterator since the unit tests
                // test with debug iterators and they fail with strings created from
                // std::to_string.
                auto str_ = convert_(value);
                auto out_iter_ = ctx.out();
                for (std::size_t i = 0, e = str_.size(); i != e; ++i)
                    *out_iter_++ = str_[i];
                return out_iter_;
            }

          private:
            /// temporary:
            template <typename U>
            static std::string convert_(U value)
                requires std::same_as<CharT, char>
            {
                return std::to_string(value);
            }

            template <typename U>
            static std::wstring convert_(U value)
                requires std::same_as<CharT, wchar_t>
            {
                return std::to_wstring(value);
            }

        };

    } // namespace __format::__detail

    //// helper specializations

    /// formatter<char, char>
    template <>
    struct formatter<char, char> : __format::__detail::formatter_char_<char, char> {};

    /// formatter<char, wchar_t>
    template <>
    struct formatter<char, wchar_t> : __format::__detail::formatter_char_<char, wchar_t> {};

    /// formatter<wchar_t, wchar_t>
    template <>
    struct formatter<wchar_t, wchar_t> : __format::__detail::formatter_char_<wchar_t, wchar_t> {};

    /// formatter<CharT*, CharT>
    template <typename CharT>
    struct formatter<CharT*, CharT> : __format::__detail::formatter_c_string_<CharT> {

        using base_ = __format::__detail::formatter_c_string_<CharT>;

        auto format(CharT* str, auto& ctx) -> decltype(ctx.out()) {
            gold::system_assert(str, "The basic_format_arg ctor. should have prevented an invalid pointer");
            return base_::format(str, ctx);
        }
    };

    /// formatter<const CharT*, CharT>
    template <typename CharT>
    struct formatter<const CharT*, CharT>
    : __format::__detail::formatter_c_string_<CharT> {};

    /// formatter<const CharT[N], CharT>
    template <typename CharT, std::size_t N>
    struct formatter<const CharT[N], CharT>
    : __format::__detail::formatter_string_<CharT> {

        using base_ = __format::__detail::formatter_string_<CharT>;

        auto format(const CharT str[N], auto& ctx) -> decltype(ctx.out()) {
            return base_::format(std::basic_string_view<CharT>(str, N), ctx);
        }
    };

    /// formatter<std::basic_string<CharT, Traits, Alloc>, CharT>
    template <typename CharT, typename Traits, typename Alloc>
    struct formatter<std::basic_string<CharT, Traits, Alloc>, CharT>
    : __format::__detail::formatter_string_<CharT> {

        using base_ = __format::__detail::formatter_string_<CharT>;

        auto format(const std::basic_string<CharT, Traits, Alloc>& str, auto& ctx) -> decltype(ctx.out()) {
            return base_::format(std::basic_string_view<CharT>(str), ctx);
        }

    };

    /// formatter<std::basic_string_view<CharT, Traits, Alloc>, CharT>
    template <typename CharT, typename Traits>
    struct formatter<std::basic_string_view<CharT, Traits>, CharT>
    : __format::__detail::formatter_string_<CharT> {};

    /// formatter<bool, CharT>
    template <typename CharT>
    struct formatter<bool, CharT> {

        auto parse(auto& parse_ctx) -> decltype(parse_ctx.begin()) {
            return parse_ctx.begin();
        }

        auto format(bool b, auto& ctx) -> decltype(ctx.out()) {
            auto out_iter_ = ctx.out();
            *out_iter_++ = CharT('0') + b;
            return out_iter_;
        }
    };

    /// formatter<signed char, CharT>
    template <typename CharT>
    struct formatter<signed char, CharT>
    : __format::__detail::formatter_arithmetic_<signed char, CharT> {};

    /// formatter<short, CharT>
    template <typename CharT>
    struct formatter<short, CharT>
    : __format::__detail::formatter_arithmetic_<short, CharT> {};

    /// formatter<int, CharT>
    template <typename CharT>
    struct formatter<int, CharT>
    : __format::__detail::formatter_arithmetic_<int, CharT> {};

    /// formatter<long, CharT>
    template <typename CharT>
    struct formatter<long, CharT>
    : __format::__detail::formatter_arithmetic_<long, CharT> {};

    /// formatter<long long, CharT>
    template <typename CharT>
    struct formatter<long long, CharT>
    : __format::__detail::formatter_arithmetic_<long long, CharT> {};

    /// formatter<unsigned char, CharT>
    template <typename CharT>
    struct formatter<unsigned char, CharT>
    : __format::__detail::formatter_arithmetic_<unsigned char, CharT> {};

    /// formatter<unsigned short, CharT>
    template <typename CharT>
    struct formatter<unsigned short, CharT>
    : __format::__detail::formatter_arithmetic_<unsigned short, CharT> {};

    /// formatter<unsigned, CharT>
    template <typename CharT>
    struct formatter<unsigned, CharT>
    : __format::__detail::formatter_arithmetic_<unsigned, CharT> {};

    /// formatter<unsigned long, CharT>
    template <typename CharT>
    struct formatter<unsigned long, CharT>
    : __format::__detail::formatter_arithmetic_<unsigned long, CharT> {};

    /// formatter<unsigned long long, CharT>
    template <typename CharT>
    struct formatter<unsigned long long, CharT>
    : __format::__detail::formatter_arithmetic_<unsigned long long, CharT> {};

    /// formatter<float, CharT>
    template <typename CharT>
    struct formatter<float, CharT>
    : __format::__detail::formatter_arithmetic_<float, CharT> {};

    /// formatter<double, CharT>
    template <typename CharT>
    struct formatter<double, CharT>
    : __format::__detail::formatter_arithmetic_<double, CharT> {};

    /// formatter<long double, CharT>
    template <typename CharT>
    struct formatter<long double, CharT>
    : __format::__detail::formatter_arithmetic_<long double, CharT> {};

} // namespace gold::preview

#endif // __GOLD_PREVIEW_BITS_FMT_FORMATTER_HPP
