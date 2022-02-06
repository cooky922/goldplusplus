// <gold/bits/format/formatter.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_FORMATTER_HPP
#define __GOLD_BITS_FORMAT_FORMATTER_HPP

#include <gold/bits/concepts/types.hpp>
#include <gold/bits/format/fmt_specs.hpp>

#include <compare>
#include <string_view>
#include <charconv>
#include <limits>
#include <bits/iterator_concepts.h>
#include <bits/ranges_algobase.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

namespace gold {

    namespace __format {

        /// char_output_iterator
        template <typename T>
        concept char_output_iterator = std::output_iterator<std::remove_reference_t<T>, char>;

        namespace __detail {

            /// is_ints_only_v_
            template <typename T>
            inline constexpr bool is_ints_only_v_ = gold::qualified_numeric<T> && std::integral<T>;

            /// is_floats_only_v_
            template <typename T>
            inline constexpr bool is_floats_only_v_ = gold::qualified_numeric<T> && std::floating_point<T>;

        } // namespace __detail

        /// plain_char_formatter_
        struct plain_char_formatter_ {

            template <gold::__format::char_output_iterator Out>
            constexpr Out format_to(Out out, char c, format_specs = {}) const noexcept {
                *out++ = c;
                return out;
            }

        };

        /// plain_char_formatter_v_
        inline constexpr plain_char_formatter_ plain_char_formatter_v_ {};

        /// plain_cstr_formatter_
        struct plain_cstr_formatter_ {

            template <gold::__format::char_output_iterator Out>
            constexpr Out format_to(Out out, const char* s, format_specs = {}) const noexcept {
                while (*s != '\0')
                    *out++ = *s++;
                return out;
            }

        };

        /// plain_cstr_formatter_v_
        inline constexpr plain_cstr_formatter_ plain_cstr_formatter_v_ {};

        /// integral_formatter_
        template <typename T>
            requires __detail::is_ints_only_v_<T>
        struct integral_formatter_ {

            template <gold::__format::char_output_iterator Out>
            constexpr Out format_to(Out out, T n, format_specs specs = {}) const noexcept {

                /// for character type
                if (specs.type == format_specs::type_t::character)
                    return plain_char_formatter_v_.format_to(out, static_cast<char>(n));

                /// default: decimal
                std::size_t counter_ { 0 };

                /// for signs [ works for signed types only ]
                if constexpr (std::is_signed_v<T>) {
                    switch (specs.sign) {
                      case format_specs::sign_t::space:
                        if (n >= 0)
                            *out++ = ' ';
                        break;

                      case format_specs::sign_t::plus:
                        if (n >= 0)
                            *out++ = '+';
                        break;

                      default:;
                    }

                    if (n < 0) {
                        n *= -1;
                        *out++ = '-';
                    }
                }

                /// for bases [ alternate form ]
                if (specs.show_base) {
                    switch (specs.type) {
                      case format_specs::type_t::binary_lower:
                        out = plain_cstr_formatter_v_.format_to(out, "0b");
                        break;

                      case format_specs::type_t::binary_upper:
                        out = plain_cstr_formatter_v_.format_to(out, "0B");
                        break;

                      case format_specs::type_t::octal:
                        out = plain_char_formatter_v_.format_to(out, '0');
                        break;

                      case format_specs::type_t::hex_lower:
                        out = plain_cstr_formatter_v_.format_to(out, "0x");
                        break;

                      case format_specs::type_t::hex_upper:
                        out = plain_cstr_formatter_v_.format_to(out, "0X");
                        break;

                      default:;
                    }
                }

                /// if equal to zero
                if (n == 0) {
                    *out++ = '0';
                    return out;
                }

                char temp_out_[std::same_as<T, __int128> ? 140 : 70]; // ~ estimated size to hold at most binary format
                char* temp_begin_ = temp_out_;

                while (n > 0) {
                    switch (specs.type) {
                      case format_specs::type_t::binary_lower:
                      case format_specs::type_t::binary_upper:
                        *temp_begin_++ = (n & 1) + '0';
                        n >>= 1;
                        break;
                      case format_specs::type_t::octal:
                        *temp_begin_++ = (n & 7) + '0';
                        n >>= 3;
                        break;
                      case format_specs::type_t::hex_lower:
                        *temp_begin_++ = "0123456789abcdef"[n & 15];
                        n >>= 4;
                        break;
                      case format_specs::type_t::hex_upper:
                        *temp_begin_++ = "0123456789ABCDEF"[n & 15];
                        n >>= 4;
                        break;
                      case format_specs::type_t::decimal:
                      case format_specs::type_t::default_opt:
                      default:
                        *temp_begin_++ = n % 10 + '0';
                        n /= 10;
                    }
                    ++counter_;
                }

                *temp_begin_++ = '\0';

                for (std::size_t i = 0; i < counter_ / 2; ++i) {
                    std::swap(temp_out_[i], temp_out_[counter_ - i - 1]);
                }

                return out = plain_cstr_formatter_v_.format_to(out, temp_out_);
            }

        };

        /// integral_formatter_v_
        template <typename T>
            requires __detail::is_ints_only_v_<T>
        inline constexpr integral_formatter_<T> integral_formatter_v_ {};

        /// floating_point_formatter_
        template <typename T>
            requires __detail::is_floats_only_v_<T>
        struct floating_point_formatter_ {

            struct double_wrapper_ {
                __int128_t whole { 0 }, frac { 0 };
                constexpr double_wrapper_(long double fv, int prec = 6)
                : whole(static_cast<__int128_t>(fv)) {
                    fv -= whole;
                    for (int i = 0; i < prec; ++i)
                        fv *= 10;
                    frac = static_cast<__int128_t>(fv);
                }
            };

            template <gold::__format::char_output_iterator Out>
            constexpr Out format_to(Out out, T n, format_specs specs = {}) const noexcept {

                if (std::is_constant_evaluated()) {

                    /// still unstable

                    if (n <=> 0.0 == std::partial_ordering::unordered) {
                        out = plain_cstr_formatter_v_.format_to(out, "NaN");
                        return out;
                    } else if (n <=> std::numeric_limits<T>::infinity() == 0) {
                        out = plain_cstr_formatter_v_.format_to(out, "inf");
                        return out;
                    } else if (n <=> -std::numeric_limits<T>::infinity() == 0) {
                        out = plain_cstr_formatter_v_.format_to(out, "-inf");
                        return out;
                    }

                    double_wrapper_ num_ { static_cast<long double>(n), specs.precision != -1 ? specs.precision : 6 };

                    if (num_.frac < 0 || num_.whole < 0)
                        *out++ = '-';

                    if (num_.frac < 0)
                        num_.frac = -num_.frac;

                    if (num_.whole < 0)
                        num_.whole = -num_.whole;

                    out = integral_formatter_v_<__int128_t>.format_to(out, num_.whole);

                    *out++ = '.';

                    out = integral_formatter_v_<__int128_t>.format_to(out, num_.frac);

                    return out;

                } else {
                    /// unstable, and wrong case output

                    std::chars_format fmt;

                    switch (specs.type) {
                      case format_specs::type_t::scientific_lower:
                      case format_specs::type_t::scientific_upper:
                        fmt = std::chars_format::scientific;
                        break;

                      case format_specs::type_t::fixed_lower:
                      case format_specs::type_t::fixed_upper:
                        fmt = std::chars_format::fixed;
                        break;

                      case format_specs::type_t::float_hex_lower:
                      case format_specs::type_t::float_hex_upper:
                        fmt = std::chars_format::hex;
                        break;

                      case format_specs::type_t::general_lower:
                      case format_specs::type_t::general_upper:
                      case format_specs::type_t::default_opt:
                      default:
                        fmt = std::chars_format::general;
                        break;
                    }

                // unstable NaN formatting

                    if (n >= 0.0) {
                        switch (specs.sign) {
                          case format_specs::sign_t::space:
                            *out++ = ' ';
                            break;

                          case format_specs::sign_t::plus:
                            *out++ = '+';
                            break;

                          default:;
                        }
                    }

                    char temp_buffer_[85] {};

                    if (specs.precision == -1) {
                        std::to_chars(temp_buffer_, temp_buffer_ + 85, n, fmt);
                    } else {
                        std::to_chars(temp_buffer_, temp_buffer_ + 85, n, fmt, specs.precision);
                    }

                    return out = plain_cstr_formatter_v_.format_to(out, temp_buffer_);

                }
            }

        };

        /// floating_point_formatter_v_
        template <typename T>
            requires __detail::is_floats_only_v_<T>
        inline constexpr floating_point_formatter_<T> floating_point_formatter_v_ {};

        /// void_pointer_formatter_
        struct void_pointer_formatter_ {

            template <gold::__format::char_output_iterator Out>
            constexpr Out format_to(Out out, const void* ptr, format_specs specs = {}) const noexcept {
                if (std::is_constant_evaluated()) {
                    return plain_cstr_formatter_v_.format_to(out, "<unknown address at compile-time>");
                } else {
                    specs.type = format_specs::type_t::hex_lower;
                    specs.show_base = true;
                    return integral_formatter_v_<std::uintptr_t>.format_to(out, reinterpret_cast<std::uintptr_t>(ptr), std::move(specs));
                }
            }

        };

        /// void_pointer_formatter_v_
        inline constexpr void_pointer_formatter_ void_pointer_formatter_v_ {};

    } // namespace __format

    /// formatter
    template <typename>
    struct formatter {
        inline static constexpr bool is_specialized = false;
        inline static constexpr bool is_customized  = false;
    };

    /// formatter<char>
    template <>
    struct formatter<char> {
        inline static constexpr bool is_specialized = true;
        inline static constexpr bool is_customized  = false;

        template <gold::__format::char_output_iterator Out>
        constexpr Out format_to(Out out, char c, format_specs specs = {}) const noexcept {
            switch (specs.type) {
              case format_specs::type_t::binary_lower:
              case format_specs::type_t::binary_upper:
              case format_specs::type_t::octal:
              case format_specs::type_t::decimal:
              case format_specs::type_t::hex_lower:
              case format_specs::type_t::hex_upper:
                out = __format::plain_char_formatter_v_.format_to(out, c, std::move(specs));
                break;

              case format_specs::type_t::character_quoted:
                *out++ = '\'';
                *out++ = c;
                *out++ = '\'';
                break;

              case format_specs::type_t::character:
              case format_specs::type_t::default_opt:
              default:
                *out++ = c;
            }
            return out;
        }
    };

    /// formatter<const char*>
    template <>
    struct formatter<const char*> {
        inline static constexpr bool is_specialized = true;
        inline static constexpr bool is_customized  = false;

        template <gold::__format::char_output_iterator Out>
        constexpr Out format_to(Out out, const char* src, format_specs specs = {}) const noexcept {
            switch (specs.type) {
              case format_specs::type_t::pointer:
                return __format::void_pointer_formatter_v_.format_to(out, static_cast<const void*>(src), std::move(specs));

              case format_specs::type_t::string_quoted:
                *out++ = '"';
                out = __format::plain_cstr_formatter_v_.format_to(out, src);
                *out++ = '"';
                return out;

              case format_specs::type_t::string:
              case format_specs::type_t::default_opt:
              default:
                return __format::plain_cstr_formatter_v_.format_to(out, src);
            }
        }
    };

    /// formatter<std::string_view>
    template <>
    struct formatter<std::string_view> {
        inline static constexpr bool is_specialized = true;
        inline static constexpr bool is_customized  = false;

        template <gold::__format::char_output_iterator Out>
        constexpr Out format_to(Out out, std::string_view src, format_specs specs = {}) const noexcept {
            switch (specs.type) {
              case format_specs::type_t::string_quoted:
                *out++ = '"';
                for (char c : src)
                    *out++ = c;
                *out++ = '"';
                break;

              case format_specs::type_t::string:
              case format_specs::type_t::default_opt:
              default:
                for (char c : src)
                    *out++ = c;
            }
            return out;
        }

    };

    /// formatter<std::string>
    template <>
    struct formatter<std::string> : formatter<std::string_view> {
        inline static constexpr bool is_specialized = true;
        inline static constexpr bool is_customized  = false;

        template <gold::__format::char_output_iterator Out>
        constexpr Out format_to(Out out, const std::string& src, format_specs specs = {}) const noexcept {
            return formatter<std::string_view>::format_to(out, src, std::move(specs));
        }

    };

    /// formatter<bool>
    template <>
    struct formatter<bool> {
        inline static constexpr bool is_specialized = true;
        inline static constexpr bool is_customized  = false;

        template <gold::__format::char_output_iterator Out>
        constexpr Out format_to(Out out, bool b, format_specs specs = {}) const noexcept {
            switch (specs.type) {
              case format_specs::type_t::binary_lower:
              case format_specs::type_t::binary_upper:
              case format_specs::type_t::octal:
              case format_specs::type_t::decimal:
              case format_specs::type_t::hex_lower:
              case format_specs::type_t::hex_upper:
                *out++ = b ? '1' : '0';
                return out;

              case format_specs::type_t::string:
              case format_specs::type_t::default_opt:
              default:
                return __format::plain_cstr_formatter_v_.format_to(out, b ? "true" : "false", std::move(specs));
            }
        }
    };

    /// formatter<std::nullptr_t>
    template <>
    struct formatter<std::nullptr_t> {
        inline static constexpr bool is_specialized = true;
        inline static constexpr bool is_customized  = false;

        template <gold::__format::char_output_iterator Out>
        constexpr Out format_to(Out out, std::nullptr_t, format_specs specs = {}) const noexcept {
            switch (specs.type) {
              case format_specs::type_t::pointer:
                specs.show_base = true;
                specs.type = format_specs::type_t::hex_lower;
                return __format::integral_formatter_v_<int>.format_to(out, 0, std::move(specs));

              case format_specs::type_t::string:
              case format_specs::type_t::default_opt:
              default:
                return __format::plain_cstr_formatter_v_.format_to(out, "nullptr");
            }
        }
    };

    /// formatter<const void*>
    template <>
    struct formatter<const void*> : __format::void_pointer_formatter_ {
        inline static constexpr bool is_specialized = true;
        inline static constexpr bool is_customized  = false;
    };

    /// formatter<Integral>
    template <typename T>
        requires __format::__detail::is_ints_only_v_<T>
    struct formatter<T> : __format::integral_formatter_<T> {
        inline static constexpr bool is_specialized = true;
        inline static constexpr bool is_customized  = false;
    };

    /// formatter<FloatingPoint>
    template <typename T>
        requires __format::__detail::is_floats_only_v_<T>
    struct formatter<T> : __format::floating_point_formatter_<T> {
        inline static constexpr bool is_specialized = true;
        inline static constexpr bool is_customized  = false;
    };

    /// formatter<OrderingType>
    template <typename T>
        requires gold::ordering_type<T>
    struct formatter<T> {
        inline static constexpr bool is_specialized = true;
        inline static constexpr bool is_customized  = false;

        template <gold::__format::char_output_iterator Out>
        constexpr Out format_to(Out out, T ord, format_specs specs = {}) const noexcept {
            constexpr bool is_strong_ordering_  = std::is_same_v<T, std::strong_ordering>;
            constexpr bool is_weak_ordering_    = std::is_same_v<T, std::weak_ordering>;
            constexpr bool is_partial_ordering_ = std::is_same_v<T, std::partial_ordering>;

            switch (specs.type) {
              case format_specs::type_t::binary_lower:
              case format_specs::type_t::binary_upper:
              case format_specs::type_t::octal:
              case format_specs::type_t::decimal:
              case format_specs::type_t::hex_lower:
              case format_specs::type_t::hex_upper:
                if constexpr (is_strong_ordering_) {
                    if (ord == std::strong_ordering::less)
                        out = __format::integral_formatter_v_<int>.format_to(out, -1, std::move(specs));
                    else if (ord == std::strong_ordering::equivalent ||
                               ord == std::strong_ordering::equal)
                        out = __format::integral_formatter_v_<int>.format_to(out, 0, std::move(specs));
                    else if (ord == std::strong_ordering::greater)
                        out = __format::integral_formatter_v_<int>.format_to(out, 1, std::move(specs));
                } else if constexpr (is_weak_ordering_) {
                    if (ord == std::weak_ordering::less)
                        out = __format::integral_formatter_v_<int>.format_to(out, -1, std::move(specs));
                    else if (ord == std::weak_ordering::equivalent)
                        out = __format::integral_formatter_v_<int>.format_to(out, 0, std::move(specs));
                    else if (ord == std::weak_ordering::greater)
                        out = __format::integral_formatter_v_<int>.format_to(out, 1, std::move(specs));
                } else if constexpr (is_partial_ordering_) {
                    if (ord == std::partial_ordering::less)
                        out = __format::integral_formatter_v_<int>.format_to(out, -1, std::move(specs));
                    else if (ord == std::partial_ordering::equivalent)
                        out = __format::integral_formatter_v_<int>.format_to(out, 0, std::move(specs));
                    else if (ord == std::partial_ordering::greater)
                        out = __format::integral_formatter_v_<int>.format_to(out, 1, std::move(specs));
                    else if (ord == std::partial_ordering::unordered)
                        out = __format::integral_formatter_v_<int>.format_to(out, 2, std::move(specs));
                }
                break;

              case format_specs::type_t::string:
              case format_specs::type_t::default_opt:
              default:
                if constexpr (is_strong_ordering_) {
                    if (specs.show_base)
                        out = __format::plain_cstr_formatter_v_.format_to(out, "strong_ordering::");

                    if (ord == std::strong_ordering::less) {
                        out = __format::plain_cstr_formatter_v_.format_to(out, "less");
                    } else if (ord == std::strong_ordering::equal) {
                        // 'equal' and 'equivalent' is the same, but 'equal' is much more preferred
                        out = __format::plain_cstr_formatter_v_.format_to(out, "equal");
                    } else if (ord == std::strong_ordering::greater) {
                        out = __format::plain_cstr_formatter_v_.format_to(out, "greater");
                    }

                } else if constexpr (is_weak_ordering_) {
                    if (specs.show_base)
                        out = __format::plain_cstr_formatter_v_.format_to(out, "weak_ordering::");

                    if (ord == std::weak_ordering::less) {
                        out = __format::plain_cstr_formatter_v_.format_to(out, "less");
                    } else if (ord == std::weak_ordering::equivalent) {
                        out = __format::plain_cstr_formatter_v_.format_to(out, "equivalent");
                    } else if (ord == std::weak_ordering::greater) {
                        out = __format::plain_cstr_formatter_v_.format_to(out, "greater");
                    }

                } else if constexpr (is_partial_ordering_) {
                    if (specs.show_base)
                        out = __format::plain_cstr_formatter_v_.format_to(out, "partial_ordering::");

                    if (ord == std::partial_ordering::less) {
                        out = __format::plain_cstr_formatter_v_.format_to(out, "less");
                    } else if (ord == std::partial_ordering::equivalent) {
                        out = __format::plain_cstr_formatter_v_.format_to(out, "equivalent");
                    } else if (ord == std::partial_ordering::greater) {
                        out = __format::plain_cstr_formatter_v_.format_to(out, "greater");
                    } else if (ord == std::partial_ordering::unordered) {
                        out = __format::plain_cstr_formatter_v_.format_to(out, "unordered");
                    }
                }
            }

            return out;
        }
    };

    /// formattable_to
    template <typename T, typename Out>
    concept formattable_to = gold::__format::char_output_iterator<Out> && requires (gold::formatter<T> fmt, Out out, T t, format_specs specs) {
        requires gold::formatter<T>::is_specialized;
        { fmt.format_to(std::move(out), t, specs) } -> gold::__format::char_output_iterator;
    };

    /// formattable
    template <typename T>
    concept formattable = formattable_to<T, std::back_insert_iterator<std::string>>;

    /// custom_formattable_to
    template <typename T, typename Out>
    concept custom_formattable_to = formattable_to<T, Out> && requires {
        requires gold::formatter<T>::is_customized;
    };

    /// custom_formattable
    template <typename T>
    concept custom_formattable = custom_formattable_to<T, std::back_insert_iterator<std::string>>;

} // namespace gold

#pragma GCC diagnostic pop
#endif // __GOLD_BITS_FORMAT_FORMATTER_HPP
