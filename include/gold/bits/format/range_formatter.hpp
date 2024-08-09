// <gold/bits/format/range_formatter.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_RANGE_FORMATTER_HPP
#define __GOLD_BITS_FORMAT_RANGE_FORMATTER_HPP

#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/concepts.hpp>
#include <gold/bits/format/parser.hpp>
#include <gold/bits/format/formatter.hpp>
#include <gold/bits/format/error.hpp>
#include <gold/bits/format/sink.hpp>
#include <gold/bits/format/tuple_formatter.hpp>
#include <gold/bits/tuples/concepts.hpp>
#include <gold/bits/type_traits/conditional.hpp>
#include <bits/ranges_base.h>

namespace gold {

    namespace __format {

        /// __format::const_formattable_range
        template <typename R>
        concept const_formattable_range =
            std::ranges::input_range<const R> &&
            gold::formattable<std::ranges::range_reference_t<const R>>;

        /// __format::fmt_maybe_const
        template <typename R>
        using fmt_maybe_const = gold::conditional_t<
            __format::const_formattable_range<R>, const R, R
        >;

        namespace __specs {

            /// __format::__specs::range_pres_kind
            enum class range_pres_kind : gold::uint8_t {
                _default,
                _mapped,
                _string,
                _escaped
            };

            /// __format::__specs::range_spec
            struct range_spec : spec_base {
                wp_kind sep_kind          : 2 = wp_kind::_none;
                bool clear_brackets       : 1 = false;
                bool has_underlying_spec  : 1 = false;
                range_pres_kind pres_kind : 2 = range_pres_kind::_default;
                gold::uint16_t sep_arg_id {};

                using iterator = std::string_view::iterator;

                constexpr bool has_specified_separator() const noexcept {
                    return static_cast<gold::uint8_t>(sep_kind) == static_cast<gold::uint8_t>(wp_kind::_from_arg);
                }

                constexpr iterator parse_fill_and_align(iterator first, iterator last) noexcept {
                    if (*first != ':')
                        return this->spec_base::parse_fill_and_align(first, last);
                    return first;
                }

                constexpr __format::result<iterator> try_parse_separator(iterator first, iterator last, format_parse_context& pc) noexcept {
                    if (*first != '~')
                        return first;
                    ++first;
                    if (*first != '{')
                        return __format::error_t(__fmt_error_code::missing_separator_field_after_tilde_in_fmt_string);
                    ++first;
                    if (first == last)
                        return __format::error_t(__fmt_error_code::unmatched_left_brace_in_fmt_string);
                    if (*first == '}') {
                        if (auto check_result = pc.try_next_arg_id(); !check_result)
                            return __format::error_t(check_result.error());
                        else
                            sep_arg_id = *check_result;
                        if (auto check_result = pc.try_check_dynamic_spec_string(sep_arg_id); !check_result)
                            return __format::error_t(check_result.error());
                    } else {
                        auto [v, ptr] = __parse::parse_arg_id(first, last);
                        if (ptr == nullptr || ptr == last || *ptr != '}')
                            return __format::error_t(__fmt_error_code::invalid_arg_id_in_fmt_string);
                        first = ptr;
                        if (auto check_result = pc.try_check_arg_id(v); !check_result)
                            return __format::error_t(check_result.error());
                        if (auto check_result = pc.try_check_dynamic_spec_string(v); !check_result)
                            return __format::error_t(check_result.error());
                        sep_arg_id = v;
                    }
                    // past the '}'
                    ++first;
                    sep_kind = wp_kind::_from_arg;
                    return first;
                }

                constexpr __format::result<std::string_view> try_get_separator(format_context& ctx) const noexcept {
                    if (has_specified_separator())
                        return try_string_from_arg(ctx.arg(sep_arg_id));
                    return ", ";
                }

                constexpr iterator parse_clear_brackets(iterator first, iterator) noexcept {
                    if (*first == 'n') {
                        clear_brackets = true;
                        ++first;
                    }
                    return first;
                }

                constexpr __format::result<iterator> try_parse_range_type(iterator first, iterator last) noexcept {
                    switch (*first) {
                      case 'm':
                        pres_kind = range_pres_kind::_mapped;
                        ++first;
                        break;
                      case 's':
                        pres_kind = range_pres_kind::_string;
                        ++first;
                        break;
                      case '?':
                        ++first;
                        if (first == last || *first != 's')
                            return __format::error_t(__fmt_error_code::failed_to_parse_fmt_spec);
                        pres_kind = range_pres_kind::_escaped;
                        ++first;
                        break;
                    }
                    return first;
                }

                constexpr iterator parse_has_underlying_spec(iterator first, iterator) noexcept {
                    if (*first == ':') {
                        has_underlying_spec = true;
                        ++first;
                    }
                    return first;
                }
            };

        } // namespace __specs

    } // namespace __format

    /// range_formatter
    // [:range-format-spec:]
    //      | [:fill-and-align (opt):] [:width (opt):] [:sep (opt):] 'n' (opt)
    //        [:range-type (opt):] [:range-underlying-spec (opt):]

    // [:fill-and-align:]
    //      | any character other than '{', '}', or ':'

    // [:sep:]
    //      | '~' '{' [:arg-id:] ? '}'

    // [:range-type:]
    //      | 'm' | 's' | '?s'

    // [:range-underlying-spec:]
    //      | ':' [:format-spec:]
    template <typename T>
        requires std::same_as<std::remove_cvref_t<T>, T> &&
                 gold::formattable<T>
    struct range_formatter {
      private:
        __format::__specs::range_spec m_rspec_ {};
        gold::formatter<T>            m_underlying_;
        std::string_view              m_sep_ = ", ";
        std::string_view              m_opening_bracket_ = "[";
        std::string_view              m_closing_bracket_ = "]";

        template <typename R>
        constexpr __format::result<typename format_context::iterator>
        mf_try_format_as_sequence_(R&& r, format_context& fc) const
            noexcept(noexcept(m_underlying_.try_format(std::declval<T&>(), fc)))
        {
            fc.advance_to(__format::write(fc.out(), m_opening_bracket_));
            bool use_separator = false;
            std::string_view sep = m_sep_;
            if (m_rspec_.has_specified_separator()) {
                if (auto get_result = m_rspec_.try_get_separator(fc); !get_result)
                    return __format::error_t(get_result.error());
                else
                    sep = *get_result;
            }
            for (auto&& e : r) {
                if (use_separator)
                    fc.advance_to(__format::write(fc.out(), sep));
                else
                    use_separator = true;
                if (auto format_result = m_underlying_.try_format(e, fc); !format_result)
                    return __format::error_t(format_result.error());
                else
                    fc.advance_to(*format_result);
            }
            return __format::write(fc.out(), m_closing_bracket_);
        }

        template <typename R>
        constexpr __format::result<typename format_context::iterator>
        mf_try_format_as_string_(R&& r, format_context& fc) const noexcept {
            __format::formatter_str f;
            if (m_rspec_.pres_kind == __format::__specs::range_pres_kind::_escaped)
                f.set_debug_format();
            if constexpr (std::ranges::contiguous_range<R> && std::ranges::sized_range<R>) {
                return f.try_format(std::string_view { std::ranges::data(r), std::ranges::size(r) }, fc);
            } else {
                __format::str_sink sink;
                __format::write_range(sink.out(), r);
                return f.try_format(std::move(sink).get(), fc);
            }
        }

        template <typename R>
        constexpr auto mf_try_format_range_(R&& r, format_context& fc) const
            noexcept(noexcept(mf_try_format_as_sequence_(r, fc)))
        {
            if constexpr (__is_same(T, char)) {
                switch (m_rspec_.pres_kind) {
                  case __format::__specs::range_pres_kind::_string:
                  case __format::__specs::range_pres_kind::_escaped:
                    return mf_try_format_as_string_(r, fc);
                  default:
                    return mf_try_format_as_sequence_(r, fc);
                }
            } else
                return mf_try_format_as_sequence_(r, fc);
        }

      public:
        range_formatter() = default;

        constexpr void set_separator(std::string_view sep) noexcept {
            m_sep_ = sep;
        }

        constexpr void set_brackets(std::string_view opening, std::string_view closing) noexcept {
            m_opening_bracket_ = opening;
            m_closing_bracket_ = closing;
        }

        constexpr gold::formatter<T>& underlying() noexcept { return m_underlying_; }
        constexpr const gold::formatter<T>& underlying() const noexcept { return m_underlying_; }

        constexpr void set_default_format() {
            // note: set_default_format and parse cannot be called together
            if constexpr (requires { m_underlying_.set_default_format(); })
                m_underlying_.set_default_format();
            if constexpr (requires { m_underlying_.set_debug_format(); })
                m_underlying_.set_debug_format();
        }

        constexpr __format::result<typename format_parse_context::iterator>
        try_parse(format_parse_context& pc) noexcept {
            auto first = pc.begin();
            const auto last = pc.end();
            __format::__specs::range_spec rspec {};
            bool done_parse_range_type = false;
            bool done_parse_has_range_underlying_spec = false;

            auto finalize = [&] -> __format::result<void> {
                if (!rspec.has_underlying_spec) {
                    pc.advance_to(first);
                    if (auto parse_result = m_underlying_.try_parse(pc); !parse_result)
                        return __format::error_t(parse_result.error());
                    else
                        first = *parse_result;
                    if constexpr (requires { m_underlying_.set_debug_format(); })
                        m_underlying_.set_debug_format();
                }
                m_rspec_ = rspec;
                return {};
            };
            auto finished = [&] -> __format::result<bool> {
                if (done_parse_range_type) {
                    if (rspec.pres_kind != __format::__specs::range_pres_kind::_default) {
                        if (rspec.clear_brackets) {
                            if (rspec.pres_kind == __format::__specs::range_pres_kind::_string)
                                return __format::error_t(__fmt_error_code::clear_brackets_and_string_type_cannot_be_used_together);
                            return __format::error_t(__fmt_error_code::clear_brackets_and_debug_string_type_cannot_be_used_together);
                        } else if (rspec.pres_kind == __format::__specs::range_pres_kind::_mapped) {
                            if constexpr (!gold::tuples::pair_like<T>)
                                return __format::error_t(__fmt_error_code::non_pair_like_underlying_used_mapped_type);
                            else {
                                set_brackets("{", "}");
                                m_underlying_.set_separator(": ");
                                m_underlying_.set_brackets({}, {});
                            }
                        } else if (rspec.has_specified_separator()) {
                            if (rspec.pres_kind == __format::__specs::range_pres_kind::_string)
                                return __format::error_t(__fmt_error_code::custom_separator_and_string_type_cannot_be_used_together);
                            else if (rspec.pres_kind == __format::__specs::range_pres_kind::_escaped)
                                return __format::error_t(__fmt_error_code::custom_separator_and_debug_string_type_cannot_be_used_together);
                        }
                        if constexpr (!__is_same(char, T)) {
                            if (rspec.pres_kind == __format::__specs::range_pres_kind::_string)
                                return __format::error_t(__fmt_error_code::non_character_underlying_used_string_type);
                            else if (rspec.pres_kind == __format::__specs::range_pres_kind::_escaped)
                                return __format::error_t(__fmt_error_code::non_character_underlying_used_debug_string_type);
                        }
                    }
                    done_parse_range_type = false;
                }
                if (done_parse_has_range_underlying_spec) {
                    if (rspec.pres_kind != __format::__specs::range_pres_kind::_default && rspec.has_underlying_spec) {
                        if (rspec.pres_kind == __format::__specs::range_pres_kind::_string)
                            return __format::error_t(__fmt_error_code::underlying_fmt_spec_and_string_type_cannot_be_used_together);
                        else if (rspec.pres_kind == __format::__specs::range_pres_kind::_escaped)
                            return __format::error_t(__fmt_error_code::underlying_fmt_spec_and_debug_string_type_cannot_be_used_together);
                    }
                    // parse if has underlying spec
                    if (rspec.has_underlying_spec) {
                        pc.advance_to(first);
                        if (auto parse_result = m_underlying_.try_parse(pc); !parse_result)
                            return __format::error_t(parse_result.error());
                        else
                            first = *parse_result;
                    }
                    done_parse_has_range_underlying_spec = false;
                }
                if (first == last || *first == '}') {
                    if (auto finalized_result = finalize(); !finalized_result)
                        return __format::error_t(finalized_result.error());
                    return true;
                }
                return false;
            };

            if (auto finished_result = finished(); finished_result) {
                if (*finished_result)
                    return first;
            } else {
                return __format::error_t(finished_result.error());
            }

            first = rspec.parse_fill_and_align(first, last);
            if (auto finished_result = finished(); finished_result) {
                if (*finished_result)
                    return first;
            } else {
                return __format::error_t(finished_result.error());
            }

            if (auto parse_result = rspec.try_parse_width(first, last, pc); !parse_result)
                return __format::error_t(parse_result.error());
            else
                first = *parse_result;
            if (auto finished_result = finished(); finished_result) {
                if (*finished_result)
                    return first;
            } else {
                return __format::error_t(finished_result.error());
            }

            if (auto parse_result = rspec.try_parse_separator(first, last, pc); !parse_result)
                return __format::error_t(parse_result.error());
            else
                first = *parse_result;
            if (auto finished_result = finished(); finished_result) {
                if (*finished_result)
                    return first;
            } else {
                return __format::error_t(finished_result.error());
            }

            first = rspec.parse_clear_brackets(first, last);
            if (rspec.clear_brackets)
                set_brackets({}, {});
            if (auto finished_result = finished(); finished_result) {
                if (*finished_result)
                    return first;
            } else {
                return __format::error_t(finished_result.error());
            }

            if (auto parse_result = rspec.try_parse_range_type(first, last); !parse_result)
                return __format::error_t(parse_result.error());
            else
                first = *parse_result;
            done_parse_range_type = true;
            if (auto finished_result = finished(); finished_result) {
                if (*finished_result)
                    return first;
            } else {
                return __format::error_t(finished_result.error());
            }

            first = rspec.parse_has_underlying_spec(first, last);
            done_parse_has_range_underlying_spec = true;
            if (auto finished_result = finished(); finished_result) {
                if (*finished_result)
                    return first;
            } else {
                return __format::error_t(finished_result.error());
            }

            return __format::error_t(__fmt_error_code::failed_to_parse_fmt_spec);
        }

        template <std::ranges::input_range R>
        constexpr __format::result<typename format_context::iterator>
        try_format(R&& r, format_context& fc) const noexcept(noexcept(mf_try_format_range_(r, fc))) {
            if (!m_rspec_.has_width())
                return mf_try_format_range_(r, fc);

            // The size of the buffer needed is:
            // - open bracket characters
            // - close bracket character
            // - n elements where every element may have a different size
            // - (n -1) separators
            // The size of the element is hard to predict, knowing the type helps but
            // it depends on the format-spec. As an initial estimate we guess 6
            // characters.
            // Typically both brackets are 1 character and the separator is 2
            // characters. Which means there will be
            //   (n - 1) * 2 + 1 + 1 = n * 2 character
            // So estimate 8 times the range size as buffer.
            std::size_t capacity_hint = 0;
            if constexpr (std::ranges::sized_range<R>)
                capacity_hint = 8 * std::ranges::size(r);
            __format::str_sink sink;
            sink.reserve(capacity_hint);
            auto artificial_ctx = __format::create_artificial_format_context(fc, sink.out());
            if (auto format_result = mf_try_format_range_(r, artificial_ctx); !format_result)
                return __format::error_t(format_result.error());
            std::string s = std::move(sink).get();
            return __format::try_write_padded_as_spec(s, s.size(), fc, m_rspec_);
        }
    };

} // namespace gold

#endif // __GOLD_BITS_FORMAT_RANGE_FORMATTER_HPP

