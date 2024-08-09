// <gold/bits/expected/bad_access.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_EXPECTED_BAD_ACCESS_HPP
#define __GOLD_BITS_EXPECTED_BAD_ACCESS_HPP

#include <bits/exception.h>
#include <bits/move.h>

namespace gold {

    /// bad_unexpected_access
    class bad_unexpected_access : public std::exception {
      public:
        const char* what() const noexcept override { return "bad unexpected access"; }
    };

    /// bad_expected_access
    template <typename>
    class bad_expected_access;

    /// bad_expected_access<void>
    template <>
    class bad_expected_access<void> : public std::exception {
      protected:
        bad_expected_access() noexcept = default;
        bad_expected_access(const bad_expected_access&) = default;
        bad_expected_access(bad_expected_access&&) noexcept = default;
        bad_expected_access& operator=(const bad_expected_access&) = default;
        bad_expected_access& operator=(bad_expected_access&&) noexcept = default;

      public:
        const char* what() const noexcept override { return "bad expected access"; }
    };

    /// bad_expected_access - main definition
    template <typename E>
    class bad_expected_access : public bad_expected_access<void> {
      private:
        E m_value_;

      public:
        explicit bad_expected_access(E e) : m_value_(std::move(e)) {}

        [[nodiscard]] E& error() & noexcept { return m_value_; }
        [[nodiscard]] const E& error() const& noexcept { return m_value_; }
        [[nodiscard]] E&& error() && noexcept { return std::move(m_value_); }
        [[nodiscard]] const E&& error() const&& noexcept { return std::move(m_value_); }
    };

    namespace __expected {

        /// __expected::bad_unexpected_access_exception
        [[noreturn]] inline void bad_unexpected_access_exception() {}

        /// __expected::throw_bad_unexpected_access
        [[noreturn]] constexpr void throw_bad_unexpected_access(bool b = true) {
            if consteval { if (b) bad_unexpected_access_exception(); }
            else { throw bad_unexpected_access(); }
        }

        /// __expected::bad_expected_access_exception
        inline void bad_expected_access_exception() {}

        /// __expected::throw_bad_expected_access
        template <typename E>
        [[noreturn]] constexpr void throw_bad_expected_access(E e, bool b = true) {
            if consteval { if (b) bad_expected_access_exception(); }
            else { throw bad_expected_access<E>(std::move(e)); }
        }

    } // namespace __expected

} // namespace gold

#endif // __GOLD_BITS_EXPECTED_BAD_ACCESS_HPP
