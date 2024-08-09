// <gold/bits/format/formatter.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_FORMATTER_HPP
#define __GOLD_BITS_FORMAT_FORMATTER_HPP

#include <limits>
#include <gold/bits/algo/min_max.hpp>
#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/error.hpp>
#include <gold/bits/format/parser.hpp>
#include <gold/bits/format/sink.hpp>
#include <gold/bits/numeric/fp_parts.hpp>

namespace gold::__format {

    /// __format::put_sign
    template <typename Int>
    constexpr char* put_sign(Int i, __specs::sign_kind sign, char* dest) noexcept {
        if (i < 0)
            *dest = '-';
        else if (sign == __specs::sign_kind::_plus)
            *dest = '+';
        else if (sign == __specs::sign_kind::_space)
            *dest = ' ';
        else
            ++dest;
        return dest;
    }

    /// __format::write
    template <__format::char_output_iterator Out>
    constexpr Out write(Out out, std::string_view str) {
        if constexpr (__is_same(sink_iterator, Out)) {
            if (str.size())
                out = str;
        } else {
            for (char c : str)
                *out++ = c;
        }
        return out;
    }

    /// __format::write_range
    template <__format::char_output_iterator Out, typename R>
    constexpr Out write_range(Out out, R&& r) {
        if constexpr (__is_convertible(R, std::string_view))
            return __format::write(out, r);
        else {
            for (char c : r)
                *out++ = c;
        }
        return out;
    }

    /// __format::write_filled
    template <__format::char_output_iterator Out>
    constexpr Out write_filled(Out out, char c, std::size_t n) {
        while (n--)
            *out++ = c;
        return out;
    }

    /// __format::to_char_hex
    constexpr char to_char_hex(gold::uint8_t op, bool upper = false) {
        if (10 <= op && op <= 15)
            return upper ? 'A' + (op - 10) : 'a' + (op - 10);
        return '0' + op;
    }

    /// __format::write_byte_hex
    template <__format::char_output_iterator Out>
    constexpr Out write_byte_hex(Out out, gold::uint8_t op, bool zero_fill = false, bool upper = false) {
        if (op > 0xf || zero_fill)
            *out++ = op > 0xf ? __format::to_char_hex(op >> 4, upper) : '0';
        *out++ = __format::to_char_hex(op & 0xf, upper);
        return out;
    }

    /// __format::escape_quote_mark
    enum class escape_quote_mark : bool { single_quote, double_quote };

    /// __format::write_escaped
    template <__format::char_output_iterator Out>
    constexpr Out write_escaped(Out out, std::string_view str, escape_quote_mark mark) {
        *out++ = mark == escape_quote_mark::single_quote ? '\'' : '"';
        for (char c : str) {
            switch (c) {
              // '\t' == '\x09'
              case '\t':
                out = __format::write(std::move(out), "\\t");
                break;
              // '\n' == '\x0a'
              case '\n':
                out = __format::write(std::move(out), "\\n");
                break;
              // '\r' == '\x0d'
              case '\r':
                out = __format::write(std::move(out), "\\r");
                break;
              case '\'':
                if (mark == escape_quote_mark::single_quote)
                    out = __format::write(std::move(out), R"(\')");
                else
                    *out++ = c;
                break;
              case '"':
                if (mark == escape_quote_mark::double_quote)
                    out = __format::write(std::move(out), R"(\")");
                else
                    *out++ = c;
                break;
              case '\\':
                out = __format::write(std::move(out), R"(\\)");
                break;
              default:
                // other control codes including null character
                if (('\x{0}' <= c && c <= '\x{1f}') || c == '\x{7f}') {
                    out = __format::write(std::move(out), "\\u{");
                    out = __format::write_byte_hex(std::move(out), static_cast<gold::uint8_t>(c));
                    *out++ = '}';
                } else {
                    *out++ = c;
                }
            }
        }
        *out++ = mark == escape_quote_mark::single_quote ? '\'' : '"';
        return out;
    }

    /// __format::write_padded
    template <__format::char_output_iterator Out>
    constexpr Out write_padded(Out out, std::string_view str, __specs::align_kind align_op,
                               std::size_t nfill, char fill_char) {
        const std::size_t buflen = 0x20;
        char padding_chars[buflen];
        std::string_view padding { padding_chars, buflen };

        auto pad = [&padding](std::size_t n, Out& o) {
            if (n == 0)
                return;
            while (n > padding.size()) {
                o = __format::write(std::move(o), padding);
                n -= padding.size();
            }
            if (n != 0)
                o = __format::write(std::move(o), padding.substr(0, n));
        };

        std::size_t l, r, max_op;
        if (align_op == __specs::align_kind::_center) {
            l = nfill / 2;
            r = l + (nfill & 1);
            max_op = r;
        } else if (align_op == __specs::align_kind::_right) {
            l = nfill;
            r = 0;
            max_op = l;
        } else {
            l = 0;
            r = nfill;
            max_op = r;
        }
        if (max_op < buflen)
            padding.remove_suffix(buflen - max_op);
        else
            max_op = buflen;
        std::char_traits<char>::assign(padding_chars, max_op, fill_char);
        pad(l, out);
        out = __format::write(std::move(out), str);
        pad(r, out);

        return out;
    }

    /// __format::try_write_padded_as_spec
    constexpr __format::result<typename format_context::iterator>
    try_write_padded_as_spec(std::string_view str,
                             std::size_t estimated_width,
                             format_context& fc,
                             const __specs::spec_base& spec,
                             __specs::align_kind align_op = __specs::align_kind::_left) noexcept {
        std::size_t width = 0;
        __GOLDM_FMT_TRY(spec.try_get_width(fc), { width = try_result; });
        if (width <= estimated_width)
            return __format::write(fc.out(), str);
        const std::size_t nfill = width - estimated_width;
        if (__parse::to_bool(align_op))
            align_op = spec.alignment;
        return __format::write_padded(fc.out(), str, align_op, nfill, spec.fill_char);
    }

    /// __format::formatter_str
    struct formatter_str {
      private:
        __specs::std_spec m_spec_ {};

      public:
        constexpr __format::result<typename format_parse_context::iterator>
        try_parse(format_parse_context& pc) noexcept {
            auto first = pc.begin();
            const auto last = pc.end();
            __specs::std_spec spec {};

            auto finalize = [this, &spec] { m_spec_ = spec; };
            auto finished = [&] {
                if (first == last || *first == '}') {
                    finalize();
                    return true;
                }
                return false;
            };

            if (finished())
                return first;

            first = spec.parse_fill_and_align(first, last);
            if (finished())
                return first;

            __GOLDM_FMT_TRY((spec.try_parse_width(first, last, pc)), { first = try_result; });
            if (finished())
                return first;

            __GOLDM_FMT_TRY((spec.try_parse_precision(first, last, pc)), { first = try_result; });
            if (finished())
                return first;

            // presentation type
            if (*first == 's')
                ++first;
            else if (*first == '?') {
                spec.pres = __specs::pres_kind::_escape;
                ++first;
            }

            if (finished())
                return first;

            return __format::error_t(__fmt_error_code::failed_to_parse_fmt_spec);
        }

        constexpr __format::result<typename format_context::iterator>
        try_format(std::string_view s, format_context& fc) const noexcept {
            if (!m_spec_.has_width() && !m_spec_.has_precision()) {
                if (m_spec_.pres == __specs::pres_kind::_escape)
                    return __format::write_escaped(fc.out(), s, __format::escape_quote_mark::double_quote);
                else
                    return __format::write(fc.out(), s);
            }

            std::string es;
            std::size_t estimated_width = s.size();

            if (m_spec_.pres == __specs::pres_kind::_escape) {
                __format::str_sink sink;
                __format::sink_iterator iter = sink.out();
                iter = __format::write_escaped(iter, s, __format::escape_quote_mark::double_quote);
                es = std::move(sink).get();
                estimated_width = es.size();
                s = es;
            }

            if (m_spec_.has_precision()) {
                std::size_t prec = 0;
                __GOLDM_FMT_TRY(m_spec_.try_get_precision(fc), { prec = try_result; });
                if (estimated_width > prec) {
                    s = s.substr(0, prec);
                    estimated_width = prec;
                }
            }

            return __format::try_write_padded_as_spec(s, estimated_width, fc, m_spec_);
        }

        constexpr void set_debug_format() noexcept { m_spec_.pres = __specs::pres_kind::_escape; }
    };

    /// __format::to_uppercase
    constexpr char to_uppercase(char c) noexcept {
        return c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c;
    }

    /// __format::formatter_int
    struct formatter_int {
        static constexpr __specs::pres_kind s_as_integer_ = __specs::pres_kind::_decimal;
        static constexpr __specs::pres_kind s_as_bool_    = __specs::pres_kind::_string;
        static constexpr __specs::pres_kind s_as_char_    = __specs::pres_kind::_char;

        __specs::std_spec m_spec_ {};

        constexpr __format::result<typename format_parse_context::iterator>
        mf_try_do_parse_(format_parse_context& pc, __specs::pres_kind type) noexcept {
            __specs::std_spec spec {};
            spec.pres = type;

            const auto last = pc.end();
            auto first = pc.begin();

            auto finalize = [this, &spec] { m_spec_ = spec; };
            auto finished = [&] {
                if (first == last || *first == '}') {
                    finalize();
                    return true;
                }
                return false;
            };

            if (finished())
                return first;

            first = spec.parse_fill_and_align(first, last);
            if (finished())
                return first;

            first = spec.parse_sign(first, last);
            if (finished())
                return first;

            first = spec.parse_alternate_form(first, last);
            if (finished())
                return first;

            first = spec.parse_zero_fill(first, last);
            if (finished())
                return first;

            __GOLDM_FMT_TRY((spec.try_parse_width(first, last, pc)), { first = try_result; });
            if (finished())
                return first;

            switch (*first) {
              case 'b':
                spec.pres = __specs::pres_kind::_binary;
                ++first;
                break;
              case 'B':
                spec.pres = __specs::pres_kind::_binary_upper;
                ++first;
                break;
              case 'c':
                if (type != s_as_bool_) {
                    spec.pres = __specs::pres_kind::_char;
                    ++first;
                }
                break;
              case 'd':
                spec.pres = __specs::pres_kind::_decimal;
                ++first;
                break;
              case 'o':
                spec.pres = __specs::pres_kind::_octal;
                ++first;
                break;
              case 'x':
                spec.pres = __specs::pres_kind::_hex;
                ++first;
                break;
              case 'X':
                spec.pres = __specs::pres_kind::_hex_upper;
                ++first;
                break;
              case 's':
                if (type == s_as_bool_) {
                    spec.pres = __specs::pres_kind::_string;
                    ++first;
                }
                break;
              case '?':
                if (type == s_as_char_) {
                    spec.pres = __specs::pres_kind::_escape;
                    ++first;
                }
                break;
            }

            if (finished())
                return first;

            return __format::error_t(__fmt_error_code::failed_to_parse_fmt_spec);
        }

        template <typename T>
        constexpr __format::result<typename format_parse_context::iterator>
        mf_try_parse_(format_parse_context& pc) noexcept {
            if constexpr (__is_same(bool, T)) {
                auto parse_result = mf_try_do_parse_(pc, s_as_bool_);
                if (!parse_result)
                    return __format::error_t(parse_result.error());
                auto last = *parse_result;
                if (m_spec_.pres == __specs::pres_kind::_string ||
                    m_spec_.pres == __specs::pres_kind::_default)
                    if (__parse::to_bool(m_spec_.sign) ||
                        m_spec_.alt || m_spec_.zero_fill)
                        return __format::error_t(__fmt_error_code::invalid_options_for_bool);
                return last;
            }
            else if constexpr (__is_same(char, T)) {
                auto parse_result = mf_try_do_parse_(pc, s_as_char_);
                if (!parse_result)
                    return __format::error_t(parse_result.error());
                auto last = *parse_result;
                if (m_spec_.pres == __specs::pres_kind::_char ||
                    m_spec_.pres == __specs::pres_kind::_escape)
                    if (__parse::to_bool(m_spec_.sign) ||
                        m_spec_.alt || m_spec_.zero_fill)
                        return __format::error_t(__fmt_error_code::invalid_options_for_char);
                return last;
            }
            else
                return mf_try_do_parse_(pc, s_as_integer_);
        }

        template <typename T>
        static constexpr __format::result<char> sf_try_to_character_(T i) noexcept {
            using Traits = std::numeric_limits<char>;
            if constexpr (std::is_signed_v<T> == std::is_signed_v<char>) {
                if (Traits::min() <= i && i <= Traits::max())
                    return static_cast<char>(i);
            } else if constexpr (std::is_signed_v<T>) {
                if (i >= 0 && std::make_unsigned_t<T>(i) <= Traits::max())
                    return static_cast<char>(i);
            } else if (i <= std::make_unsigned_t<char>(Traits::max()))
                return static_cast<char>(i);
            return __format::error_t(__fmt_error_code::integer_not_representable_as_character);
        }

        constexpr __format::result<typename format_context::iterator>
        mf_try_format_character_(char c, format_context& fc) const noexcept {
            return __format::try_write_padded_as_spec({&c, 1u}, 1, fc, m_spec_);
        }

        constexpr __format::result<typename format_context::iterator>
        mf_try_format_character_escaped_(char c, format_context& fc) const noexcept {
            char buf[7];
            char* last = __format::write_escaped(buf, {&c, 1u}, __format::escape_quote_mark::single_quote);
            std::size_t n = static_cast<std::size_t>(last - buf);
            return __format::try_write_padded_as_spec({buf, n}, n, fc, m_spec_);
        }

        constexpr __format::result<typename format_context::iterator>
        mf_try_format_int_(std::string_view s, std::size_t prefix_len, format_context& fc) const noexcept {
            std::size_t width = 0;
            __GOLDM_FMT_TRY(m_spec_.try_get_width(fc), { width = try_result; });
            if (width <= s.size())
                return __format::write(fc.out(), s);
            char fill_char = m_spec_.fill_char;
            __specs::align_kind align_op = m_spec_.alignment;
            std::size_t nfill = width - s.size();
            auto out = fc.out();
            if (align_op == __specs::align_kind::_default) {
                align_op = __specs::align_kind::_right;
                if (m_spec_.zero_fill) {
                    fill_char = '0';
                    if (prefix_len != 0) {
                        out = __format::write(std::move(out), s.substr(0, prefix_len));
                        s.remove_prefix(prefix_len);
                    }
                } else
                    fill_char = ' ';
            }
            return __format::write_padded(std::move(out), s, align_op, nfill, fill_char);
        }

        template <typename Int>
        constexpr __format::result<typename format_context::iterator>
        try_format(Int i, format_context& fc) const noexcept {
            if (m_spec_.pres == __specs::pres_kind::_char) {
                char c = '\0';
                __GOLDM_FMT_TRY(sf_try_to_character_(i), { c = try_result; });
                return mf_try_format_character_(c, fc);
            }

            char buf[sizeof(Int) * 8 + 3];
            std::to_chars_result res {};
            std::string_view base_prefix;
            std::make_unsigned_t<Int> u;
            if (i < 0)
                u = -static_cast<std::make_unsigned_t<Int>>(i);
            else
                u = i;
            char* start = buf + 3;
            char* const last = buf + sizeof(buf);
            char* const start_digits = start;

            switch (m_spec_.pres) {
              case __specs::pres_kind::_binary:
              case __specs::pres_kind::_binary_upper:
                base_prefix = m_spec_.pres == __specs::pres_kind::_binary ? "0b" : "0B";
                res = std::to_chars(start, last, u, 2);
                break;

              case __specs::pres_kind::_default:
                [[fallthrough]];
              case __specs::pres_kind::_decimal:
                res = std::to_chars(start, last, u, 10);
                break;

              case __specs::pres_kind::_octal:
                if (i != 0)
                    base_prefix = "0";
                res = std::to_chars(start, last, u, 8);
                break;

              case __specs::pres_kind::_hex:
              case __specs::pres_kind::_hex_upper:
                base_prefix = m_spec_.pres == __specs::pres_kind::_hex ? "0x" : "0X";
                res = std::to_chars(start, last, u, 16);
                if (m_spec_.pres == __specs::pres_kind::_hex_upper) {
                    for (auto p = start; p != res.ptr; ++p)
                        *p = __format::to_uppercase(*p);
                }
                break;
              default:
                __builtin_unreachable();
            }
            if (m_spec_.alt && base_prefix.size()) {
                start -= base_prefix.size();
                if consteval {
                    __format::write(start, base_prefix);
                } else {
                    __builtin_memcpy(start, base_prefix.data(), base_prefix.size());
                }
            }

            start = __format::put_sign(i, m_spec_.sign, start - 1);
            return mf_try_format_int_(std::string_view(start, res.ptr - start), start_digits - start, fc);
        }

        constexpr __format::result<typename format_context::iterator>
        try_format(bool b, format_context& fc) const noexcept {
            if (m_spec_.pres == __specs::pres_kind::_char)
                return mf_try_format_character_(static_cast<unsigned char>(b), fc);
            if (m_spec_.pres == __specs::pres_kind::_default ||
                m_spec_.pres == __specs::pres_kind::_string) {
                std::string_view s = b ? "true" : "false";
                return __format::try_write_padded_as_spec(s, s.size(), fc, m_spec_);
            }
            return try_format(static_cast<unsigned char>(b), fc);
        }
    };

    /// __format::has_to_chars
    template <typename T>
    inline constexpr bool has_to_chars = requires (char* p, T op) {
        std::to_chars(p, p, op, std::chars_format::scientific, 6);
    };

    /// __format::formattable_float
    template <typename T>
    concept formattable_float = std::floating_point<T> && has_to_chars<T>;

    // compile-time fallback for formatting floating-points
    namespace __fp {

        /// __format::__fp::to_chars_uint
        constexpr char* to_chars_uint(char* out, auto op, int base = 10) {
            char buffer[sizeof(op) * 8 + 1];
            auto [ptr, __] = std::to_chars(buffer, buffer + sizeof(buffer), op, base);
            return __format::write(out, std::string_view{buffer, static_cast<std::size_t>(ptr - buffer)});
        }

        /// __format::__fp::fp_wrapper
        struct fp_wrapper {
            gold::uint128_t whole   = 0;
            gold::uint128_t frac    = 0;
            unsigned int frac_leading_zeroes = 0;

            constexpr fp_wrapper(gold::float128_t op, unsigned int prec = 6)
            : whole (static_cast<gold::uint128_t>(op)) {
                op -= whole;
                while (prec) {
                    op *= 10;
                    if (static_cast<gold::uint128_t>(op) == 0) {
                        ++frac_leading_zeroes;
                        continue;
                    }
                    --prec;
                }
                frac = static_cast<gold::uint128_t>(op);
            }
        };

        /// __format::__fp::to_chars_fp_hex
        constexpr char* to_chars_fp_hex(char* out, auto fp, std::size_t prec = 0, bool upper = false) {
            using F     = decltype(fp);
            using Parts = __num::fp_parts<F>;

            auto parts = Parts::from_float(fp);

            if (parts.mantissa == 0) {
                // if zero
                if (parts.exponent == 0) {
                    *out++ = '0';
                    *out++ = upper ? 'P' : 'p';
                    *out++ = '+';
                    *out++ = '0';
                    return out;
                }
                *out++ = '1';
                if (prec > 0) {
                    *out++ = '.';
                    out = __format::write_filled(out, '0', prec);
                }
                *out++ = upper ? 'P' : 'p';
                const int biased_exp = parts.biased_exponent();
                if (biased_exp >= 0)
                    *out++ = '+';
                out = __format::__fp::to_chars_uint(out, biased_exp);
                return out;
            }

            *out++ = '1';
            *out++ = '.';

            constexpr std::size_t shift_amount = Parts::mantissa_bits & 3 ? 4 - (Parts::mantissa_bits & 3) : 0;
            constexpr std::size_t padded_mantissa_bits = Parts::mantissa_bits + (Parts::mantissa_bits & 3 ? shift_amount : 0);
            constexpr std::size_t nibble_count = padded_mantissa_bits >> 2;

            auto mantissa = static_cast<typename Parts::storage_type>(parts.mantissa) << shift_amount;
            auto nibble_at = [mantissa](std::size_t pos) -> gold::uint8_t {
                return static_cast<gold::uint8_t>((mantissa >> ((nibble_count - pos - 1) << 2)) & 0xFu);
            };
            auto trailing_zeroes_start_at = [&nibble_at] -> std::size_t {
                gold::uint32_t flags = 0;
                for (std::size_t i = 0; i < nibble_count; ++i) {
                    flags <<= 1;
                    flags  |= nibble_at(i) != 0;
                }
                return nibble_count - __builtin_ctz(flags);
            }();

            char buffer[40];
            std::size_t i = 0;
            for (; i < nibble_count; ++i) {
                if (i >= trailing_zeroes_start_at)
                    break;
                buffer[i] = __format::to_char_hex(nibble_at(i), upper);
            }

            if (prec > 0 && prec <= i)
                i = prec;
            out = __format::write(out, std::string_view{buffer, i});
            if (prec > i)
                out = __format::write_filled(out, '0', prec - i);

            *out++ = upper ? 'P' : 'p';
            const int biased_exp = parts.biased_exponent();
            if (biased_exp >= 0)
                *out++ = '+';
            return __format::__fp::to_chars_uint(out, biased_exp);
        }

        /// __format::__fp::to_chars_fp_fixed
        constexpr char* to_chars_fp_fixed(char* out, auto fp, std::size_t prec = 0) {
            using F = decltype(fp);
            using Parts = __num::fp_parts<F>;

            __format::__fp::fp_wrapper wrap (static_cast<gold::float128_t>(fp), prec ? prec : Parts::max_precision);
            out = __format::__fp::to_chars_uint(out, wrap.whole);

            if (wrap.frac == 0) {
                if (prec > 0) {
                    *out++ = '.';
                    out = __format::write_filled(out, '0', prec);
                }
                return out;
            }
            *out++ = '.';
            std::size_t len = 0;
            if (wrap.frac_leading_zeroes) {
                len = wrap.frac_leading_zeroes;
                if (prec > 0 && prec <= wrap.frac_leading_zeroes)
                    return __format::write_filled(out, '0', prec);
                out = __format::write_filled(out, '0', len);
            }
            char buffer[40];
            char* ptr = std::to_chars(buffer, buffer + sizeof(buffer), wrap.frac, 10).ptr;
            if (len && prec)
                prec -= len;
            len = ptr - buffer;
            if (prec > 0 && prec <= len)
                len = prec;
            out = __format::write(out, std::string_view{buffer, len});
            if (prec > len)
                out = __format::write_filled(out, '0', prec - len);
            return out;
        }

        /// __format::__fp::fp_floor_log10
        constexpr int fp_floor_log10(auto fp) {
            using F = decltype(fp);
            if constexpr (__is_same(F, float) || __is_same(F, gold::float32x_t))
                return __builtin_floorf(__builtin_log10f(static_cast<float>(fp)));
            else if constexpr (__is_same(F, double) || __is_same(F, gold::float64x_t))
                return __builtin_floor(__builtin_log10(static_cast<double>(fp)));
            else if constexpr (__is_same(F, long double))
                return __builtin_floorl(__builtin_log10l(fp));
            else if constexpr (__is_same(F, gold::float16x_t))
                return __builtin_floorf16(__builtin_log10f16(fp));
            else if constexpr (__is_same(F, gold::bfloat16x_t))
                return static_cast<gold::bfloat16x_t>(__builtin_floorf(__builtin_log10f(static_cast<float>(fp))));
            __builtin_unreachable();
        }

        /// __format::__fp::to_chars_fp_scientific_with_exp
        constexpr char* to_chars_fp_scientific_with_exp(char* out, auto fp, int exponent, std::size_t prec = 0, bool upper = false) {
            using F = decltype(fp);
            F normalized_coefficient = [exponent, fp] {
                if constexpr (__is_same(F, float) || __is_same(F, gold::float32x_t)
                                                  || __is_same(F, gold::bfloat16x_t))
                    return fp / static_cast<F>(__builtin_powf(10.0f, exponent));
                else if constexpr (__is_same(F, double) || __is_same(F, gold::float64x_t))
                    return fp / __builtin_pow(10.0, exponent);
                else if constexpr (__is_same(F, long double))
                    return fp / __builtin_powl(10.0l, exponent);
                else if constexpr (__is_same(F, gold::float16x_t))
                    return fp / __builtin_powf16(10.0f16, exponent);
                __builtin_unreachable();
            }();
            out = __format::__fp::to_chars_fp_fixed(out, normalized_coefficient, prec);
            *out++ = upper ? 'E' : 'e';
            if (exponent >= 0)
                *out++ = '+';
            return __format::__fp::to_chars_uint(out, exponent);
        }

        /// __format::__fp::to_chars_fp_scientific
        constexpr char* to_chars_fp_scientific(char* out, auto fp, std::size_t prec = 0, bool upper = false) {
            return __format::__fp::to_chars_fp_scientific_with_exp(
                out, fp, __format::__fp::fp_floor_log10(fp), prec, upper
            );
        }

        /// __format::__fp::to_chars_fp_general
        constexpr char* to_chars_fp_general(char* out, auto fp, std::size_t prec = 0, bool upper = false) {
            using F = decltype(fp);
            const int exponent = __format::__fp::fp_floor_log10(fp);
            if ((-4 <= exponent && exponent <= 16) || fp == static_cast<F>(0.0))
                return __format::__fp::to_chars_fp_fixed(out, fp, prec);
            return __format::__fp::to_chars_fp_scientific_with_exp(out, fp, exponent, prec, upper);
        }

        /// __format::__fp::to_chars_fp
        constexpr char* to_chars_fp(char* out, auto fp, std::chars_format fmt = std::chars_format::general,
                                                        std::size_t prec = 0,
                                                        bool upper = false,
                                                        __specs::sign_kind sign = __specs::sign_kind::_default) {
            if (__builtin_isnan(fp))
                return __format::write(out, std::string_view(upper ? "NAN" : "nan"));

            if (__builtin_signbit(fp)) {
                *out++ = '-';
                fp = -fp;
            } else {
                if (sign == __specs::sign_kind::_plus)
                    *out++ = '+';
                else if (sign == __specs::sign_kind::_space)
                    *out++ = ' ';
            }

            if (__builtin_isinf(fp))
                return __format::write(out, std::string_view(upper ? "INF" : "inf"));

            switch (fmt) {
              case std::chars_format::general:
                return __format::__fp::to_chars_fp_general(out, fp, prec, upper);
              case std::chars_format::fixed:
                return __format::__fp::to_chars_fp_fixed(out, fp, prec);
              case std::chars_format::scientific:
                return __format::__fp::to_chars_fp_scientific(out, fp, prec, upper);
              case std::chars_format::hex:
                return __format::__fp::to_chars_fp_hex(out, fp, prec, upper);
            }

            __builtin_unreachable();
        }

    } // namespace __fp

    /// __format::formatter_fp
    struct formatter_fp {
        __specs::std_spec m_spec_ {};

        constexpr __format::result<typename format_parse_context::iterator>
        try_parse(format_parse_context& pc) noexcept {
            __specs::std_spec spec {};

            const auto last = pc.end();
            auto first = pc.begin();

            auto finalize = [this, &spec] { m_spec_ = spec; };
            auto finished = [&] {
                if (first == last || *first == '}') {
                    finalize();
                    return true;
                }
                return false;
            };

            if (finished())
                return first;

            first = spec.parse_fill_and_align(first, last);
            if (finished())
                return first;

            first = spec.parse_sign(first, last);
            if (finished())
                return first;

            first = spec.parse_alternate_form(first, last);
            if (finished())
                return first;

            first = spec.parse_zero_fill(first, last);
            if (finished())
                return first;

            if (first[0] != '.') {
                __GOLDM_FMT_TRY((spec.try_parse_width(first, last, pc)), { first = try_result; });
                if (finished())
                    return first;
            }

            __GOLDM_FMT_TRY((spec.try_parse_precision(first, last, pc)), { first = try_result; });
            if (finished())
                return first;

            switch (*first) {
              case 'a':
                spec.pres = __specs::pres_kind::_float_hex;
                ++first;
                break;
              case 'A':
                spec.pres = __specs::pres_kind::_float_hex_upper;
                ++first;
                break;
              case 'e':
                spec.pres = __specs::pres_kind::_scientific;
                ++first;
                break;
              case 'E':
                spec.pres = __specs::pres_kind::_scientific_upper;
                ++first;
                break;
              case 'f':
                spec.pres = __specs::pres_kind::_fixed;
                ++first;
                break;
              case 'F':
                spec.pres = __specs::pres_kind::_fixed_upper;
                ++first;
                break;
              case 'g':
                spec.pres = __specs::pres_kind::_general;
                ++first;
                break;
              case 'G':
                spec.pres = __specs::pres_kind::_general_upper;
                ++first;
                break;
            }

            if (finished())
                return first;

            return __format::error_t(__fmt_error_code::failed_to_parse_fmt_spec);
        }

        template <typename F>
        constexpr __format::result<typename format_context::iterator>
        try_format(F op, format_context& fc) const noexcept {
            std::string_view str;
            std::string dynbuf;
            char buf[512] {};
            if consteval {
                // compile-time fallback implementation for
                // formatting floating-points with limitations:
                // + can't produce shortest string output
                // + can't guarantee round-trip
                // + rounding errors are everywhere
                // + can't format with alternate form
                std::size_t prec = 6;
                bool use_prec = m_spec_.has_precision();
                if (use_prec)
                    __GOLDM_FMT_TRY(m_spec_.try_get_precision(fc), { prec = try_result; });

                std::chars_format fmt {};
                bool upper = false;

                switch (m_spec_.pres) {
                  case __specs::pres_kind::_float_hex_upper:
                    upper = true;
                    [[fallthrough]];
                  case __specs::pres_kind::_float_hex:
                    fmt  = std::chars_format::hex;
                    break;
                  case __specs::pres_kind::_scientific_upper:
                    upper = true;
                    [[fallthrough]];
                  case __specs::pres_kind::_scientific:
                    use_prec = true;
                    fmt = std::chars_format::scientific;
                    break;
                  case __specs::pres_kind::_fixed_upper:
                    upper = true;
                    [[fallthrough]];
                  case __specs::pres_kind::_fixed:
                    use_prec = true;
                    fmt = std::chars_format::fixed;
                    break;
                  case __specs::pres_kind::_general_upper:
                    upper = true;
                    [[fallthrough]];
                  case __specs::pres_kind::_general:
                    use_prec = true;
                    fmt = std::chars_format::general;
                    break;
                  case __specs::pres_kind::_default:
                    if (use_prec)
                        fmt = std::chars_format::general;
                    break;
                  default:;
                }

                char* ptr = [&] {
                    if (use_prec)
                        return __format::__fp::to_chars_fp(buf, op, fmt, prec, upper, m_spec_.sign);
                    else
                        return __format::__fp::to_chars_fp(buf, op, fmt, 0, upper, m_spec_.sign);
                }();

                str = std::string_view(buf, ptr - buf);
            } else {
                std::to_chars_result res {};

                std::size_t prec = 6;
                bool use_prec = m_spec_.has_precision();
                if (use_prec)
                    __GOLDM_FMT_TRY(m_spec_.try_get_precision(fc), { prec = try_result; });

                char* start = buf + 1; // reserve space for sign
                char* last  = buf + sizeof(buf);

                std::chars_format fmt {};
                bool upper = false;
                bool trailing_zeros = false;
                char expc = 'e';

                switch (m_spec_.pres) {
                  case __specs::pres_kind::_float_hex_upper:
                    upper = true;
                    expc  = 'P';
                    [[fallthrough]];
                  case __specs::pres_kind::_float_hex:
                    if (m_spec_.pres != __specs::pres_kind::_float_hex_upper)
                        expc = 'p';
                    fmt  = std::chars_format::hex;
                    break;
                  case __specs::pres_kind::_scientific_upper:
                    upper = true;
                    expc  = 'E';
                    [[fallthrough]];
                  case __specs::pres_kind::_scientific:
                    use_prec = true;
                    fmt = std::chars_format::scientific;
                    break;
                  case __specs::pres_kind::_fixed_upper:
                    upper = true;
                    [[fallthrough]];
                  case __specs::pres_kind::_fixed:
                    use_prec = true;
                    fmt = std::chars_format::fixed;
                    break;
                  case __specs::pres_kind::_general_upper:
                    upper = true;
                    expc  = 'E';
                    [[fallthrough]];
                  case __specs::pres_kind::_general:
                    trailing_zeros = true;
                    use_prec = true;
                    fmt = std::chars_format::general;
                    break;
                  case __specs::pres_kind::_default:
                    if (use_prec)
                        fmt = std::chars_format::general;
                    break;
                  default:;
                }

                auto to_chars_fn = [&](char* b, char* e) {
                    if (use_prec)
                        return std::to_chars(b, e, op, fmt, prec);
                    else if (fmt != std::chars_format{})
                        return std::to_chars(b, e, op, fmt);
                    else
                        return std::to_chars(b, e, op);
                };

                res = to_chars_fn(start, last);

                if (res.ec == std::errc::value_too_large) [[unlikely]] {
                    // If the buffer is too small it's probably because of a large
                    // precision, or a very large value in fixed format.
                    std::size_t guess = 8 + prec;
                    if (fmt == std::chars_format::fixed && op != 0) {
                        if constexpr (__is_same(float, F))
                            guess += __builtin_log10f(op < 0.0f ? -op : op);
                        else if constexpr (__is_same(double, F))
                            guess += __builtin_log10(op < 0.0 ? -op : op);
                        else if constexpr (__is_same(long double, F))
                            guess += __builtin_log10l(op < 0.0l ? -op : op);
                        else
                            guess = std::numeric_limits<F>::max_exponent10;
                    }
                    if (guess <= sizeof(buf)) [[unlikely]]
                        guess = sizeof(buf) * 2;
                    dynbuf.reserve(guess);

                    do {
                        dynbuf.resize_and_overwrite(dynbuf.capacity() * 2, [&to_chars_fn, &res](char* p, std::size_t n) {
                            res = to_chars_fn(p + 1, p + n - 1);
                            return res.ec == std::errc{} ? res.ptr - p : 0;
                        });
                        start = dynbuf.data() + 1;
                        last  = dynbuf.data() + dynbuf.size();
                    } while (__builtin_expect(res.ec == std::errc::value_too_large, 0));
                }

                if (upper) {
                    for (char* p = start; p != res.ptr; ++p)
                        *p = __format::to_uppercase(*p);
                }

                // add sign for non-negative values
                if (!__builtin_signbit(op)) {
                    if (m_spec_.sign == __specs::sign_kind::_plus)
                        *--start = '+';
                    else if (m_spec_.sign == __specs::sign_kind::_space)
                        *--start = ' ';
                }

                str = std::string_view(start, res.ptr - start);

                // use the alternate form
                if (m_spec_.alt && __builtin_isfinite(op)) {
                    std::string_view s = str;
                    std::size_t z = 0;
                    std::size_t p;
                    std::size_t d = s.find('.');
                    std::size_t sigfigs;
                    if (d != s.npos) {
                        p = s.find(expc, d + 1);
                        if (p == s.npos)
                            p = s.size();
                        sigfigs = p - 1;
                    } else {
                        p = s.find(expc);
                        if (p == s.npos)
                            p = s.size();
                        d = p; // position where '.' should be inserted
                        sigfigs = d;
                    }

                    if (trailing_zeros && prec != 0) {
                        if (!__format::__parse::is_xdigit(s[0]))
                            --sigfigs;
                        z = prec - sigfigs; // number of zeroes to insert
                    }

                    if (std::size_t extras = int(d == p) + z) {
                        if (dynbuf.empty() && extras <= std::size_t(last - res.ptr)) {
                            // move exponent to make space for extra chars
                            __builtin_memmove(start + p + extras,
                                              start + p,
                                              s.size() - p);
                            if (d == p)
                                start[p++] = '.';
                            __builtin_memset(start + p, '0', z);
                            str = {s.data(), s.size() + extras};
                        } else {
                            dynbuf.reserve(s.size() + extras);
                            if (dynbuf.empty()) {
                                dynbuf = s.substr(0, p);
                                if (d == p)
                                    dynbuf += '.';
                                if (z)
                                    dynbuf.append(z, '0');
                            } else {
                                dynbuf.insert(p, extras, '0');
                                if (d == p)
                                    dynbuf[p] = '.';
                            }
                            str = dynbuf;
                        }
                    }
                }
            }

            std::size_t width = 0;
            __GOLDM_FMT_TRY(m_spec_.try_get_width(fc), { width = try_result; });

            if (width <= str.size())
                return __format::write(fc.out(), str);

            char fill_char = m_spec_.fill_char;
            __specs::align_kind align_op = m_spec_.alignment;

            std::size_t nfill = width - str.size();
            auto out = fc.out();
            if (align_op == __specs::align_kind::_default) {
                align_op = __specs::align_kind::_right;
                if (m_spec_.zero_fill && __builtin_isfinite(op)) {
                    fill_char = '0';
                    // write sign before zero filling
                    if (!__format::__parse::is_xdigit(str[0])) {
                        *out++ = str[0];
                        str.remove_prefix(1);
                    }
                } else
                    fill_char = ' ';
            }
            return __format::write_padded(std::move(out), str, align_op, nfill, fill_char);
        }
    };

    /// __format::formatter_ptr
    struct formatter_ptr {
        __specs::std_spec m_spec_ {};

        constexpr __format::result<typename format_parse_context::iterator>
        try_parse(format_parse_context& pc) noexcept {
            __specs::std_spec spec {};
            const auto last = pc.end();
            auto first = pc.begin();

            auto finalize = [this, &spec] { m_spec_ = spec; };
            auto finished = [&] {
                if (first == last || *first == '}') {
                    finalize();
                    return true;
                }
                return false;
            };

            if (finished())
                return first;

            first = spec.parse_fill_and_align(first, last);
            if (finished())
                return first;

            first = spec.parse_zero_fill(first, last);
            if (finished())
                return first;

            __GOLDM_FMT_TRY((spec.try_parse_width(first, last, pc)), { first = try_result; });
            if (finished())
                return first;

            if (first != last) {
                if (*first == 'p')
                    ++first;
                else if (*first == 'P') {
                    spec.pres = __specs::pres_kind::_pointer_upper;
                    ++first;
                }
            }

            if (finished())
                return first;

            return __format::error_t(__fmt_error_code::failed_to_parse_fmt_spec);
        }

        constexpr __format::result<typename format_context::iterator>
        try_format(const void* p, format_context& fc) const noexcept {
            if consteval {
                static constexpr std::string_view str = "<pointer>";
                return __format::try_write_padded_as_spec(str, str.size(), fc, m_spec_, __specs::align_kind::_right);
            } else {
                auto u = reinterpret_cast<__UINTPTR_TYPE__>(p);
                char buf[2 + sizeof(p) * 2];
                auto [ptr, _] = std::to_chars(buf + 2, buf + 2 + sizeof(p) * 2, u, 16);
                int n = ptr - buf;
                buf[0] = '0';
                buf[1] = 'x';
                if (m_spec_.pres == __specs::pres_kind::_pointer_upper) {
                    buf[1] = 'X';
                    for (auto p = buf + 2; p != ptr; ++p)
                        *p = __format::to_uppercase(*p);
                }
                std::string_view str (buf, n);
                if (m_spec_.zero_fill) {
                    std::size_t width = 0;
                    __GOLDM_FMT_TRY(m_spec_.try_get_width(fc), { width = try_result; });
                    if (width <= str.size())
                        return __format::write(fc.out(), str);
                    auto out = fc.out();
                    // write "0x" or "0X" prefix before zero-filling
                    out = __format::write(std::move(out), str.substr(0, 2));
                    str.remove_prefix(2);
                    std::size_t nfill = width - n;
                    return __format::write_padded(std::move(out), str, __specs::align_kind::_right,
                                                  nfill, '0');
                }
                return __format::try_write_padded_as_spec(str, n, fc, m_spec_, __specs::align_kind::_right);
            }
        }
    };

    /// __format::formatter_ord
    struct formatter_ord {
        __specs::std_spec m_spec_ {};

        constexpr __format::result<typename format_parse_context::iterator>
        mf_try_parse_(format_parse_context& pc) noexcept {
            __specs::std_spec spec {};
            const auto last = pc.end();
            auto first = pc.begin();

            auto finalize = [this, &spec] { m_spec_ = spec; };
            auto finished = [&] {
                if (first == last || *first == '}') {
                    finalize();
                    return true;
                }
                return false;
            };

            if (finished())
                return first;

            first = spec.parse_fill_and_align(first, last);
            if (finished())
                return first;

            first = spec.parse_sign(first, last);
            if (finished())
                return first;

            first = spec.parse_alternate_form(first, last);
            if (finished())
                return first;

            __GOLDM_FMT_TRY((spec.try_parse_width(first, last, pc)), { first = try_result; });
            if (finished())
                return first;

            // only supported pres: integer form and string
            // - if string  -> only alternate form
            switch (*first) {
              case 'b':
                spec.pres = __specs::pres_kind::_binary;
                ++first;
                break;
              case 'B':
                spec.pres = __specs::pres_kind::_binary_upper;
                ++first;
                break;
              case 'd':
                spec.pres = __specs::pres_kind::_decimal;
                ++first;
                break;
              case 'o':
                spec.pres = __specs::pres_kind::_octal;
                ++first;
                break;
              case 'x':
                spec.pres = __specs::pres_kind::_hex;
                ++first;
                break;
              case 'X':
                spec.pres = __specs::pres_kind::_hex_upper;
                ++first;
                break;
              case 's':
                ++first;
                break;
            }

            if (finished())
                return first;

            return __format::error_t(__fmt_error_code::failed_to_parse_fmt_spec);
        }

        constexpr __format::result<typename format_parse_context::iterator>
        try_parse(format_parse_context& pc) noexcept {
            auto parse_result = mf_try_parse_(pc);
            if (m_spec_.pres == __specs::pres_kind::_default ||
                m_spec_.pres == __specs::pres_kind::_string)
                if (__parse::to_bool(m_spec_.sign))
                    return __format::error_t(__fmt_error_code::invalid_options_for_ordering);
            return parse_result;
        }

        template <typename T>
        constexpr auto try_format(T op, format_context& fc) const noexcept {
            if (m_spec_.pres == __specs::pres_kind::_decimal) {
                const __format::formatter_int f { m_spec_ };
                return f.try_format(__builtin_bit_cast(gold::int8_t, op), fc);
            }
            std::string_view prefix = [this] {
                if (!m_spec_.alt)
                    return "";
                if constexpr (__is_same(std::strong_ordering, T))
                    return "strong_ordering::";
                else if constexpr (__is_same(std::weak_ordering, T))
                    return "weak_ordering::";
                else
                    return "partial_ordering::";
            }();
            std::string_view str = [op] {
                gold::int8_t u = __builtin_bit_cast(gold::int8_t, op);
                switch (u) {
                  // less
                  case -1:
                    return "less";
                  // equal | equivalent
                  case 0:
                    if constexpr (__is_same(std::strong_ordering, T))
                        return "equal";
                    else
                        return "equivalent";
                  // greater
                  case 1:
                    return "greater";
                  // unordered
                  case 2:
                    return "unordered";
                }
                __builtin_unreachable();
            }();
            char buf[29];
            char* last = __format::write(buf, prefix);
            last = __format::write(last, str);
            std::string_view final_str (buf, last - buf);
            return __format::try_write_padded_as_spec(final_str, final_str.size(), fc, m_spec_);
        }
    };

} // namespace gold::__format

#endif // __GOLD_BITS_FORMAT_FORMATTER_HPP
