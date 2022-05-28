// <gold/bits/any/cpo.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ANY_CPO_HPP
#define __GOLD_BITS_ANY_CPO_HPP

#include <gold/bits/any/fwd.hpp>
#include <gold/bits/any/bad_any_access.hpp>
#include <concepts>

namespace gold {

    inline namespace __cpo_access {

        /// __cpo_access::holds_current_type_fn [internal use]
        template <typename T>
        struct holds_current_type_fn {
            template <typename Any>
                requires requires (const Any& op) {
                    { holds_current_type<T>(op) } noexcept -> std::convertible_to<bool>;
                }
            constexpr bool operator()(const Any& op) const noexcept {
                return holds_current_type<T>(op);
            }
        };

        /// __cpo_access::any_cast_fn [internal use]
        template <typename T>
            requires (!std::is_reference_v<T>)
        struct any_cast_fn {

            template <typename Any>
            inline static constexpr bool s_has_already_ref_adl_ = requires (Any&& op) {
                { any_cast<T>(std::forward<Any>(op)) };
            };

            // for pointer overloads:
            template <typename Any>
                requires requires (Any* op) {
                    { any_cast<T>(op) } noexcept -> std::same_as<T*>;
                }
            constexpr T* operator()(Any* op) const noexcept {
                return any_cast<T>(op);
            }

            template <typename Any>
                requires requires (const Any* op) {
                    { any_cast<T>(op) } noexcept -> std::same_as<const T*>;
                }
            constexpr const T* operator()(const Any* op) const noexcept {
                return any_cast<T>(op);
            }

            // for reference overloads:
            template <typename Any>
                requires (!std::is_pointer_v<std::remove_cvref_t<Any>> && s_has_already_ref_adl_<Any>)
            constexpr decltype(auto) operator()(Any&& op) const {
                return any_cast<T>(std::forward<Any>(op));
            }

            template <typename Any>
                requires (!s_has_already_ref_adl_<Any>)
            constexpr auto& operator()(Any& op) const {
                auto* temp = (*this)(std::addressof(op));
                if (temp == nullptr) {
                    if consteval { __bad_any_access::bad_cast(); }
                    else { throw bad_any_access("bad any cast"); }
                }
                return *temp;
            }

            template <typename Any>
                requires (!s_has_already_ref_adl_<Any>)
            constexpr const auto& operator()(const Any& op) const {
                auto* temp = (*this)(std::addressof(as_mutable(op)));
                if (temp == nullptr) {
                    if consteval { __bad_any_access::bad_cast(); }
                    else { throw bad_any_access("bad any cast"); }
                }
                return *temp;
            }

            template <typename Any>
                requires (!std::is_lvalue_reference_v<Any> && !s_has_already_ref_adl_<Any>)
            constexpr auto operator()(Any&& op) const {
                auto* temp = (*this)(std::addressof(op));
                if (temp == nullptr) {
                    if consteval { __bad_any_access::bad_cast(); }
                    else { throw bad_any_access("bad any cast"); }
                }
                return std::move(*temp);
            }

            template <typename Any>
            constexpr auto operator()(const Any&&) const = delete;

        };

    } // inline namespace __cpo_access

    inline namespace __cpo {

        /// holds_current_type
        template <typename T>
        inline constexpr __cpo_access::holds_current_type_fn<T> holds_current_type {};

        /// any_cast
        template <typename T>
            requires (!std::is_reference_v<T>)
        inline constexpr __cpo_access::any_cast_fn<T> any_cast {};

    } // inline namespace __cpo

} // namespace gold

#endif // __GOLD_BITS_ANY_CPO_HPP
