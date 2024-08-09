// <gold/bits/format/format_functions.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_FORMAT_FUNCTIONS_HPP
#define __GOLD_BITS_FORMAT_FORMAT_FUNCTIONS_HPP

#include <gold/bits/concepts/string.hpp>
#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/format_arg_kind.hpp>
#include <gold/bits/format/format_args.hpp>
#include <gold/bits/format/format_arg_handle.hpp>
#include <gold/bits/format/format_string.hpp>
#include <gold/bits/format/scanner.hpp>
#include <gold/bits/format/sink.hpp>
#include <gold/bits/type_traits/conditional.hpp>

namespace gold {

    namespace __format {

        /// __format::choose_iter_sink
        template <typename Out, bool Consteval = true>
        struct choose_iter_sink {
            using type = __format::iter_sink<Out>;
        };

        template <std::contiguous_iterator Out>
            requires std::same_as<char, std::iter_value_t<Out>>
        struct choose_iter_sink<Out, false> {
            using type = __format::contiguous_iter_sink<Out>;
        };

        template <typename Seq, bool Consteval>
        struct choose_iter_sink<std::back_insert_iterator<Seq>, Consteval> {
            using type = __format::seq_sink<Seq&>;
        };

        /// __format::choose_iter_sink_t
        template <typename Out, bool Consteval = true>
        using choose_iter_sink_t = choose_iter_sink<Out, Consteval>::type;

        /// __format::try_do_vformat_to
        template <typename Out>
        constexpr __format::result<Out> try_do_vformat_to(Out out, std::string_view fmt, const format_args& args) {
            if consteval {
                using Sink = __format::choose_iter_sink_t<Out, true>;

                Sink sink (std::move(out));
                __format::sink_iterator sink_out;

                if constexpr (__is_same(__format::sink_iterator, Out))
                    sink_out = out;
                else
                    sink_out = sink.out();

                auto ctx = __format::create_format_context(&args, sink_out);
                __format::formatting_scanner scanner (ctx, fmt);
                if (auto scanner_result = scanner.try_scan(); !scanner_result)
                    return __format::error_t(scanner_result.error());

                if constexpr (__is_same(__format::sink_iterator, Out))
                    return ctx.out();
                else
                    return std::move(sink).finish().out;
            } else {
                using Sink = __format::choose_iter_sink_t<Out, false>;

                Sink sink (std::move(out));
                __format::sink_iterator sink_out;

                if constexpr (__is_same(__format::sink_iterator, Out))
                    sink_out = out;
                else
                    sink_out = sink.out();

                auto ctx = __format::create_format_context(&args, sink_out);
                __format::formatting_scanner scanner (ctx, fmt);
                if (auto scanner_result = scanner.try_scan(); !scanner_result)
                    return __format::error_t(scanner_result.error());

                if constexpr (__is_same(__format::sink_iterator, Out))
                    return ctx.out();
                else
                    return std::move(sink).finish().out;
            }
        }

        /// __format::do_vformat_to
        template <typename Out>
        constexpr Out do_vformat_to(Out out, std::string_view fmt, const format_args& args) {
            if (auto result = __format::try_do_vformat_to(std::move(out), fmt, args); result)
                return *result;
            else
                __fmt_error::on_error_with(result.error());
        }

    } // namespace __format

    /// vformat_to
    template <__format::char_output_iterator Out>
    [[gnu::always_inline]] inline constexpr Out vformat_to(Out out, std::string_view fmt, format_args args) {
        return __format::do_vformat_to(std::move(out), fmt, args);
    }

    /// vformat
    [[nodiscard]] inline constexpr std::string vformat(std::string_view fmt, format_args args) {
        __format::str_sink buf;
        gold::vformat_to(buf.out(), fmt, args);
        return std::move(buf).get();
    }

    /// format
    template <gold::formattable... Args>
    [[nodiscard]] inline constexpr std::string format(gold::format_string<Args...> fmt, Args&&... args) {
        return gold::vformat(fmt.get(), gold::make_format_args(args...));
    }

    /// try_format
    template <gold::formattable... Args>
    [[nodiscard]] inline constexpr try_format_result<std::string> try_format(std::string_view fmt, Args&&... args) {
        __format::str_sink buf;
        if (auto result = __format::try_do_vformat_to(buf.out(), fmt, gold::make_format_args(args...)); !result)
            return __format::error_t(result.error());
        return try_format_result<std::string>(std::in_place, std::move(buf).get());
    }

    /// format_to
    template <__format::char_output_iterator Out, gold::formattable... Args>
    inline constexpr Out format_to(Out out, gold::format_string<Args...> fmt, Args&&... args) {
        return gold::vformat_to(std::move(out), fmt.get(), gold::make_format_args(args...));
    }

    /// format_to_n
    template <__format::char_output_iterator Out, gold::formattable... Args>
    inline constexpr gold::format_to_n_result<Out> format_to_n(Out out, std::iter_difference_t<Out> n,
                                                               gold::format_string<Args...> fmt,
                                                               Args&&... args) {
        if consteval {
            using Sink = __format::choose_iter_sink_t<Out, true>;
            Sink sink (std::move(out), n);
            gold::vformat_to(sink.out(), fmt.get(), gold::make_format_args(args...));
            return std::move(sink).finish();
        } else {
            using Sink = __format::choose_iter_sink_t<Out, false>;
            Sink sink (std::move(out), n);
            gold::vformat_to(sink.out(), fmt.get(), gold::make_format_args(args...));
            return std::move(sink).finish();
        }
    }

    /// formatted_size
    template <gold::formattable... Args>
    [[nodiscard]] inline constexpr std::size_t formatted_size(gold::format_string<Args...> fmt, Args&&... args) {
        __format::counting_sink buf;
        gold::vformat_to(buf.out(), fmt.get(), gold::make_format_args(args...));
        return buf.count();
    }

    /// to_formatted
    template <__format::char_output_iterator Out, typename Arg>
        requires gold::string_like<Arg> ||
                 gold::formattable<Arg>
    inline constexpr Out to_formatted(Out out, Arg&& arg) {

        // immature implementation:
        // return gold::format_to(out, "{}", arg);

        // optimized implementation (skipping the parsing process):
        if constexpr (gold::string_like<Arg>) {
            return __format::write(std::move(out), std::string_view(arg));
        } else {
            __format::choose_iter_sink_t<Out> sink (std::move(out));
            __format::sink_iterator sink_out = sink.out();
            auto ctx = __format::create_format_context(nullptr, sink_out);
            gold::formatter<__remove_cvref(Arg)> f {};
            if constexpr (requires { f.set_default_options(); })
                f.set_default_options();
            f.try_format(std::forward<Arg>(arg), ctx);
            return std::move(sink).finish().out;
        }
    }

    /// to_formatted_string
    template <typename Arg>
        requires gold::string_like<Arg> ||
                 gold::formattable<Arg>
    inline constexpr std::string to_formatted_string(Arg&& arg) {

        // immature implementation:
        // return gold::format("{}", arg);

        // optimized implementation (skipping the parsing process):
        if constexpr (gold::string_like<Arg>) {
            if constexpr (__is_same(std::string, __remove_cvref(Arg)))
                return std::forward<Arg>(arg);
            else if constexpr (__is_same(std::string_view, __remove_cvref(Arg)))
                return std::string (arg);
            else if constexpr (__is_constructible(std::string, Arg))
                return std::string (std::forward<Arg>(arg));
            else {
                std::string_view op (std::forward<Arg>(arg));
                return std::string (op);
            }
        } else {
            __format::str_sink sink;
            __format::sink_iterator sink_out = sink.out();
            auto ctx = __format::create_format_context(nullptr, sink_out);
            gold::formatter<__remove_cvref(Arg)> f {};
            if constexpr (requires { f.set_default_format(); })
                f.set_default_format();
            f.try_format(std::forward<Arg>(arg), ctx);
            return std::move(sink).get();
        }
    }

    /// try_formatter_parse_result
    using try_formatter_parse_result = try_format_result<typename format_parse_context::iterator>;

    /// try_formatter_write_result
    using try_formatter_write_result = try_format_result<typename format_context::iterator>;

} // namespace gold

#endif // __GOLD_BITS_FORMAT_FORMAT_FUNCTIONS_HPP
