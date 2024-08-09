// <gold/bits/format/tuple_formatter.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_TUPLE_FORMATTER_HPP
#define __GOLD_BITS_FORMAT_TUPLE_FORMATTER_HPP

#include <tuple>
#include <gold/pack>
#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/concepts.hpp>
#include <gold/bits/format/parser.hpp>
#include <gold/bits/format/formatter.hpp>
#include <gold/bits/format/error.hpp>
#include <gold/bits/format/sink.hpp>
#include <gold/bits/tuples/concepts.hpp>
#include <gold/bits/tuples/get.hpp>
#include <gold/bits/tuples/size.hpp>

namespace gold {

    namespace __format {

        namespace __specs {

            /// __format::__specs::tuple_pres_kind
            enum class tuple_pres_kind : gold::uint8_t {
                _default,
                _mapped,
                _clear_brackets
            };

            /// __format::__specs::tuple_spec
            struct tuple_spec : spec_base {
                wp_kind sep_kind          : 2 = wp_kind::_none;
                tuple_pres_kind pres_kind : 2 = tuple_pres_kind::_default;
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
                        __GOLDM_FMT_TRY(pc.try_next_arg_id(), { sep_arg_id = try_result; });
                        __GOLDM_FMT_TRYV(pc.try_check_dynamic_spec_string(sep_arg_id), {});
                    } else {
                        auto [v, ptr] = __parse::parse_arg_id(first, last);
                        if (ptr == nullptr || ptr == last || *ptr != '}')
                            return __format::error_t(__fmt_error_code::invalid_arg_id_in_fmt_string);
                        first = ptr;
                        __GOLDM_FMT_TRYV(pc.try_check_arg_id(v), {});
                        __GOLDM_FMT_TRYV(pc.try_check_dynamic_spec_string(v), {});
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

                constexpr iterator parse_tuple_type(iterator first, iterator last) noexcept {
                    switch (*first) {
                      case 'm':
                        pres_kind = tuple_pres_kind::_mapped;
                        ++first;
                        break;
                      case 'n':
                        pres_kind = tuple_pres_kind::_clear_brackets;
                        ++first;
                        break;
                    }
                    return first;
                }
            };

        } // namespace __specs

    } // namespace __format

    /// tuple_formatter
    // [:tuple-format-spec:]
    //      | [:fill-and-align (opt):] [:width (opt):] [:sep (opt):] [:tuple-type (opt):]

    // [:fill-and-align:]
    //      | any character other than '{', '}', or ':'

    // [:sep:]
    //      | '~' '{' [:arg-id:] ? '}'

    // [:tuple-type:]
    //      | 'm' | 'n'

    template <gold::formattable... Ts>
    struct tuple_formatter {
      private:
        __format::__specs::tuple_spec                     m_tspec_ {};
        std::tuple<formatter<std::remove_cvref_t<Ts>>...> m_underlying_;
        std::string_view m_sep_ = ", ";
        std::string_view m_opening_bracket_ = "(";
        std::string_view m_closing_bracket_ = ")";

        template <typename Tuple>
        constexpr __format::result<typename format_context::iterator>
        mf_try_format_tuple_(Tuple&& tup, format_context& fc) const /* TODO: add noexcept */ {
            fc.advance_to(__format::write(fc.out(), m_opening_bracket_));
            std::string_view sep = m_sep_;
            if (m_tspec_.has_specified_separator())
                __GOLDM_FMT_TRY(m_tspec_.try_get_separator(fc), { sep = try_result; });
            __format::error_code ec = __fmt_error_code::none;
            gold::pack_for_each_index<sizeof...(Ts)>([&]<std::size_t I> {
                if (ec == __fmt_error_code::none) {
                    if constexpr (I)
                        fc.advance_to(__format::write(fc.out(), sep));
                    if (auto format_result = std::get<I>(m_underlying_).try_format(gold::tuples::get<I>(tup), fc); !format_result)
                        ec = format_result.error();
                    else
                        fc.advance_to(*format_result);
                }
            });
            if (ec != __fmt_error_code::none)
                return __format::error_t(ec);
            return __format::write(fc.out(), m_closing_bracket_);
        }

      public:
        constexpr tuple_formatter() = default;

        constexpr void set_separator(std::string_view sep) noexcept {
            m_sep_ = sep;
        }

        constexpr void set_brackets(std::string_view opening, std::string_view closing) noexcept {
            m_opening_bracket_ = opening;
            m_closing_bracket_ = closing;
        }

        constexpr void set_default_format() {
            // note: set_default_options and parse cannot be called together
            gold::pack_for_each_index<sizeof...(Ts)>([&]<std::size_t I> {
                auto& f = std::get<I>(m_underlying_);
                if constexpr (requires { f.set_default_format(); })
                    f.set_default_format();
                if constexpr (requires { f.set_debug_format(); })
                    f.set_debug_format();
            });
        }

        constexpr __format::result<typename format_parse_context::iterator>
        try_parse(format_parse_context& pc) noexcept {
            auto first = pc.begin();
            const auto last = pc.end();
            __format::__specs::tuple_spec tspec {};
            bool done_parse_tuple_type = false;

            auto finalize = [&] -> __format::result<void> {
                m_tspec_ = tspec;
                pc.advance_to(first);
                __format::error_code ec = __fmt_error_code::none;
                gold::pack_for_each_index<sizeof...(Ts)>([&]<std::size_t I> {
                    if (ec == __fmt_error_code::none) {
                        auto& f = std::get<I>(m_underlying_);
                        if (auto parse_result = f.try_parse(pc); !parse_result)
                            ec = parse_result.error();
                        else
                            first = *parse_result;
                        if constexpr (requires { f.set_debug_format(); })
                            f.set_debug_format();
                    }
                });
                if (ec != __fmt_error_code::none)
                    return __format::error_t(ec);
                return {};
            };
            auto finished = [&] -> __format::result<bool> {
                if (done_parse_tuple_type) {
                    if (tspec.pres_kind == __format::__specs::tuple_pres_kind::_mapped) {
                        if constexpr (sizeof...(Ts) != 2)
                            return __format::error_t(__fmt_error_code::non_pair_like_underlying_used_mapped_type);
                        else if (tspec.has_specified_separator())
                            return __format::error_t(__fmt_error_code::custom_separator_and_mapped_type_cannot_be_used_together);
                        set_separator(": ");
                        set_brackets({}, {});
                    } else if (tspec.pres_kind == __format::__specs::tuple_pres_kind::_clear_brackets)
                        set_brackets({}, {});
                    done_parse_tuple_type = false;
                }
                if (first == last || *first == '}') {
                    if (auto finalized_result = finalize(); !finalized_result)
                        return __format::error_t(finalized_result.error());
                    return true;
                }
                return false;
            };

            __GOLDM_FMT_TRY(finished(), {
                if (try_result)
                    return first;
            });

            first = tspec.parse_fill_and_align(first, last);
            __GOLDM_FMT_TRY(finished(), {
                if (try_result)
                    return first;
            });

            __GOLDM_FMT_TRY((tspec.try_parse_width(first, last, pc)), { first = try_result; });
            __GOLDM_FMT_TRY(finished(), {
                if (try_result)
                    return first;
            });

            __GOLDM_FMT_TRY((tspec.try_parse_separator(first, last, pc)), { first = try_result; });
            __GOLDM_FMT_TRY(finished(), {
                if (try_result)
                    return first;
            });

            first = tspec.parse_tuple_type(first, last);
            done_parse_tuple_type = true;
            __GOLDM_FMT_TRY(finished(), {
                if (try_result)
                    return first;
            });

            return __format::error_t(__fmt_error_code::failed_to_parse_fmt_spec);
        }

        template <typename Tuple>
        constexpr __format::result<typename format_context::iterator>
        try_format(Tuple&& tup, format_context& fc) const /* TODO: add noexcept */ {
            if (!m_tspec_.has_width())
                return mf_try_format_tuple_(tup, fc);

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
            std::size_t capacity_hint = 8 * gold::tuples::size(tup);
            __format::str_sink sink;
            sink.reserve(capacity_hint);
            auto artificial_ctx = __format::create_artificial_format_context(fc, sink.out());
            __GOLDM_FMT_TRYV((mf_try_format_tuple_(tup, artificial_ctx)), {});
            std::string s = std::move(sink).get();
            return __format::try_write_padded_as_spec(s, s.size(), fc, m_tspec_);
        }
    };

    template <gold::formattable T, gold::formattable U>
    struct formatter<std::pair<T, U>> : tuple_formatter<T, U> {};

    template <gold::formattable... Ts>
    struct formatter<std::tuple<Ts...>> : tuple_formatter<Ts...> {};

} // namespace gold

#endif // __GOLD_BITS_FORMAT_TUPLE_FORMATTER_HPP
