// <gold/bits/expected/expected_impl.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_EXPECTED_IMPL_HPP
#define __GOLD_BITS_EXPECTED_IMPL_HPP

#include <gold/bits/expected/bad_access.hpp>
#include <gold/bits/expected/unexpected.hpp>
#include <variant>
#include <gold/bits/functional/invoke.hpp>

namespace gold {

    /// expected
    template <typename T, typename E>
    class expected {
      public:
        /// ::value_type
        using value_type = T;
        /// ::error_type
        using error_type = E;
        /// ::unexpected_type
        using unexpected_type = unexpected<E>;

      private:
        /// ::error_type_wrapper
        struct error_type_wrapper {
            error_type value;

            template <typename... Args>
            constexpr error_type_wrapper(Args&&... args)
            : value(std::forward<Args>(args)...) {}
        };

        /// ::s_value_type_
        using s_value_type_ = value_type;

        /// ::s_error_type_
        using s_error_type_ = error_type_wrapper;

        /// ::s_member_type_
        using s_member_type_ = std::variant<s_value_type_, s_error_type_>;

        /// .m_value_
        s_member_type_ m_value_;

        /// .mf_get_value_
        constexpr s_value_type_& mf_get_value_() & {
            return std::get<s_value_type_>(m_value_);
        }
        constexpr s_value_type_&& mf_get_value_() && {
            return std::move(std::get<s_value_type_>(m_value_));
        }
        constexpr const s_value_type_& mf_get_value_() const& {
            return std::get<s_value_type_>(m_value_);
        }
        constexpr const s_value_type_&& mf_get_value_() const&& {
            return std::move(std::get<s_value_type_>(m_value_));
        }

        /// .mf_get_error_wrapper_
        constexpr s_error_type_& mf_get_error_wrapper_() & {
            return std::get<s_error_type_>(m_value_);
        }
        constexpr s_error_type_&& mf_get_error_wrapper_() && {
            return std::move(std::get<s_error_type_>(m_value_));
        }
        constexpr const s_error_type_& mf_get_error_wrapper_() const& {
            return std::get<s_error_type_>(m_value_);
        }
        constexpr const s_error_type_&& mf_get_error_wrapper_() const&& {
            return std::move(std::get<s_error_type_>(m_value_));
        }

        /// .mf_get_error_
        constexpr error_type& mf_get_error_() & {
            return mf_get_error_wrapper_().value;
        }
        constexpr error_type&& mf_get_error_() && {
            return std::move(mf_get_error_wrapper_().value);
        }
        constexpr const error_type& mf_get_error_() const& {
            return mf_get_error_wrapper_().value;
        }
        constexpr const error_type&& mf_get_error_() const&& {
            return std::move(mf_get_error_wrapper_().value);
        }

        /// ::s_convert_from_other_variant_
        template <typename U, typename G>
        static constexpr auto s_convert_from_other_variant_(const std::variant<U, G>& other) {
            using return_type = std::variant<T, E>;
            if (std::holds_alternative<U>(other))
                return return_type(std::in_place_type<T>, std::get<U>(other));
            else
                return return_type(std::in_place_type<E>, std::get<G>(other));
        }

        template <typename U, typename G>
        static constexpr auto s_convert_from_other_variant_(std::variant<U, G>&& other) {
            using return_type = std::variant<T, E>;
            if (std::holds_alternative<U>(other))
                return return_type(std::in_place_type<T>, std::move(std::get<U>(other)));
            else
                return return_type(std::in_place_type<E>, std::move(std::get<G>(other)));
        }

      public:
        //// Constructors
        /// default ctor
        constexpr expected()
            requires std::default_initializable<T>
        = default;

        /// copy ctor
        constexpr expected(const expected&)
            requires (std::copy_constructible<T> && std::copy_constructible<E>)
         = default;

        /// move ctor
        constexpr expected(expected&&) noexcept
            requires (std::move_constructible<T> && std::move_constructible<E>)
         = default;

        /// conv copy ctor
        template <typename U, typename G>
        constexpr
            explicit(!std::is_convertible_v<const U&, T> || !std::is_convertible_v<const G&, E>)
        expected(const expected<U, G>& other)
        : m_value_(s_convert_from_other_variant_(other.m_value_)) {}

        /// conv move ctor
        template <typename U, typename G>
        constexpr
            explicit(!std::is_convertible_v<U, T> || !std::is_convertible_v<G, E>)
        expected(expected<U, G>&& other)
        : m_value_(s_convert_from_other_variant_(std::move(other.m_value_))) {}

        /// in-place value ctor
        // # 0
        template <typename... Args>
            requires std::constructible_from<T, Args...>
        constexpr explicit expected(std::in_place_t, Args&&... args)
        : m_value_(std::in_place_type<s_value_type_>, std::forward<Args>(args)...) {}

        // # 1
        template <typename U, typename... Args>
            requires std::constructible_from<T, std::initializer_list<U>, Args...>
        constexpr explicit expected(std::in_place_t, std::initializer_list<U> il, Args&&... args)
        : m_value_(std::in_place_type<s_value_type_>, il, std::forward<Args>(args)...) {}

        /// in-place error ctor
        // # 0
        template <typename... Args>
            requires std::constructible_from<E, Args...>
        constexpr explicit expected(unexpect_t, Args&&... args)
        : m_value_(std::in_place_type<s_error_type_>, std::forward<Args>(args)...) {}

        // # 1
        template <typename U, typename... Args>
            requires std::constructible_from<E, std::initializer_list<U>, Args...>
        constexpr explicit expected(unexpect_t, std::initializer_list<U> il, Args&&... args)
        : m_value_(std::in_place_type<s_error_type_>, il, std::forward<Args>(args)...) {}

        /// unexpected value ctor
        // # 0
        template <typename G = E>
            requires std::constructible_from<E, const G&>
        constexpr
            explicit(!std::is_convertible_v<const G&, E>)
        expected(const unexpected<G>& err)
        : m_value_(std::in_place_type<s_error_type_>, err.value()) {}

        // # 1
        template <typename G = E>
            requires std::constructible_from<E, G>
        constexpr
            explicit(!std::is_convertible_v<G, E>)
        expected(unexpected<G>&& err)
        : m_value_(std::in_place_type<s_error_type_>, std::move(err.value())) {}

        /// value-type ctor
        template <typename U = T>
            requires std::constructible_from<T, U>
        constexpr explicit(!std::is_convertible_v<U, T>) expected(U&& v)
        : m_value_(std::in_place_type<s_value_type_>, std::forward<U>(v)) {}

        /// dtor
        constexpr ~expected() = default;

        //// Assignments
        /// copy assignment
        constexpr expected& operator=(const expected&)
            requires (std::copyable<T> && std::copyable<E>)
        = default;

        /// move assignment
        constexpr expected& operator=(expected&&) noexcept
            requires (std::movable<T> && std::movable<E>)
        = default;

        /// unexpected-value assignment
        template <typename G = E>
            requires (
                std::constructible_from<E, const G&> &&
                std::assignable_from<E&, const G&>
            )
        constexpr expected& operator=(const unexpected<G>& err) {
            if (this->has_value())
                m_value_.template emplace<s_error_type_>(err.value());
            else
                m_value_ = s_error_type_{ err.value() };
            return *this;
        }

        template <typename G = E>
            requires (
                std::constructible_from<E, G> &&
                std::assignable_from<E&, G>
            )
        constexpr expected& operator=(unexpected<G>&& err) {
            if (this->has_value())
                m_value_.template emplace<s_error_type_>(std::move(err.value()));
            else
                m_value_ = s_error_type_{ std::move(err.value()) };
            return *this;
        }

        /// expected-value assignment
        template <typename U = T>
            requires std::constructible_from<T, U>
        constexpr expected& operator=(U&& arg) {
            if (this->has_value())
                m_value_ = std::forward<U>(arg);
            return *this;
        }

        /// emplace
        template <typename... Args>
            requires std::constructible_from<T, Args...>
        constexpr T& emplace(Args&&... args)
            noexcept(std::is_nothrow_constructible_v<T, Args...>)
        {
            return m_value_.template emplace<s_value_type_>(std::forward<Args>(args)...);
        }

        template <typename U, typename... Args>
            requires std::constructible_from<T, std::initializer_list<U>, Args...>
        constexpr T& emplace(std::initializer_list<U> il, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>, Args...>)
        {
            return m_value_.template emplace<s_value_type_>(il, std::forward<Args>(args)...);
        }

        /// swap
        constexpr void swap(expected& other)
            noexcept(
                std::is_nothrow_swappable_v<T> &&
                std::is_nothrow_swappable_v<E>
            )
            requires (std::swappable<T> && std::swappable<E>)
        {
            m_value_.swap(other.m_value_);
        }

        /// operator->
        // #0
        constexpr T* operator->() {
            if (!this->has_value())
                throw bad_expected_access<E>(mf_get_error_());
            return std::addressof(mf_get_value_());
        }
        // #1
        constexpr const T* operator->() const {
            if (!this->has_value())
                throw bad_expected_access<E>(mf_get_error_());
            return std::addressof(mf_get_value_());
        }

        /// operator*
        // #0
        constexpr T& operator*() & {
            return *this->operator->();
        }
        // #1
        constexpr T&& operator*() && {
            return std::move(*this->operator->());
        }
        // #2
        constexpr const T& operator*() const& {
            return *this->operator->();
        }
        // #3
        constexpr const T&& operator*() const&& {
            return std::move(*this->operator->());
        }

        /// operator bool
        constexpr explicit operator bool() const noexcept {
            return this->has_value();
        }

        /// has_value
        constexpr bool has_value() const noexcept {
            return std::holds_alternative<s_value_type_>(m_value_);
        }

        /// value
        // #0
        constexpr T& value() & {
            return this->operator*();
        }
        // #1
        constexpr T&& value() && {
            return std::move(this->operator*());
        }
        // #2
        constexpr const T& value() const& {
            return this->operator*();
        }
        // #3
        constexpr const T&& value() const&& {
            return std::move(this->operator*());
        }

        /// error
        // #0
        constexpr E& error() & {
            if (this->has_value())
                throw bad_unexpected_access();
            return mf_get_error_();
        }
        // #1
        constexpr E&& error() && {
            if (this->has_value())
                throw bad_unexpected_access();
            return std::move(mf_get_error_());
        }
        // #2
        constexpr const E& error() const& {
            if (this->has_value())
                throw bad_unexpected_access();
            return mf_get_error_();
        }
        // #3
        constexpr const E&& error() const&& {
            if (this->has_value())
                throw bad_unexpected_access();
            return std::move(mf_get_error_());
        }

        /// value_or
        // #0
        template <typename U = T>
            requires (std::copy_constructible<T> && std::convertible_to<U, T>)
        constexpr T value_or(U&& arg) const& noexcept {
            return *this ? **this : static_cast<T>(std::forward<U>(arg));
        }
        // #1
        template <typename U = T>
            requires (std::move_constructible<T> && std::convertible_to<U, T>)
        constexpr T value_or(U&& arg) && noexcept {
            return *this ? std::move(**this) : static_cast<T>(std::forward<U>(arg));
        }

        /// error_or
        // #0
        template <typename G = E>
            requires (std::copy_constructible<E> && std::convertible_to<G, E>)
        constexpr E error_or(G&& arg) const& noexcept{
            return !*this ? this->error() : static_cast<E>(std::forward<G>(arg));
        }

        // #1
        template <typename G = E>
            requires (std::move_constructible<E> && std::convertible_to<G, E>)
        constexpr E error_or(G&& arg) && noexcept{
            return !*this ? std::move(this->error()) : static_cast<E>(std::forward<G>(arg));
        }

        //// Monadic Operations
        /// and_then
        // #0
        template <
            std::invocable<value_type&> F,
            std::copy_constructible U = std::invoke_result_t<F, value_type&>
        > requires (
            std::copy_constructible<E> &&
            gold::is_specialization_of_v<std::remove_cvref_t<U>, gold::expected>()
        )
        constexpr auto and_then(F&& f) & {
            if (*this)
                return gold::invoke(std::forward<F>(f), **this);
            else
                return *this;
        }
        // #1
        template <
            std::invocable<value_type> F,
            std::move_constructible U = std::invoke_result_t<F, value_type>
        > requires (
            std::move_constructible<E> &&
            gold::is_specialization_of_v<std::remove_cvref_t<U>, gold::expected>()
        )
        constexpr auto and_then(F&& f) && {
            if (*this)
                return gold::invoke(std::forward<F>(f), std::move(**this));
            else
                return std::move(*this);
        }
        // #2
        template <
            std::invocable<const value_type&> F,
            std::copy_constructible U = std::invoke_result_t<F, const value_type&>
        > requires (
            std::copy_constructible<E> &&
            gold::is_specialization_of_v<std::remove_cvref_t<U>, gold::expected>()
        )
        constexpr auto and_then(F&& f) const& {
            if (*this)
                return gold::invoke(std::forward<F>(f), **this);
            else
                return *this;
        }
        // #3
        template <
            std::invocable<const value_type> F,
            std::move_constructible U = std::invoke_result_t<F, const value_type>
        > requires (
            std::move_constructible<E> &&
            gold::is_specialization_of_v<std::remove_cvref_t<U>, gold::expected>()
        )
        constexpr auto and_then(F&& f) const&& {
            if (*this)
                return gold::invoke(std::forward<F>(f), std::move(**this));
            else
                return std::move(*this);
        }

        /// transform
        template <
            std::invocable<value_type&> F,
            std::copy_constructible U = std::invoke_result_t<F, value_type&>
        > requires std::copy_constructible<E>
        constexpr auto transform(F&& f) & {
            if (*this)
                return gold::expected<U, E>(std::in_place, gold::invoke(std::forward<F>(f), **this));
            else
                return *this;
        }

        template <
            std::invocable<value_type> F,
            std::move_constructible U = std::invoke_result_t<F, value_type>
        > requires std::move_constructible<E>
        constexpr auto transform(F&& f) && {
            if (*this)
                return gold::expected<U, E>(std::in_place, gold::invoke(std::forward<F>(f), std::move(**this)));
            else
                return std::move(*this);
        }

        template <
            std::invocable<const value_type&> F,
            std::copy_constructible U = std::invoke_result_t<F, const value_type&>
        > requires std::copy_constructible<E>
        constexpr auto transform(F&& f) const& {
            if (*this)
                return gold::expected<U, E>(std::in_place, gold::invoke(std::forward<F>(f), **this));
            else
                return *this;
        }

        template <
            std::invocable<const value_type> F,
            std::move_constructible U = std::invoke_result_t<F, const value_type>
        > requires std::move_constructible<E>
        constexpr auto transform(F&& f) const&& {
            if (*this)
                return gold::expected<U, E>(std::in_place, gold::invoke(std::forward<F>(f), std::move(**this)));
            else
                return std::move(*this);
        }

        /// error_transform
        template <
            std::invocable<error_type&> F,
            std::copy_constructible G = std::invoke_result_t<F, error_type&>
        > requires std::copy_constructible<T>
        constexpr auto error_transform(F&& f) & {
            if (!*this)
                return gold::expected<T, G>(unexpect, gold::invoke(std::forward<F>(f), this->error()));
            else
                return *this;
        }

        template <
            std::invocable<error_type> F,
            std::move_constructible G = std::invoke_result_t<F, error_type>
        > requires std::move_constructible<T>
        constexpr auto error_transform(F&& f) && {
            if (!*this)
                return gold::expected<T, G>(unexpect, gold::invoke(std::forward<F>(f), std::move(this->error())));
            else
                return std::move(*this);
        }

        template <
            std::invocable<const error_type&> F,
            std::copy_constructible G = std::invoke_result_t<F, const error_type&>
        > requires std::copy_constructible<T>
        constexpr auto error_transform(F&& f) const& {
            if (!*this)
                return gold::expected<T, G>(unexpect, gold::invoke(std::forward<F>(f), this->error()));
            else
                return *this;
        }

        template <
            std::invocable<const error_type> F,
            std::move_constructible G = std::invoke_result_t<F, const error_type>
        > requires std::move_constructible<T>
        constexpr auto error_transform(F&& f) const&& {
            if (!*this)
                return gold::expected<T, G>(unexpect, gold::invoke(std::forward<F>(f), std::move(this->error())));
            else
                return std::move(*this);
        }

        /// or_else
        // #0
        template <std::invocable F, typename U = std::invoke_result_t<F>>
            requires (
                std::copy_constructible<E> &&
                (std::same_as<std::remove_cvref_t<U>, expected> || std::is_void_v<U>)
            )
        constexpr auto or_else(F&& f) const& {
            if constexpr (std::is_void_v<U>) {
                if (!*this)
                    gold::invoke_r<void>(std::forward<F>(f));
            } else {
                return *this ? *this : gold::invoke(std::forward<F>(f));
            }
        }
        // #1
        template <std::invocable F, typename U = std::invoke_result_t<F>>
            requires (
                std::move_constructible<E> &&
                (std::same_as<std::remove_cvref_t<U>, expected> || std::is_void_v<U>)
            )
        constexpr auto or_else(F&& f) && {
            if constexpr (std::is_void_v<U>) {
                if (!*this)
                    gold::invoke_r<void>(std::forward<F>(f));
            } else {
                return *this ? std::move(*this) : gold::invoke(std::forward<F>(f));
            }
        }
        // #2
        template <std::invocable<error_type&> F, typename U = std::invoke_result_t<F, error_type&>>
            requires (
                std::copy_constructible<E> &&
                (std::same_as<std::remove_cvref_t<U>, expected> || std::is_void_v<U>)
            )
        constexpr auto or_else(F&& f) & {
            if constexpr (std::is_void_v<U>) {
                if (!*this)
                    gold::invoke_r<void>(std::forward<F>(f), this->error());
            } else {
                return *this ? *this : gold::invoke(std::forward<F>(f), this->error());
            }
        }
        // #3
        template <std::invocable<error_type> F, typename U = std::invoke_result_t<F, error_type>>
            requires (
                std::move_constructible<E> &&
                (std::same_as<std::remove_cvref_t<U>, expected> || std::is_void_v<U>)
            )
        constexpr auto or_else(F&& f) && {
            if constexpr (std::is_void_v<U>) {
                if (!*this)
                    gold::invoke_r<void>(std::forward<F>(f), std::move(this->error()));
            } else {
                return *this ? std::move(*this) : gold::invoke(std::forward<F>(f), std::move(this->error()));
            }
        }
        // #4
        template <std::invocable<const error_type&> F, typename U = std::invoke_result_t<F, const error_type&>>
            requires (
                std::copy_constructible<E> &&
                (std::same_as<std::remove_cvref_t<U>, expected> || std::is_void_v<U>)
            )
        constexpr auto or_else(F&& f) const& {
            if constexpr (std::is_void_v<U>) {
                if (!*this)
                    gold::invoke_r<void>(std::forward<F>(f), this->error());
            } else {
                return *this ? *this : gold::invoke(std::forward<F>(f), this->error());
            }
        }
        // #5
        template <std::invocable<const error_type> F, typename U = std::invoke_result_t<F, const error_type>>
            requires (
                std::move_constructible<E> &&
                (std::same_as<std::remove_cvref_t<U>, expected> || std::is_void_v<U>)
            )
        constexpr auto or_else(F&& f) const&& {
            if constexpr (std::is_void_v<U>) {
                if (!*this)
                    gold::invoke_r<void>(std::forward<F>(f), std::move(this->error()));
            } else {
                return *this ? std::move(*this) : gold::invoke(std::forward<F>(f), std::move(this->error()));
            }
        }
    };

    /// expected<void, E>
    template <typename T, typename E>
        requires std::is_void_v<T>
    class expected<T, E> {
      public:
        /// ::value_type
        using value_type = T;
        /// ::error_type
        using error_type = E;
        /// ::unexpected_type
        using unexpected_type = unexpected<E>;

      private:
        /// ::s_member_type_
        using s_member_type_ = std::variant<std::monostate, error_type>;

        /// .m_value_
        s_member_type_ m_value_;

        /// .mf_get_error_
        constexpr error_type& mf_get_error_() & {
            return std::get<error_type>(m_value_);
        }
        constexpr error_type&& mf_get_error_() && {
            return std::move(std::get<error_type>(m_value_));
        }
        constexpr const error_type& mf_get_error_() const& {
            return std::get<error_type>(m_value_);
        }
        constexpr const error_type&& mf_get_error_() const&& {
            return std::move(std::get<error_type>(m_value_));
        }

        /// ::s_convert_from_other_variant_
        template <typename G>
        static constexpr auto s_convert_from_other_variant_(const std::variant<std::monostate, G>& other) {
            using return_type = std::variant<std::monostate, E>;
            if (std::holds_alternative<G>(other))
                return return_type(std::in_place_type<E>, std::get<G>(other));
            else
                return return_type();
        }

        template <typename G>
        static constexpr auto s_convert_from_other_variant_(std::variant<std::monostate, G>&& other) {
            using return_type = std::variant<std::monostate, E>;
            if (std::holds_alternative<G>(other))
                return return_type(std::in_place_type<E>, std::move(std::get<G>(other)));
            else
                return return_type();
        }

      public:
        //// Constructors
        /// default ctor
        constexpr expected() noexcept = default;

        /// copy ctor
        constexpr expected(const expected&)
            requires (std::copy_constructible<E>)
         = default;

        /// move ctor
        constexpr expected(expected&&) noexcept
            requires (std::move_constructible<E>)
         = default;

        /// conv copy ctor
        template <typename U, typename G>
            requires std::is_void_v<U>
        constexpr
            explicit(!std::is_convertible_v<const G&, E>)
        expected(const expected<U, G>& other)
        : m_value_(s_convert_from_other_variant_(other.m_value_)) {}

        /// conv move ctor
        template <typename U, typename G>
            requires std::is_void_v<U>
        constexpr
            explicit(!std::is_convertible_v<G, E>)
        expected(expected<U, G>&& other)
        : m_value_(s_convert_from_other_variant_(std::move(other.m_value_))) {}

        /// in-place value ctor
        constexpr explicit expected(std::in_place_t)
        : m_value_() {}

        /// in-place error ctor
        // # 0
        template <typename... Args>
            requires std::constructible_from<E, Args...>
        constexpr explicit expected(unexpect_t, Args&&... args)
        : m_value_(std::in_place_type<error_type>, std::forward<Args>(args)...) {}

        // # 1
        template <typename U, typename... Args>
            requires std::constructible_from<E, std::initializer_list<U>, Args...>
        constexpr explicit expected(unexpect_t, std::initializer_list<U> il, Args&&... args)
        : m_value_(std::in_place_type<error_type>, il, std::forward<Args>(args)...) {}

        /// unexpected value ctor
        // # 0
        template <typename G = E>
            requires std::constructible_from<E, const G&>
        constexpr
            explicit(!std::is_convertible_v<const G&, E>)
        expected(const unexpected<G>& err)
        : m_value_(std::in_place_type<error_type>, err.value()) {}

        // # 1
        template <typename G = E>
            requires std::constructible_from<E, G>
        constexpr
            explicit(!std::is_convertible_v<G, E>)
        expected(unexpected<G>&& err)
        : m_value_(std::in_place_type<error_type>, std::move(err.value())) {}

        /// dtor
        constexpr ~expected() = default;

        //// Assignments
        /// copy assignment
        constexpr expected& operator=(const expected&)
            requires std::copyable<E>
        = default;

        /// move assignment
        constexpr expected& operator=(expected&&) noexcept
            requires std::movable<E>
        = default;

        /// unexpected-value assignment
        // #0
        template <typename G = E>
            requires (
                std::constructible_from<E, const G&> &&
                std::assignable_from<E&, const G&>
            )
        constexpr expected& operator=(const unexpected<G>& err) {
            m_value_ = err.value();
            return *this;
        }
        // #1
        template <typename G = E>
            requires (
                std::constructible_from<E, G> &&
                std::assignable_from<E&, G>
            )
        constexpr expected& operator=(unexpected<G>&& err) {
            m_value_ = std::move(err.value());
            return *this;
        }

        /// emplace
        constexpr void emplace() noexcept {
            m_value_.template emplace<std::monostate>();
        }

        /// swap
        constexpr void swap(expected& other)
            noexcept(std::is_nothrow_swappable_v<E>)
            requires std::swappable<E>
        {
            m_value_.swap(other.m_value_);
        }

        /// operator*
        constexpr void operator*() const noexcept {
            if (!this->has_value())
                throw bad_expected_access<E>(mf_get_error_());
        }

        /// operator bool
        constexpr explicit operator bool() const noexcept {
            return this->has_value();
        }

        /// has_value
        constexpr bool has_value() const noexcept {
            return std::holds_alternative<std::monostate>(m_value_);
        }

        /// value
        // #0
        constexpr void value() const& {
            if (!this->has_value())
                throw bad_expected_access<E>(mf_get_error_());
        }
        // #1
        constexpr void value() && {
            if (!this->has_value())
                throw bad_expected_access<E>(std::move(mf_get_error_()));
        }

        /// error
        // #0
        constexpr E& error() & {
            if (this->has_value())
                throw bad_unexpected_access();
            return mf_get_error_();
        }
        // #1
        constexpr E&& error() && {
            if (this->has_value())
                throw bad_unexpected_access();
            return std::move(mf_get_error_());
        }
        // #2
        constexpr const E& error() const& {
            if (this->has_value())
                throw bad_unexpected_access();
            return mf_get_error_();
        }
        // #3
        constexpr const E&& error() const&& {
            if (this->has_value())
                throw bad_unexpected_access();
            return std::move(mf_get_error_());
        }

        /// error_or
        // #0
        template <typename G = E>
            requires (std::copy_constructible<E> && std::convertible_to<G, E>)
        constexpr E error_or(G&& arg) const& noexcept{
            return !*this ? this->error() : static_cast<E>(std::forward<G>(arg));
        }

        // #1
        template <typename G = E>
            requires (std::move_constructible<E> && std::convertible_to<G, E>)
        constexpr E error_or(G&& arg) && noexcept{
            return !*this ? std::move(this->error()) : static_cast<E>(std::forward<G>(arg));
        }

        //// Monadic Operations
        /// and_then
        // #0
        template <std::invocable F>
            requires (
                std::copy_constructible<E> &&
                std::same_as<std::remove_cvref_t<std::invoke_result_t<F>>, expected>
            )
        constexpr auto and_then(F&& f) const& {
            if (*this)
                return gold::invoke(std::forward<F>(f));
            else
                return *this;
        }
        // #1
        template <std::invocable F>
            requires (
                std::move_constructible<E> &&
                std::same_as<std::remove_cvref_t<std::invoke_result_t<F>>, expected>
            )
        constexpr auto and_then(F&& f) && {
            if (*this)
                return gold::invoke(std::forward<F>(f));
            else
                return std::move(*this);
        }

        /// error_transform
        template <
            std::invocable<error_type&> F,
            std::copy_constructible G = std::invoke_result_t<F, error_type&>
        > requires std::copy_constructible<T>
        constexpr auto error_transform(F&& f) & {
            if (!*this)
                return gold::expected<T, G>(unexpect, gold::invoke(std::forward<F>(f), this->error()));
            else
                return *this;
        }

        template <
            std::invocable<error_type> F,
            std::move_constructible G = std::invoke_result_t<F, error_type>
        > requires std::move_constructible<T>
        constexpr auto error_transform(F&& f) && {
            if (!*this)
                return gold::expected<T, G>(unexpect, gold::invoke(std::forward<F>(f), std::move(this->error())));
            else
                return std::move(*this);
        }

        template <
            std::invocable<const error_type&> F,
            std::copy_constructible G = std::invoke_result_t<F, const error_type&>
        > requires std::copy_constructible<T>
        constexpr auto error_transform(F&& f) const& {
            if (!*this)
                return gold::expected<T, G>(unexpect, gold::invoke(std::forward<F>(f), this->error()));
            else
                return *this;
        }

        template <
            std::invocable<const error_type> F,
            std::move_constructible G = std::invoke_result_t<F, const error_type>
        > requires std::move_constructible<T>
        constexpr auto error_transform(F&& f) const&& {
            if (!*this)
                return gold::expected<T, G>(unexpect, gold::invoke(std::forward<F>(f), std::move(this->error())));
            else
                return std::move(*this);
        }

        /// or_else
        // #0
        template <std::invocable F, typename U = std::invoke_result_t<F>>
            requires (
                std::copy_constructible<E> &&
                (std::same_as<std::remove_cvref_t<U>, expected> || std::is_void_v<U>)
            )
        constexpr auto or_else(F&& f) const& {
            if constexpr (std::is_void_v<U>) {
                if (!*this)
                    gold::invoke_r<void>(std::forward<F>(f));
            } else {
                return *this ? *this : gold::invoke(std::forward<F>(f));
            }
        }
        // #1
        template <std::invocable F, typename U = std::invoke_result_t<F>>
            requires (
                std::move_constructible<E> &&
                (std::same_as<std::remove_cvref_t<U>, expected> || std::is_void_v<U>)
            )
        constexpr auto or_else(F&& f) && {
            if constexpr (std::is_void_v<U>) {
                if (!*this)
                    gold::invoke_r<void>(std::forward<F>(f));
            } else {
                return *this ? std::move(*this) : gold::invoke(std::forward<F>(f));
            }
        }
        // #2
        template <std::invocable<error_type&> F, typename U = std::invoke_result_t<F, error_type&>>
            requires (
                std::copy_constructible<E> &&
                (std::same_as<std::remove_cvref_t<U>, expected> || std::is_void_v<U>)
            )
        constexpr auto or_else(F&& f) & {
            if constexpr (std::is_void_v<U>) {
                if (!*this)
                    gold::invoke_r<void>(std::forward<F>(f), this->error());
            } else {
                return *this ? *this : gold::invoke(std::forward<F>(f), this->error());
            }
        }
        // #3
        template <std::invocable<error_type> F, typename U = std::invoke_result_t<F, error_type>>
            requires (
                std::move_constructible<E> &&
                (std::same_as<std::remove_cvref_t<U>, expected> || std::is_void_v<U>)
            )
        constexpr auto or_else(F&& f) && {
            if constexpr (std::is_void_v<U>) {
                if (!*this)
                    gold::invoke_r<void>(std::forward<F>(f), std::move(this->error()));
            } else {
                return *this ? std::move(*this) : gold::invoke(std::forward<F>(f), std::move(this->error()));
            }
        }
        // #4
        template <std::invocable<const error_type&> F, typename U = std::invoke_result_t<F, const error_type&>>
            requires (
                std::copy_constructible<E> &&
                (std::same_as<std::remove_cvref_t<U>, expected> || std::is_void_v<U>)
            )
        constexpr auto or_else(F&& f) const& {
            if constexpr (std::is_void_v<U>) {
                if (!*this)
                    gold::invoke_r<void>(std::forward<F>(f), this->error());
            } else {
                return *this ? *this : gold::invoke(std::forward<F>(f), this->error());
            }
        }
        // #5
        template <std::invocable<const error_type> F, typename U = std::invoke_result_t<F, const error_type>>
            requires (
                std::move_constructible<E> &&
                (std::same_as<std::remove_cvref_t<U>, expected> || std::is_void_v<U>)
            )
        constexpr auto or_else(F&& f) const&& {
            if constexpr (std::is_void_v<U>) {
                if (!*this)
                    gold::invoke_r<void>(std::forward<F>(f), std::move(this->error()));
            } else {
                return *this ? std::move(*this) : gold::invoke(std::forward<F>(f), std::move(this->error()));
            }
        }

    };

    /// operator== for expected
    // #0
    template <typename T, typename E, typename U, typename G>
        requires (!std::is_void_v<T> && !std::is_void_v<U>)
    constexpr bool operator==(const expected<T, E>& lhs, const expected<U, G>& rhs) {
        return (
            lhs.has_value() != rhs.has_value()
            ? false                      : !lhs.has_value()
            ? lhs.error() == rhs.error() : *lhs == *rhs
        );
    }
    // #1
    template <typename T, typename E, typename U, typename G>
        requires (std::is_void_v<T> && std::is_void_v<T>)
    constexpr bool operator==(const expected<T, E>& lhs, const expected<U, G>& rhs) {
        return lhs.has_value() || static_cast<bool>(lhs.error() == rhs.error());
    }
    // #2
    template <typename T, typename E, typename U>
        requires (!std::is_void_v<T>)
    constexpr bool operator==(const expected<T, E>& lhs, const U& rhs) {
        return lhs.has_value() && static_cast<bool>(*lhs == rhs);
    }
    // #3
    template <typename T, typename E, typename G>
    constexpr bool operator==(const expected<T, E>& lhs, const unexpected<G>& rhs) {
        return !lhs.has_value() && static_cast<bool>(lhs.error() == rhs.value());
    }

} // namespace gold

#endif // __GOLD_BITS_EXPECTED_IMPL_HPP
