// <gold/bits/format/std_formatter.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_STD_FORMATTER_HPP
#define __GOLD_BITS_FORMAT_STD_FORMATTER_HPP

#include <gold/bits/concepts/class_or_union_or_enum.hpp>
#include <gold/bits/type_traits/conditional.hpp>
#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/concepts.hpp>
#include <gold/bits/format/parser.hpp>
#include <gold/bits/format/formatter.hpp>

namespace gold {

    /// formatter<char>
    template <>
    struct formatter<char> {
      private:
        __format::formatter_int m_f_;

      public:
        formatter() = default;

        constexpr __format::result<typename format_parse_context::iterator>
        try_parse(format_parse_context& pc) noexcept {
            return m_f_.template mf_try_parse_<char>(pc);
        }

        constexpr __format::result<typename format_context::iterator>
        try_format(char c, format_context& fc) const noexcept {
            if (m_f_.m_spec_.pres == __format::__specs::pres_kind::_default)
                return m_f_.mf_try_format_character_(c, fc);
            else if (m_f_.m_spec_.pres == __format::__specs::pres_kind::_escape)
                return m_f_.mf_try_format_character_escaped_(c, fc);
            else
                return m_f_.try_format(c, fc);
        }

        constexpr void set_debug_format() noexcept {
            m_f_.m_spec_.pres = __format::__specs::pres_kind::_escape;
        }
    };

    /// formatter<char*>
    template <>
    struct formatter<char*> {
      private:
        __format::formatter_str m_f_;

      public:
        formatter() = default;

        [[gnu::always_inline]] constexpr auto try_parse(format_parse_context& pc) noexcept {
            return m_f_.try_parse(pc);
        }

        [[gnu::nonnull]] constexpr auto try_format(char* s, format_context& fc) const noexcept {
            return m_f_.try_format(s, fc);
        }

        constexpr void set_debug_format() noexcept { m_f_.set_debug_format(); }
    };

    /// formatter<const char*>
    template <>
    struct formatter<const char*> {
      private:
        __format::formatter_str m_f_;

      public:
        formatter() = default;

        [[gnu::always_inline]] constexpr auto try_parse(format_parse_context& pc) noexcept {
            return m_f_.try_parse(pc);
        }

        [[gnu::nonnull]] constexpr auto try_format(const char* s, format_context& fc) const noexcept {
            return m_f_.try_format(s, fc);
        }

        constexpr void set_debug_format() noexcept { m_f_.set_debug_format(); }
    };

    /// formatter<char[N]>
    template <std::size_t N>
    struct formatter<char[N]> {
      private:
        __format::formatter_str m_f_;

      public:
        formatter() = default;

        [[gnu::always_inline]] constexpr auto try_parse(format_parse_context& pc) noexcept {
            return m_f_.try_parse(pc);
        }

        constexpr auto try_format(const char (& s)[N], format_context& fc) const noexcept {
            return m_f_.try_format({s, N}, fc);
        }

        constexpr void set_debug_format() noexcept { m_f_.set_debug_format(); }
    };

    /// formatter<std::basic_string>
    template <typename Traits, typename Alloc>
    struct formatter<std::basic_string<char, Traits, Alloc>> {
      private:
        __format::formatter_str m_f_;

      public:
        formatter() = default;

        [[gnu::always_inline]] constexpr auto try_parse(format_parse_context& pc) noexcept {
            return m_f_.try_parse(pc);
        }

        constexpr auto try_format(const std::basic_string<char, Traits, Alloc>& s, format_context& fc) const noexcept {
            return m_f_.try_format(s, fc);
        }

        constexpr void set_debug_format() noexcept { m_f_.set_debug_format(); }
    };

    /// formatter<std::basic_string_view>
    template <typename Traits>
    struct formatter<std::basic_string_view<char, Traits>> {
      private:
        __format::formatter_str m_f_;

      public:
        formatter() = default;

        [[gnu::always_inline]] constexpr auto try_parse(format_parse_context& pc) noexcept {
            return m_f_.try_parse(pc);
        }

        constexpr auto try_format(std::basic_string_view<char, Traits> s, format_context& fc) const noexcept {
            return m_f_.try_format(s, fc);
        }

        constexpr void set_debug_format() noexcept { m_f_.set_debug_format(); }
    };

    /// formatter<Int>
    template <__format::int_only T>
    struct formatter<T> {
      private:
        __format::formatter_int m_f_;

      public:
        formatter() = default;

        constexpr auto try_parse(format_parse_context& pc) noexcept {
            return m_f_.template mf_try_parse_<T>(pc);
        }

        constexpr auto try_format(T i, format_context& fc) const noexcept {
            return m_f_.try_format(i, fc);
        }

    };

    /// formatter<bool>
    template <>
    struct formatter<bool> {
      private:
        __format::formatter_int m_f_;

      public:
        formatter() = default;

        constexpr auto try_parse(format_parse_context& pc) noexcept {
            return m_f_.template mf_try_parse_<bool>(pc);
        }

        constexpr auto try_format(bool b, format_context& fc) const noexcept {
            return m_f_.try_format(b, fc);
        }
    };

    /// formatter<FloatingPoint>
    template <__format::formattable_float T>
    struct formatter<T> {
      private:
        __format::formatter_fp m_f_;

      public:
        formatter() = default;

        constexpr auto try_parse(format_parse_context& pc) noexcept {
            return m_f_.try_parse(pc);
        }

        constexpr auto try_format(T i, format_context& fc) const noexcept {
            return m_f_.try_format(i, fc);
        }
    };

    /// formatter<void*>
    template <>
    struct formatter<void*> {
      private:
        __format::formatter_ptr m_f_;

      public:
        formatter() = default;

        constexpr auto try_parse(format_parse_context& pc) noexcept {
            return m_f_.try_parse(pc);
        }

        constexpr auto try_format(void* p, format_context& fc) const noexcept {
            return m_f_.try_format(p, fc);
        }
    };

    /// formatter<const void*>
    template <>
    struct formatter<const void*> {
      private:
        __format::formatter_ptr m_f_;

      public:
        formatter() = default;

        constexpr auto try_parse(format_parse_context& pc) noexcept {
            return m_f_.try_parse(pc);
        }

        constexpr auto try_format(const void* p, format_context& fc) const noexcept {
            return m_f_.try_format(p, fc);
        }
    };

    /// formatter<std::nullptr_t>
    template <>
    struct formatter<std::nullptr_t> {
      private:
        __format::formatter_ptr m_f_;

      public:
        formatter() = default;

        constexpr auto try_parse(format_parse_context& pc) noexcept {
            return m_f_.try_parse(pc);
        }

        constexpr auto try_format(std::nullptr_t, format_context& fc) const noexcept {
            return m_f_.try_format(nullptr, fc);
        }
    };

    /// formatter<OrderingType>
    template <gold::ordering_type T>
    struct formatter<T> {
      private:
        __format::formatter_ord m_f_;

      public:
        formatter() = default;

        constexpr auto try_parse(format_parse_context& pc) noexcept {
            return m_f_.try_parse(pc);
        }

        constexpr auto try_format(T op, format_context& fc) const noexcept {
            return m_f_.try_format(op, fc);
        }
    };

    namespace __format {

        // NOTE:
        // if you wish to overload 'format_as'
        // please define it in the same scope as
        // your user-defined type

        // ex:
        //
        // this is ok:
        // =========================
        // struct A {};
        // int format_as(A) { return 0; }
        //
        //
        // this is error:
        // =========================
        // struct A {};
        // namespace gold { int format_as(A) { return 0; } }
        //

        /// __format::uses_adl_format_as
        template <typename T>
        concept uses_adl_format_as =
            __concepts::class_or_union_or_enum<T> &&
            requires (const T& op) {
                 { format_as(op) } -> different_from<T>;
                 { format_as(op) } -> formattable;
            };

        /// __format::adl_format_as_result_t
        template <typename T>
        using adl_format_as_result_t = decltype(format_as(std::declval<const T&>()));

    } // namespace __format

    // TODO:
    // do not specialize another formatter
    // but only redefine formattable to
    // accept 'format_as' alone
    template <__format::uses_adl_format_as T>
    struct formatter<T> : formatter<__format::adl_format_as_result_t<T>> {
      private:
        using Base = formatter<__format::adl_format_as_result_t<T>>;
        using Arg  = gold::conditional_t<
            std::is_enum_v<T> || std::is_trivially_copyable_v<T>,
            T, const T&
        >;

      public:
        static constexpr bool uses_format_as = true;

        constexpr auto try_format(Arg arg, format_context& fc) const {
            return Base::try_format(format_as(arg), fc);
        }
    };

} // namespace gold

// define it for std::byte
namespace std { constexpr int format_as(std::byte b) noexcept { return static_cast<int>(b); } }

#endif // __GOLD_BITS_FORMAT_STD_FORMATTER_HPP
