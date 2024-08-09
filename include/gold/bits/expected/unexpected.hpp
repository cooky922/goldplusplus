// <gold/bits/expected/unexpected.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_EXPECTED_UNEXPECTED_HPP
#define __GOLD_BITS_EXPECTED_UNEXPECTED_HPP

#include <concepts>
#include <initializer_list>
#include <gold/bits/__util/like_t.hpp>
#include <gold/bits/expected/utils.hpp>

namespace gold {

    /// unexpected
    template <__expected::can_be_unexpected E>
    class unexpected {
      private:
        E m_value_;

      public:
        constexpr unexpected() = default;
        constexpr unexpected(const unexpected&) = default;
        constexpr unexpected(unexpected&&) = default;

        template <typename... Args>
            requires std::constructible_from<E, Args...>
        explicit constexpr unexpected(std::in_place_t, Args&&... args)
        : m_value_(std::forward<Args>(args)...) {}

        template <typename U, typename... Args>
            requires std::constructible_from<std::initializer_list<U>&, Args...>
        explicit constexpr unexpected(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        : m_value_(il, std::forward<Args>(args)...) {}

        template <typename Err = E>
            requires std::constructible_from<E, Err>
        constexpr explicit unexpected(Err&& e)
        : m_value_(std::forward<Err>(e)) {}

        constexpr unexpected& operator=(const unexpected&) = default;
        constexpr unexpected& operator=(unexpected&&) = default;

        constexpr ~unexpected() = default;

        template <typename Self>
        [[nodiscard]] constexpr decltype(auto) error(this Self&& self) noexcept {
            return std::forward_like<Self>(self.m_value_);
        }

        constexpr void swap(unexpected& other) noexcept(std::is_nothrow_swappable_v<E>) {
            std::ranges::swap(m_value_, other.m_value_);
        }

        friend constexpr void swap(unexpected& lhs, unexpected& rhs) noexcept(noexcept(lhs.swap(rhs))) {
            lhs.swap(rhs);
        }

        template <typename G>
        friend constexpr bool operator==(const unexpected& lhs, const unexpected<G>& rhs) {
            return lhs.m_value_ == rhs.m_value_;
        }
    };

    template <typename E>
    unexpected(E) -> unexpected<E>;

    namespace __expected {

        /// __expected::is_unexpected_v<Unexpected>
        template <typename E>
        inline constexpr bool is_unexpected_v<unexpected<E>> = true;

    } // namespace __expected

} // namespace gold

#endif // __GOLD_BITS_EXPECTED_UNEXPECTED_HPP
