// <gold/bits/expected/bad_access.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_EXPECTED_BAD_ACCESS_HPP
#define __GOLD_BITS_EXPECTED_BAD_ACCESS_HPP

#include <exception>
#include <gold/type_traits>

namespace gold {

    /// expected [fwd decl]
    template <typename, typename>
    class expected;

    /// bad_unexpected_access
    class bad_unexpected_access : public std::exception {
      public:
        const char* what() const noexcept override { return "bad unexpected access"; }
    };

    /// bad_expected_access [fwd decl]
    template <typename>
    class bad_expected_access;

    /// bad_expected_access<void>
    template <>
    class bad_expected_access<void> : public std::exception {
      protected:
        bad_expected_access() noexcept = default;
        bad_expected_access(const bad_expected_access&) = default;
        bad_expected_access(bad_expected_access&&) = default;
        bad_expected_access& operator=(const bad_expected_access&) = default;
        bad_expected_access& operator=(bad_expected_access&&) = default;

      public:
        const char* what() const noexcept override { return "bad expected access"; }
    };

    /// bad_expected_access<T>
    template <typename E>
        requires (!std::is_void_v<E>)
    class bad_expected_access<E> : public bad_expected_access<void> {
      private:
        E m_value_;

      public:
        explicit bad_expected_access(E e)
        : m_value_(std::move(e)) {}

        E& error() & noexcept { return m_value_; }
        E&& error() && noexcept { return std::move(m_value_); }
        const E& error() const& noexcept { return m_value_; }
        const E&& error() const&& noexcept { return std::move(m_value_); }
    };

} // namespace gold

#endif // __GOLD_BITS_EXPECTED_BAD_ACCESS_HPP
