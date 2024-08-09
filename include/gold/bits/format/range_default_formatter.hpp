// <gold/bits/format/range_default_formatter.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_RANGE_DEFAULT_FORMATTER_HPP
#define __GOLD_BITS_FORMAT_RANGE_DEFAULT_FORMATTER_HPP

#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/range_formatter.hpp>

namespace gold {

    /// range_format
    enum class range_format {
        disabled, map, set, sequence, string, debug_string
    };

    /// range_format_kind
    template <typename R>
    inline constexpr range_format range_format_kind = [] {
        static_assert(false, "create a template specialization of format_kind for your type");
        return range_format::disabled;
    }();

    template <std::ranges::input_range R>
        requires std::same_as<R, std::remove_cvref_t<R>>
    inline constexpr range_format range_format_kind<R> = [] {
        if constexpr (std::same_as<std::remove_cvref_t<std::ranges::range_reference_t<R>>, R>)
            return range_format::disabled;
        else if constexpr (requires { typename R::key_type; }) {
            if constexpr (requires { typename R::mapped_type; } &&
                          gold::tuples::pair_like<std::remove_cvref_t<std::ranges::range_reference_t<R>>>)
                return range_format::map;
            else
                return range_format::set;
        } else
            return range_format::sequence;
    }();

    namespace __format {

        /// __format::range_default_formatter
        template <range_format, std::ranges::input_range>
        struct range_default_formatter;

        template <std::ranges::input_range R>
        struct range_default_formatter<range_format::sequence, R> {
          private:
            using maybe_const_r = __format::fmt_maybe_const<R>;
            range_formatter<std::remove_cvref_t<std::ranges::range_reference_t<maybe_const_r>>> m_underlying_;

          public:
            constexpr void set_separator(std::string_view sep) noexcept {
                m_underlying_.set_separator(sep);
            }

            constexpr void set_brackets(std::string_view opening, std::string_view closing) noexcept {
                m_underlying_.set_brackets(opening, closing);
            }

            constexpr void set_default_format() { m_underlying_.set_default_format(); }

            constexpr auto try_parse(format_parse_context& pc) noexcept {
                return m_underlying_.try_parse(pc);
            }

            constexpr auto try_format(maybe_const_r& r, format_context& fc) const
                noexcept(noexcept(m_underlying_.try_format(r, fc)))
            {
                return m_underlying_.try_format(r, fc);
            }
        };

        template <std::ranges::input_range R>
        struct range_default_formatter<range_format::map, R> {
          private:
            using maybe_const_r = __format::fmt_maybe_const<R>;
            using element_type  = std::remove_cvref_t<std::ranges::range_reference_t<maybe_const_r>>;
            range_formatter<element_type> m_underlying_;

          public:
            constexpr range_default_formatter() requires gold::tuples::pair_like<element_type> {
                m_underlying_.set_brackets("{", "}");
                m_underlying_.underlying().set_brackets({}, {});
                m_underlying_.underlying().set_separator(": ");
            }

            constexpr void set_default_format() { m_underlying_.set_default_format(); }

            constexpr auto try_parse(format_parse_context& pc) noexcept {
                return m_underlying_.try_parse(pc);
            }

            constexpr auto try_format(maybe_const_r& r, format_context& fc) const
                noexcept(noexcept(m_underlying_.try_format(r, fc)))
            {
                return m_underlying_.try_format(r, fc);
            }
        };

        template <std::ranges::input_range R>
        struct range_default_formatter<range_format::set, R> {
          private:
            using maybe_const_r = __format::fmt_maybe_const<R>;
            using element_type  = std::remove_cvref_t<std::ranges::range_reference_t<maybe_const_r>>;
            range_formatter<element_type> m_underlying_;

          public:
            constexpr range_default_formatter() {
                m_underlying_.set_brackets("{", "}");
            }

            constexpr void set_default_format() { m_underlying_.set_default_format(); }

            constexpr auto try_parse(format_parse_context& pc) noexcept {
                return m_underlying_.try_parse(pc);
            }

            constexpr auto try_format(maybe_const_r& r, format_context& fc) const
                noexcept(noexcept(m_underlying_.try_format(r, fc)))
            {
                return m_underlying_.try_format(r, fc);
            }
        };

        template <range_format K, std::ranges::input_range R>
            requires (K == range_format::string || K == range_format::debug_string)
        struct range_default_formatter<K, R> {
          private:
            __format::formatter_str m_underlying_;

            using D = gold::conditional_t<std::ranges::input_range<const R>, const R&, R&>;

          public:
            constexpr void set_default_format() {
                if constexpr (K == range_format::debug_string)
                    m_underlying_.set_debug_format();
            }

            constexpr auto try_parse(format_parse_context& pc) noexcept {
                auto result = m_underlying_.try_parse(pc);
                if constexpr (K == range_format::debug_string)
                    m_underlying_.set_debug_format();
                return result;
            }

            constexpr auto try_format(D r, format_context& fc) const noexcept {
                if constexpr (std::ranges::contiguous_range<R> && std::ranges::sized_range<R>) {
                    return m_underlying_.try_format(std::string_view { std::ranges::data(r), std::ranges::size(r) }, fc);
                } else {
                    __format::str_sink sink;
                    __format::write_range(sink.out(), r);
                    return m_underlying_.try_format(std::move(sink).get(), fc);
                }
            }
        };

    } // namespace __format

    template <std::ranges::input_range R>
        requires (range_format_kind<R> != range_format::disabled && formattable<std::ranges::range_reference_t<R>>)
    struct formatter<R> : __format::range_default_formatter<range_format_kind<R>, R> {};

} // namespace gold

#endif // __GOLD_BITS_FORMAT_RANGE_DEFAULT_FORMATTER_HPP
