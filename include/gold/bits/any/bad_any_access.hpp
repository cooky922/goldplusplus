// <gold/bits/any/bad_any_access.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_BAD_ANY_ACCESS_HPP
#define __GOLD_BITS_BAD_ANY_ACCESS_HPP

#include <stdexcept>

namespace gold {

    /// bad_any_access_kind
    enum class bad_any_access_kind {
        bad_cast,
        no_equality_operator,
        no_spaceship_operator,
        unknown
    };

    namespace __any {
        constexpr const char* error_kind_to_str(bad_any_access_kind kind) noexcept {
            switch (kind) {
                using enum bad_any_access_kind;
                case bad_cast:
                    return "bad any cast";
                case no_equality_operator:
                    return "no equality operator";
                case no_spaceship_operator:
                    return "no spaceship operator";
                default:
                    return "bad any access";
            }
        }
    } // namespace __any

    /// bad_any_access
    class bad_any_access : public std::runtime_error {
      private:
        bad_any_access_kind m_error_kind_ = bad_any_access_kind::unknown;

      public:
        explicit bad_any_access(bad_any_access_kind error_kind)
        : std::runtime_error(__any::error_kind_to_str(error_kind)),
          m_error_kind_(error_kind) {}

        explicit bad_any_access(const char* s)
        : std::runtime_error(s) {}

        explicit bad_any_access(const std::string& s)
        : std::runtime_error(s) {}

        virtual ~bad_any_access() noexcept = default;

        bad_any_access_kind error_kind() const noexcept { return m_error_kind_; }
    };

} // namespace gold

#endif // __GOLD_BITS_BAD_ANY_ACCESS_HPP
