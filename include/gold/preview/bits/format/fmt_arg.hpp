// <gold/preview/bits/format/fmt_arg.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: this is only a prototype
// note: this is an internal header

#pragma once
#ifndef __GOLD_PREVIEW_BITS_FMT_ARG_HPP
#define __GOLD_PREVIEW_BITS_FMT_ARG_HPP

#include <functional>
#include <variant>
#include <gold/bits/concepts/types.hpp>
#include <gold/bits/concepts/string.hpp>
#include <gold/assertion>
#include <gold/preview/bits/format/fmt_error.hpp>
#include <gold/preview/bits/format/fmt_fwd.hpp>

namespace gold::preview {

    namespace __format::__detail {

        /// arg_t_
        enum class arg_t_ : std::uint8_t {
            none_,
            boolean_,
            char_type_,
            int_,
            long_long_,
            unsigned_,
            unsigned_long_long_,
            float_,
            double_,
            long_double_,
            const_char_type_ptr_,
            string_view_,
            ptr_,
            custom_
        };

        /// is_proper_string_
        template <typename T>
        inline constexpr bool is_proper_string_ = false;

        template <>
        inline constexpr bool is_proper_string_<const char*> = true;

        template <>
        inline constexpr bool is_proper_string_<const wchar_t*> = true;

        template <>
        inline constexpr bool is_proper_string_<char*> = true;

        template <>
        inline constexpr bool is_proper_string_<wchar_t*> = true;

        template <std::size_t N>
        inline constexpr bool is_proper_string_<const char[N]> = true;

        template <std::size_t N>
        inline constexpr bool is_proper_string_<const wchar_t[N]> = true;

        template <std::size_t N>
        inline constexpr bool is_proper_string_<char[N]> = true;

        template <std::size_t N>
        inline constexpr bool is_proper_string_<wchar_t[N]> = true;

        template <typename CharT, typename Traits>
        inline constexpr bool is_proper_string_<std::basic_string_view<CharT, Traits>> = true;

        template <typename CharT, typename Traits, typename Alloc>
        inline constexpr bool is_proper_string_<std::basic_string<CharT, Traits, Alloc>> = true;

        /// supported_builtin_types_
        template <typename T>
        concept supported_builtin_types_ = (
            std::same_as<T, bool> ||
            gold::char_type<T>    || gold::qualified_numeric<T> ||
            is_proper_string_<std::remove_cvref_t<T>> ||
            std::is_pointer_v<std::remove_cvref_t<T>> ||
            std::is_null_pointer_v<std::remove_cvref<T>>
        );

    } // namespace __format::__detail

    /// visit_format_arg [ forward decl ]
    template <typename Visitor, typename Context>
    decltype(auto) visit_format_arg(Visitor&&, basic_format_arg<Context>);

    /// basic_format_arg
    template <typename Context>
    class basic_format_arg {
      public:
        class handle;

      private:
        friend handle;

        using context_type = Context;
        using char_type    = typename Context::char_type;

      public:
        class handle {
          private:
            friend basic_format_arg;

            const void* m_ptr_;
            void (*m_format_)(basic_format_parse_context<char_type>&, Context&, const void*);

          public:
            template <typename T>
            explicit handle(const T& v) noexcept
            : m_ptr_(std::addressof(v)),
              m_format_(
                 [](basic_format_parse_context<char_type>& parse_ctx, Context& format_ctx, const void* ptr) {
                    typename Context::template formatter_type<T> formatter_;
                    parse_ctx.advance_to(formatter_.parse(parse_ctx));
                    format_ctx.advance_to(formatter_.format(*static_cast<const T*>(ptr), format_ctx));
                 }
              ) {}

            void format(basic_format_parse_context<char_type>& parse_ctx, Context& format_ctx) const {
                m_format_(parse_ctx, format_ctx, m_ptr_);
            }
        };

      private:

        template <typename, typename...>
        friend struct __detail::format_arg_store_;

        template <typename Context_, typename... Args_>
        friend __detail::format_arg_store_<Context_, Args_...> __detail::make_basic_format_args_(const Args_&...);

        template <typename Visitor_, typename Context_>
        friend decltype(auto) visit_format_arg(Visitor_&&, basic_format_arg<Context_>);

        /// data member
        union {
            bool boolean_;
            char_type char_type_;
            int int_;
            long long long_long_;
            unsigned unsigned_;
            unsigned long long unsigned_long_long_;
            float float_;
            double double_;
            long double long_double_;
            const char_type* const_char_type_ptr_;
            std::basic_string_view<char_type> string_view_;
            const void* ptr_;
            handle custom_;
        };

        __format::__detail::arg_t_ m_type_;

      public:

        basic_format_arg() noexcept
        : m_type_(__format::__detail::arg_t_::none_) {}

        explicit operator bool() const noexcept {
            return m_type_ != __format::__detail::arg_t_::none_;
        }

      private:

        /// bool constructor
        explicit basic_format_arg(bool v) noexcept
        : boolean_(v), m_type_(__format::__detail::arg_t_::boolean_) {}

        /// char_type constructor
        explicit basic_format_arg(char_type v) noexcept
        : char_type_(v), m_type_(__format::__detail::arg_t_::char_type_) {}

        /// signed integral constructor
        template <std::signed_integral T>
        explicit basic_format_arg(T v) noexcept {
            if constexpr (sizeof(T) <= sizeof(int)) {
                int_ = static_cast<int>(v);
                m_type_ = __format::__detail::arg_t_::int_;
            } else if constexpr (sizeof(T) <= sizeof(long long)) {
                long_long_ = static_cast<long long>(v);
                m_type_ = __format::__detail::arg_t_::long_long_;
            } else {
                static_assert(sizeof(T) == 0, "An unsupported signed integer was used");
            }
        }

        /// unsigned integral constructor
        template <std::unsigned_integral T>
        explicit basic_format_arg(T v) noexcept {
            if constexpr (sizeof(T) <= sizeof(unsigned)) {
                unsigned_ = static_cast<unsigned>(v);
                m_type_ = __format::__detail::arg_t_::unsigned_;
            } else if constexpr (sizeof(T) <= sizeof(unsigned long long)) {
                unsigned_long_long_ = static_cast<unsigned long long>(v);
                m_type_ = __format::__detail::arg_t_::unsigned_long_long_;
            } else {
                static_assert(sizeof(T) == 0, "An unsupported unsigned integer was used");
            }
        }

        /// float constructor
        explicit basic_format_arg(float v) noexcept
        : float_(v), m_type_(__format::__detail::arg_t_::float_) {}

        /// double constructor
        explicit basic_format_arg(double v) noexcept
        : double_(v), m_type_(__format::__detail::arg_t_::double_) {}

        /// long double constructor
        explicit basic_format_arg(long double v) noexcept
        : long_double_(v), m_type_(__format::__detail::arg_t_::long_double_) {}

        /// const char_type* constructor
        explicit basic_format_arg(const char_type* s)
        : const_char_type_ptr_(s), m_type_(__format::__detail::arg_t_::const_char_type_ptr_) {
            gold::system_assert(s, "Used a nullptr argument to initialize a C-string");
        }

        /// string_view constructor
        template <typename Traits_>
        explicit basic_format_arg(std::basic_string_view<char_type, Traits_> s) noexcept
        : string_view_{s.data(), s.size()},
          m_type_(__format::__detail::arg_t_::string_view_) {}

        /// string constructor
        template <typename Traits_, typename Alloc_>
        explicit basic_format_arg(const std::basic_string<char_type, Traits_, Alloc_>& s) noexcept
        : string_view_{s.data(), s.size()},
          m_type_(__format::__detail::arg_t_::string_view_) {}

        /// null pointer constructor
        explicit basic_format_arg(std::nullptr_t) noexcept
        : ptr_(nullptr), m_type_(__format::__detail::arg_t_::ptr_) {}

        /// generic pointer constructor
        explicit basic_format_arg(const void* v) noexcept
        : ptr_(v), m_type_(__format::__detail::arg_t_::ptr_) {}

        /// custom constructor
        template <typename T>
            requires (!__format::__detail::supported_builtin_types_<T>)
        explicit basic_format_arg(T v) noexcept
        : custom_(v), m_type_(__format::__detail::arg_t_::custom_) {}

    };

    /// visit_format_arg
    template <typename Visitor, typename Context>
    decltype(auto) visit_format_arg(Visitor&& vis, basic_format_arg<Context> arg) {
        switch (arg.m_type_) {
            using enum __format::__detail::arg_t_;
            case none_ :
                return std::invoke(std::forward<Visitor>(vis), std::monostate{});
            case boolean_ :
                return std::invoke(std::forward<Visitor>(vis), arg.boolean_);
            case char_type_ :
                return std::invoke(std::forward<Visitor>(vis), arg.char_type_);
            case int_ :
                return std::invoke(std::forward<Visitor>(vis), arg.int_);
            case long_long_ :
                return std::invoke(std::forward<Visitor>(vis), arg.long_long_);
            case unsigned_ :
                return std::invoke(std::forward<Visitor>(vis), arg.unsigned_);
            case unsigned_long_long_ :
                return std::invoke(std::forward<Visitor>(vis), arg.unsigned_long_long_);
            case float_ :
                return std::invoke(std::forward<Visitor>(vis), arg.float_);
            case double_ :
                return std::invoke(std::forward<Visitor>(vis), arg.double_);
            case long_double_ :
                return std::invoke(std::forward<Visitor>(vis), arg.long_double_);
            case const_char_type_ptr_ :
                return std::invoke(std::forward<Visitor>(vis), arg.const_char_type_ptr_);
            case string_view_ :
                return std::invoke(std::forward<Visitor>(vis), arg.string_view_);
            case ptr_ :
                return std::invoke(std::forward<Visitor>(vis), arg.ptr_);
            case custom_ :
                return std::invoke(std::forward<Visitor>(vis), arg.custom_);
        }
        __builtin_unreachable();
    }

} // namespace gold::preview

#endif // __GOLD_PREVIEW_BITS_FMT_ARG_HPP
