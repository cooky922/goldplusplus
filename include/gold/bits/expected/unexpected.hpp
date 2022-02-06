// <gold/bits/expected/unexpected.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_EXPECTED_UNEXPECTED_HPP
#define __GOLD_BITS_EXPECTED_UNEXPECTED_HPP

#include <initializer_list>
#include <concepts>

namespace gold {

    /// unexpected
    template <typename E>
    class unexpected {
      private:
        E m_value_;

      public:
        constexpr unexpected() = default;
        constexpr unexpected(const unexpected&) = default;
        constexpr unexpected(unexpected&&) = default;

        template <typename... Args>
            requires std::constructible_from<E, Args...>
        constexpr explicit unexpected(std::in_place_t, Args&&... args)
        : m_value_(std::forward<Args>(args)...) {}

        template <typename U, typename... Args>
            requires std::constructible_from<std::initializer_list<U>, Args...>
        constexpr explicit unexpected(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        : m_value_(il, std::forward<Args>(args)...) {}

        template <typename Err = E>
            requires std::constructible_from<E, Err>
        constexpr explicit unexpected(Err&& e)
        : m_value_(std::forward<Err>(e)) {}

        constexpr unexpected& operator=(const unexpected&) = default;
        constexpr unexpected& operator=(unexpected&&) = default;

        constexpr E& value() & noexcept { return m_value_; }
        constexpr E&& value() && noexcept { return std::move(m_value_); }
        constexpr const E& value() const& noexcept { return m_value_; }
        constexpr const E&& value() const&& noexcept { return std::move(m_value_); }

        constexpr void swap(unexpected& other) noexcept {
            using std::swap;
            swap(m_value_, other.m_value_);
        }

        constexpr bool operator==(const unexpected&) const noexcept = default;
        constexpr auto operator<=>(const unexpected&) const noexcept = default;
    };

    template <typename E>
    unexpected(E) -> unexpected<E>;

    /// unexpect_t
    struct unexpect_t {
        constexpr explicit unexpect_t() = default;
    };

    /// unexpect
    inline constexpr unexpect_t unexpect {};

} // namespace gold

#endif // __GOLD_BITS_EXPECTED_UNEXPECTED_HPP
