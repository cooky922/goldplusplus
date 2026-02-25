// <gold/preview/bits/format/fmt_string.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: this is only a prototype
// note: this is an internal header

#pragma once
#ifndef __GOLD_PREVIEW_BITS_FMT_STRING_HPP
#define __GOLD_PREVIEW_BITS_FMT_STRING_HPP

#include <gold/assertion>
#include <gold/preview/bits/format/fmt_error.hpp>
#include <gold/preview/bits/format/fmt_arg.hpp>

#include <cstddef>
#include <cstdint>
#include <limits>

namespace gold::preview {

    namespace __format::__detail {

        /// parse_number_result_
        template <typename CharT>
        struct parse_number_result_ {
            const CharT* ptr_;
            std::uint32_t value_;
        };

        /// string_iter_t_
        template <typename CharT>
        struct string_iter_pair_t_ {
            const CharT* begin_;
            const CharT* end_;
        };

        /// parse_number_
        template <typename CharT>
        constexpr parse_number_result_<CharT> parse_number_(string_iter_pair_t_<CharT>*);

        /**
         * The maximum value of a numeric argument.
         *
         * This is used for:
         * * arg-id
         * * width as value or arg-id.
         * * precision as value or arg-id.
         *
         * The value is compatible with the maximum formatting width and precision
         * using the `%*` syntax on a 32-bit system.
         */

        /// number_max_
        inline constexpr std::uint32_t number_max_ = std::numeric_limits<std::uint32_t>::max();

        /// parse_zero_
        template <typename CharT>
        constexpr parse_number_result_<CharT> parse_zero_(string_iter_pair_t_<CharT>* iter_pair, auto& parse_ctx) {
            parse_ctx.check_arg_id(0);
            return { ++iter_pair->begin_, 0 };
        }

        /// parse_automatic_
        template <typename CharT>
        constexpr parse_number_result_<CharT> parse_automatic_(string_iter_pair_t_<CharT>* iter_pair, auto& parse_ctx) {
            std::size_t value_ = parse_ctx.next_arg_id();
            gold::system_assert(value_ <= number_max_, "Compilers don't support this number of arguments.");
            return { iter_pair->begin_, std::uint32_t(value_) };
        }

        /// parse_manual_
        template <typename CharT>
        constexpr parse_number_result_<CharT> parse_manual_(string_iter_pair_t_<CharT>* iter_pair, auto& parse_ctx) {
            auto r_ = parse_number_(iter_pair);
            parse_ctx.check_arg_id(r_.value_);
            return r_;
        }

         /**
         * Parses a number.
         *
         * The number is used for the 31-bit values @em width and @em precision. This
         * allows a maximum value of 2147483647.
         */

        /// parse_number_
        template <typename CharT>
        constexpr parse_number_result_<CharT> parse_number_(string_iter_pair_t_<CharT>* iter_pair) {

            auto& begin_      = iter_pair->begin_;
            auto& end_input_  = iter_pair->end_;
            const CharT* end_ = end_input_ - begin_ > 9 ? begin_ + 9 : end_input_;

            std::uint32_t value_ = *begin_ - CharT('0');

            while (++begin_ != end_) {
                if (*begin_ < CharT('0') || *begin_ > CharT('9'))
                return { begin_, value_ };

                value_ = value_ * 10 + *begin_ - CharT('0');
            }

            if (begin_ != end_input_ && *begin_ >= '0' && *end_input_ <= '9') {

                /*
                 * There are more than 9 digits, do additional validations:
                 * - Does the 10th digit exceed the maximum allowed value?
                 * - Are there more than 10 digits?
                 * (More than 10 digits always overflows the maximum.)
                 */

                 std::uint64_t v_ = std::uint64_t(value_) * 10 + *iter_pair->begin_++ - CharT('0');

                 if (v_ > number_max_ || (begin_ != end_input_ && *begin_ >= CharT('0') && *begin_ <= CharT('9')))
                    throw gold::preview::format_error("The numeric value of the format-spec is too large");

                 value_ = v_;
            }

            return { begin_, value_ };

        }

        /**
         * Multiplexer for all parse functions.
         *
         * The parser will return a pointer beyond the last consumed character. This
         * should be the closing '}' of the arg-id.
         */

        /// parse_arg_id_
        template <typename CharT>
        constexpr parse_number_result_<CharT> parse_arg_id_(string_iter_pair_t_<CharT>* iter_pair, auto& parse_ctx) {

            switch (*iter_pair->begin_) {
              case CharT('0'):
                return parse_zero_(iter_pair, parse_ctx);

              case CharT(':'):
                // This case is conditionally valid. It's allowed in an arg-id in the
                // replacement-field, but not in the std-format-spec. The caller can
                // provide a better diagnostic, so accept it here unconditionally.
              case CharT('}'):
                return parse_automatic_(iter_pair, parse_ctx);
            }

            if (*iter_pair->begin_ < CharT('0') || *iter_pair->begin_ > CharT('9'))
                throw gold::preview::format_error("The arg-id of the format-spec starts with an invalid character");

            return parse_manual_(iter_pair, parse_ctx);
        }

        /// handle_replacement_field_
        template <typename CharT, typename ParseCtx, typename Ctx>
        const CharT* handle_replacement_field_(string_iter_pair_t_<CharT>* iter_pair,
                                               ParseCtx& parse_ctx, Ctx& ctx) {

            auto r_ = parse_arg_id_(iter_pair, parse_ctx);

            switch (*r_.ptr_) {
              case CharT(':'):
                // The arg-id has a format-specifier, advance the input to the format-spec.
                parse_ctx.advance_to(r_.ptr_ + 1);
                break;
              case CharT('}'):
                // The arg-id has no format-specifier.
                parse_ctx.advance_to(r_.ptr_);
                break;
              default:
                throw gold::preview::format_error("The replacement field arg-id should terminate at a ':' or '}'");
            }

            gold::preview::visit_format_arg(
                [&]<typename T>(T arg) {
                    if constexpr (std::same_as<T, std::monostate>)
                        throw gold::preview::format_error("Argument index out of bounds");
                    else if constexpr (std::same_as<T, typename gold::preview::basic_format_arg<Ctx>::handle>) {
                        arg.format(parse_ctx, ctx);
                    } else {
                        formatter<T, CharT> formatter_;
                        parse_ctx.advance_to(formatter_.parse(parse_ctx));
                        ctx.advance_to(formatter_.format(arg, ctx));
                    }
                },
                ctx.arg(r_.value_)
            );

            iter_pair->begin_ = parse_ctx.begin();
            if (iter_pair->begin_ == iter_pair->end_ || *iter_pair->begin_ != CharT('}'))
                throw gold::preview::format_error("The replacement field misses a terminating '}'");

            return ++iter_pair->begin_;
        }

    } // namespace __format::__detail

} // namespace gold::preview

#endif // __GOLD_PREVIEW_BITS_FMT_STRING_HPP
