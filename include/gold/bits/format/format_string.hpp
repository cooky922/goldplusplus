// <gold/bits/format/format_string.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FORMAT_FORMAT_STRING_HPP
#define __GOLD_BITS_FORMAT_FORMAT_STRING_HPP

#include <gold/bits/format/fwd.hpp>
#include <gold/bits/format/scanner.hpp>

namespace gold {

    /// unchecked_format_string
    struct unchecked_format_string {
      private:
        std::string_view m_str_;

      public:
        template <typename T>
            requires std::convertible_to<const T&, std::string_view>
        constexpr unchecked_format_string(const T& op)
        : m_str_(op) {}

        constexpr std::string_view get() const noexcept { return m_str_; }
    };

    namespace __format {

        /// __format::fmt_string
        template <typename... Args>
        class fmt_string {
          private:
            std::string_view m_str_;

          public:
            template <typename T>
                requires std::convertible_to<const T&, std::string_view>
            consteval fmt_string(const T& op)
            : m_str_(op) {
                __format::checking_scanner<__remove_cvref(Args)...> fmt_scanner (m_str_);
                fmt_scanner.scan();
            }

            constexpr fmt_string(gold::unchecked_format_string op) noexcept
            : m_str_(op.get()) {}

            constexpr std::string_view get() const noexcept { return m_str_; }
        };

    } // namespace __format

} // namespace gold

#endif // __GOLD_BITS_FORMAT_FORMAT_STRING_HPP
