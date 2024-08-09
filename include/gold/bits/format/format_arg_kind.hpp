// <gold/bits/format/format_arg_kind.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_FORMAT_ARG_KIND_HPP
#define __GOLD_BITS_FORMAT_FORMAT_ARG_KIND_HPP

#include <concepts>
#include <bit>
#include <string_view>
#if __has_include(<bits/compare.h>)
#include <bits/compare.h>
#else
#include <compare>
#endif
#include <gold/basic_types>
#include <gold/bits/__util/uneval.hpp>
#include <gold/bits/__util/mini_span.hpp>

namespace gold::__format {

    /// __format::is_char_array
    template <typename T>
    inline constexpr bool is_char_array =
        std::is_bounded_array_v<T> &&
        std::is_same_v<char, __remove_cv(std::remove_extent_t<T>)>;

    /// __format::arg_kind
    enum class arg_kind : gold::uint8_t {
        none_type = 0,          // 0
        bool_type,              // 1
        char_type,              // 2
        i32_type,               // 3
        i64_type,               // 4
        i128_type,              // 5
        u32_type,               // 6
        u64_type,               // 7
        u128_type,              // 8
        f16_type,               // 9
        bf16_type,              // 10
        f32_type,               // 11
        f64_type,               // 12
        f80_type,               // 13
        f128_type,              // 14
        cstring_view_type,      // 15
        string_view_type,       // 16
        ptr_type,               // 17
        strong_ord_type,        // 18
        weak_ord_type,          // 19
        partial_ord_type,       // 20
        custom_type             // 21
    };

    /// __format::normalize_type_fn
    template <typename T, typename Custom>
    consteval auto normalize_type_fn() {
        using D = __remove_cvref(T);
        if constexpr (__is_same(bool, D))
            return __util::uneval<bool>;
        else if constexpr (__is_same(char, D))
            return __util::uneval<char>;
        else if constexpr (__is_same(wchar_t, D)  ||
                           __is_same(char8_t, D)  ||
                           __is_same(char16_t, D) ||
                           __is_same(char32_t, D))
            static_assert(false, "unsupported wide or unicode character type");
        else if constexpr (std::signed_integral<D>) {
            if constexpr (sizeof(D) <= sizeof(gold::int32_t)) {
                return __util::uneval<gold::int32_t>;
            } else if constexpr (sizeof(D) <= sizeof(gold::int64_t)) {
                return __util::uneval<gold::int64_t>;
            } else if constexpr (sizeof(D) <= sizeof(gold::int128_t)) {
                return __util::uneval<gold::int128_t>;
            } else {
                static_assert(false, "unsupported signed integer");
            }
        } else if constexpr (std::unsigned_integral<D>) {
            if constexpr (sizeof(D) <= sizeof(gold::uint32_t)) {
                return __util::uneval<gold::uint32_t>;
            } else if constexpr (sizeof(D) <= sizeof(gold::uint64_t)) {
                return __util::uneval<gold::uint64_t>;
            } else if constexpr (sizeof(D) <= sizeof(gold::uint128_t)) {
                return __util::uneval<gold::uint128_t>;
            } else {
                static_assert(false, "unsupported unsigned integer");
            }
        } else if constexpr (std::floating_point<D>) {
            if constexpr (__is_same(gold::float16x_t, D)) {
                // gold::float16x_t == std::float16_t
                return __util::uneval<gold::float16x_t>;
            } else if constexpr (__is_same(gold::bfloat16x_t, D)) {
                // gold::bfloat16x_t == std::bfloat16_t
                return __util::uneval<gold::bfloat16x_t>;
            } else if constexpr (__is_same(gold::float32_t, D) ||
                                 __is_same(gold::float32x_t, D)) {
                // gold::float32_t == float
                return __util::uneval<gold::float32_t>;
            } else if constexpr (__is_same(gold::float64_t, D) ||
                                 __is_same(gold::float64x_t, D)) {
                // gold::float64_t == double
                return __util::uneval<gold::float64_t>;
            } else if constexpr (__is_same(gold::float80_t, D)) {
                // gold::float80_t == long double
                return __util::uneval<gold::float80_t>;
            } else if constexpr (__is_same(gold::float128_t, D) ||
                                 __is_same(gold::float128x_t, D)) {
                // gold::float128x_t == std::float128_t
                return __util::uneval<gold::float128x_t>;
            } else {
                static_assert(false, "unsupported floating point");
            }
        } else if constexpr (__is_same(char*, D)
                          || __is_same(const char*, D)) {
            return __util::uneval<const char*>;
        } else if constexpr (__is_same(void*, D)
                          || __is_same(const void*, D)) {
            return __util::uneval<const void*>;
        } else if constexpr (__format::is_char_array<D>
                          || __is_convertible(D, std::string_view)) {
            return __util::uneval<std::string_view>;
        } else if constexpr (__is_same(std::strong_ordering, D)) {
            return __util::uneval<std::strong_ordering>;
        } else if constexpr (__is_same(std::weak_ordering, D)) {
            return __util::uneval<std::weak_ordering>;
        } else if constexpr (__is_same(std::partial_ordering, D)) {
            return __util::uneval<std::partial_ordering>;
        } else {
            return __util::uneval<Custom>;
        }
    }

    /// __format::normalize_type
    template <typename T, typename Custom = __remove_cvref(T)>
    using normalize_type = __remove_reference(decltype(normalize_type_fn<T, Custom>()()));

    /// __format::type_to_enum
    template <typename T, typename Custom = __remove_cvref(T)>
    static consteval __format::arg_kind type_to_enum() {
        if constexpr (__is_same(bool, T))
            return __format::arg_kind::bool_type;
        else if constexpr (__is_same(char, T))
            return __format::arg_kind::char_type;
        else if constexpr (__is_same(gold::int32_t, T))
            return __format::arg_kind::i32_type;
        else if constexpr (__is_same(gold::int64_t, T))
            return __format::arg_kind::i64_type;
        else if constexpr (__is_same(gold::int128_t, T))
            return __format::arg_kind::i128_type;
        else if constexpr (__is_same(gold::uint32_t, T))
            return __format::arg_kind::u32_type;
        else if constexpr (__is_same(gold::uint64_t, T))
            return __format::arg_kind::u64_type;
        else if constexpr (__is_same(gold::uint128_t, T))
            return __format::arg_kind::u128_type;
        else if constexpr (__is_same(gold::float16x_t, T))
            return __format::arg_kind::f16_type;
        else if constexpr (__is_same(gold::bfloat16x_t, T))
            return __format::arg_kind::bf16_type;
        else if constexpr (__is_same(gold::float32_t, T))
            return __format::arg_kind::f32_type;
        else if constexpr (__is_same(gold::float64_t, T))
            return __format::arg_kind::f64_type;
        else if constexpr (__is_same(gold::float80_t, T))
            return __format::arg_kind::f80_type;
        else if constexpr (__is_same(gold::float128x_t, T))
            return __format::arg_kind::f128_type;
        else if constexpr (__is_same(const char*, T))
            return __format::arg_kind::cstring_view_type;
        else if constexpr (__is_same(std::string_view, T))
            return __format::arg_kind::string_view_type;
        else if constexpr (__is_same(const void*, T))
            return __format::arg_kind::ptr_type;
        else if constexpr (__is_same(std::strong_ordering, T))
            return __format::arg_kind::strong_ord_type;
        else if constexpr (__is_same(std::weak_ordering, T))
            return __format::arg_kind::weak_ord_type;
        else if constexpr (__is_same(std::partial_ordering, T))
            return __format::arg_kind::partial_ord_type;
        else if constexpr (__is_same(Custom, T))
            return __format::arg_kind::custom_type;
        else
            return __format::arg_kind::none_type;
    }

    /// __format::arg_kinds_packed_type
    template <std::size_t N>
    using arg_kinds_packed_type = __type_pack_element<
        0  <= N && N <= 3  ? 0 :
        4  <= N && N <= 6  ? 1 :
        7  <= N && N <= 12 ? 2 :
        13 <= N && N <= 25 ? 3 : 4,
        gold::uint16_t,
        gold::uint32_t,
        gold::uint64_t,
        gold::uint128_t,
        __util::mini_span<const __format::arg_kind>
    >;

    /// __format::arg_kinds
    template <typename PackedType = __util::mini_span<const __format::arg_kind>>
    struct arg_kinds {
        PackedType packed_values;

        constexpr std::size_t size() const noexcept {
            // only '__util::mini_span<const __format::arg_kind>' can do this
            if constexpr (requires { packed_values.size(); })
                return packed_values.size();
            else {
                auto approx_bit_width = std::bit_width(packed_values);
                return approx_bit_width / 5 + PackedType(bool(approx_bit_width % 5));
            }
        }

        constexpr __format::arg_kind operator[](std::size_t pos) const noexcept {
            // only '__util::mini_span<const __format::arg_kind>' can do this
            if constexpr (requires { packed_values[pos]; })
                return packed_values[pos];
            else {
                return __format::arg_kind((packed_values >> ((size() - pos - 1) * 5)) & 0b11111u);
            }
        }
    };

    /// __format::make_arg_kind
    template <typename T>
    consteval auto make_arg_kind() {
        return __format::type_to_enum<__format::normalize_type<T>>();
    }

    /// __format::make_arg_kinds
    template <typename... Ts>
    consteval auto make_arg_kinds() {
        if constexpr (sizeof...(Ts) != 0) {
            static constexpr __format::arg_kind kinds[] { __format::type_to_enum<__format::normalize_type<Ts>>() ... };
            return __format::arg_kinds <> { __util::mini_span<const __format::arg_kind> { kinds, sizeof...(Ts) } };
        } else {
            static constexpr __format::arg_kind none_kind = __format::arg_kind::none_type;
            return __format::arg_kinds <> { __util::mini_span<const __format::arg_kind> { &none_kind, 0uz } };
        }
    }

    /// __format::make_packed_arg_kinds
    template <typename... Ts>
    consteval auto make_packed_arg_kinds() {
        if constexpr (sizeof...(Ts) != 0) {
            static constexpr __format::arg_kind kinds[] { __format::type_to_enum<__format::normalize_type<Ts>>() ... };
            using packed_type = __format::arg_kinds_packed_type<sizeof...(Ts)>;
            if constexpr (sizeof...(Ts) > 25)
                return __format::arg_kinds { packed_type { kinds, sizeof...(Ts) } };
            else {
                packed_type packed_values = 0;
                for (__format::arg_kind kind : kinds) {
                    packed_values <<= 5;
                    packed_values  |= static_cast<gold::uint8_t>(kind);
                }
                return __format::arg_kinds { packed_values };
            }
        } else {
            static constexpr __format::arg_kind none_kind = __format::arg_kind::none_type;
            return __format::arg_kinds <> { __util::mini_span<const __format::arg_kind> { &none_kind, 0uz } };
        }
    }

} // namespace gold::__format

#endif // __GOLD_BITS_FORMAT_FORMAT_ARG_KIND_HPP
