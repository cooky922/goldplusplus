// <gold/bits/expected/expected_impl.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_EXPECTED_IMPL_HPP
#define __GOLD_BITS_EXPECTED_IMPL_HPP

#include <gold/bits/__util/empty_t.hpp>
#include <utility>
#include <gold/bits/expected/utils.hpp>
#include <gold/bits/expected/bad_access.hpp>
#include <gold/bits/expected/unexpected.hpp>
#include <gold/bits/concepts/types.hpp>
#include <gold/bits/functional/invoke.hpp>
#include <gold/bits/memory/ops.hpp>

namespace gold {

    namespace __expected {

        /// __expected::_t
        template <typename T>
        using _t = std::type_identity_t<T>;

        /// __expected::construct_guard
        // note: T is required to be nothrow-move-constructible
        template <typename T>
        class construct_guard {
          private:
            T* m_guarded_;
            T  m_temp_;

          public:
            explicit constexpr construct_guard(T& op)
            : m_guarded_(std::addressof(op)), m_temp_(std::move(op)) {
                gold::destroy_at(m_guarded_);
            }

            constexpr ~construct_guard() {
                if (m_guarded_) [[unlikely]]
                    gold::construct_at(m_guarded_, std::move(m_temp_));
            }

            constexpr construct_guard(const construct_guard&) = delete;
            constexpr construct_guard& operator=(const construct_guard&) = delete;

            constexpr T&& release() noexcept {
                m_guarded_ = nullptr;
                return std::move(m_temp_);
            }
        };

        /// __expected::reinit
        template <typename T, typename U, typename V>
        constexpr void reinit(T* new_val, U* old_val, V&& arg)
            noexcept(std::is_nothrow_constructible_v<T, V>) {
            if constexpr (std::is_nothrow_constructible_v<T, V>) {
                gold::destroy_at(old_val);
                gold::construct_at(new_val, std::forward<V>(arg));
            } else if constexpr (std::is_nothrow_move_constructible_v<T>) {
                gold::destroy_at(old_val);
                gold::construct_at(new_val, auto(std::forward<V>(arg)));
            } else {
                construct_guard<U> guard { *old_val };
                gold::construct_at(new_val, std::forward<V>(arg)); // might throw
                guard.release();
            }
        }

        /// __expected::constructible_from_expected
        template <typename T, typename E, typename Ex, typename Unex = unexpected<E>>
        concept constructible_from_expected =
               std::is_constructible_v<T, Ex&>
            || std::is_constructible_v<T, const Ex&>
            || std::is_constructible_v<T, Ex>
            || std::is_constructible_v<T, const Ex>
            || std::is_convertible_v<Ex&, T>
            || std::is_convertible_v<const Ex&, T>
            || std::is_convertible_v<Ex, T>
            || std::is_convertible_v<const Ex, T>
            || std::is_constructible_v<Unex, Ex&>
            || std::is_constructible_v<Unex, const Ex&>
            || std::is_constructible_v<Unex, Ex>
            || std::is_constructible_v<Unex, const Ex>;

        /// __expected::void_constructible_from_expected
        template <typename E, typename Ex, typename Unex = unexpected<E>>
        concept void_constructible_from_expected =
               std::is_constructible_v<Unex, Ex&>
            || std::is_constructible_v<Unex, const Ex&>
            || std::is_constructible_v<Unex, Ex>
            || std::is_constructible_v<Unex, const Ex>;

        /// __expected::explicit_expected_convertible
        template <typename T, typename E, typename U, typename G>
        concept explicit_expected_convertible =
               !std::is_convertible_v<U, T>
            || !std::is_convertible_v<G, E>;

        /// __expected::or_else_invocable_impl
        template <typename Value, typename U>
        concept or_else_invocable_impl =
               std::is_void_v<U>
            || (std::constructible_from<std::remove_cvref_t<Value>, Value>
             && is_expected_v<U>
             && std::same_as<typename U::value_type, std::remove_cvref_t<Value>>);

        /// __expected::void_or_else_invocable_impl
        template <typename U>
        concept void_or_else_invocable_impl =
            std::is_void_v<U>
            || (is_expected_v<U>
             && std::is_void_v<typename U::value_type>);

        /// __expected::or_else_invocable
        template <typename F, typename Value, typename Error, bool VoidExpected = false>
        concept or_else_invocable =
               (std::invocable<F>
             && []{
                if constexpr (VoidExpected)
                    return void_or_else_invocable_impl<std::remove_cvref_t<std::invoke_result_t<F>>>;
                else
                    return or_else_invocable_impl<Value, std::remove_cvref_t<std::invoke_result_t<F>>>;
             }())
            || (std::invocable<F, Error>
             && []{
                if constexpr (VoidExpected)
                    return void_or_else_invocable_impl<std::remove_cvref_t<std::invoke_result_t<F, Error>>>;
                else
                    return or_else_invocable_impl<Value, std::remove_cvref_t<std::invoke_result_t<F, Error>>>;
             }());

        /// __expected::or_else_result
        template <typename F, typename Value, typename Error>
        struct or_else_result {};

        template <typename F, typename Value, typename Error>
            requires std::invocable<F>
        struct or_else_result<F, Value, Error> {
            using type = std::remove_cvref_t<std::invoke_result_t<F>>;
            inline static constexpr bool is_nullary = true;
        };

        template <typename F, typename Value, typename Error>
            requires std::invocable<F, Error>
        struct or_else_result<F, Value, Error> {
            using type = std::remove_cvref_t<std::invoke_result_t<F, Error>>;
            inline static constexpr bool is_nullary = false;
        };

        /// __expected::can_be_transform_result
        template <typename T>
        concept can_be_transform_result =
            std::is_void_v<T>
             || (std::is_object_v<T>
             && !std::is_array_v<T>
             && !std::is_same_v<T, std::in_place_t>
             && !std::is_same_v<T, unexpect_t>
             && !is_unexpected_v<T>);

        /// __expected::can_be_transform_error_result
        template <typename T>
        concept can_be_transform_error_result =
                 std::is_object_v<T>
             && !std::is_array_v<T>
             && !std::is_same_v<T, std::in_place_t>
             && !std::is_same_v<T, unexpect_t>
             && !is_unexpected_v<T>;

        /// __expected::destroy_at
        template <typename T>
        constexpr void destroy_at(T* ptr) noexcept {
            if constexpr (!std::is_trivially_destructible_v<T>)
                ptr->~T();
        }

    } // namespace __expected

    /// expected
    template <typename T, typename E>
        requires __expected::can_be_expected<T, E>
    class expected {
      private:
        union {
            __util::empty_t m_invalid_;
            T m_val_;
            E m_unex_;
        };
        bool m_has_val_;

        /// ::mf_assign_val_
        template <typename U>
        constexpr void mf_assign_val_(U&& u) {
            if (m_has_val_) {
                if constexpr (std::is_assignable_v<T&, U>) {
                    m_val_ = std::forward<U>(u);
                } else {
                    gold::destroy_at(std::addressof(m_val_));
                    gold::construct_at(std::addressof(m_val_), std::forward<U>(u));
                }
            } else {
                __expected::reinit(std::addressof(m_val_), std::addressof(m_unex_), std::forward<U>(u));
                m_has_val_ = true;
            }
        }

        /// ::mf_assign_unex_
        template <typename G>
        constexpr void mf_assign_unex_(G&& g) {
            if (m_has_val_) {
                __expected::reinit(std::addressof(m_unex_), std::addressof(m_val_), std::forward<G>(g));
                m_has_val_ = false;
            } else {
                if constexpr (std::is_assignable_v<E&, G>) {
                    m_unex_ = std::forward<G>(g);
                } else {
                    gold::destroy_at(std::addressof(m_unex_));
                    gold::construct_at(std::addressof(m_unex_), std::forward<G>(g));
                }
            }
        }

        /// ::mf_swap_val_unex_
        // before: this (has value), other (has no value)
        // after:  this (has no value), other (has value)
        constexpr void mf_swap_val_unex_(expected& other)
            noexcept(std::is_nothrow_move_constructible_v<T>
                  && std::is_nothrow_move_constructible_v<E>)
        {
            if constexpr (std::is_nothrow_move_constructible_v<E>) {
                __expected::construct_guard<E> guard (other.m_unex_);
                gold::construct_at(std::addressof(other.m_val_), std::move(m_val_));
                other.m_has_val_ = true;
                gold::destroy_at(std::addressof(m_val_));
                gold::construct_at(std::addressof(m_unex_), guard.release());
                m_has_val_ = false;
            } else {
                __expected::construct_guard<T> guard (m_val_);
                gold::construct_at(std::addressof(m_unex_), std::move(other.m_unex_));
                m_has_val_ = false;
                gold::destroy_at(std::addressof(other.m_unex_));
                gold::construct_at(std::addressof(other.m_val_), guard.release());
                other.m_has_val_ = true;
            }
        }

        /// ::s_can_do_value_or_else_
        template <typename F, typename Self>
        static consteval bool s_can_do_value_or_else_() {
            constexpr bool is_xible   = [] -> bool {
                constexpr bool is_copy_op = std::is_lvalue_reference_v<Self>;
                if constexpr (is_copy_op)
                    return std::is_copy_constructible_v<T>;
                else
                    return std::is_move_constructible_v<T>;
            }();
            if constexpr (!is_xible)
                return false;
            else {
                using Ed = __util::merge_cvref_t<E, Self>;
                if constexpr (std::is_invocable_v<F, Ed>)
                    return std::convertible_to<std::invoke_result_t<F, Ed>, T>;
                else if constexpr (std::is_invocable_v<F>)
                    return std::convertible_to<std::invoke_result_t<F>, T>;
                else
                    return false;
            }
        }

        /// ::s_value_or_else_noexcept_
        template <typename F, typename Self>
        static consteval bool s_value_or_else_noexcept_() {
            using Ed = __util::merge_cvref_t<E, Self>;
            bool result;
            if constexpr (std::is_lvalue_reference_v<Self>)
                result = std::is_nothrow_copy_constructible_v<T>;
            else
                result = std::is_nothrow_move_constructible_v<T>;
            if constexpr (std::is_invocable_v<F, Ed>)
                result = result && std::is_nothrow_invocable_v<F, Ed>
                                && std::is_nothrow_convertible_v<std::invoke_result_t<F, Ed>, T>;
            else
                result = result && std::is_nothrow_invocable_v<F>
                                && std::is_nothrow_convertible_v<std::invoke_result_t<F>, T>;
            return result;
        }

        /// ::s_can_do_error_or_else_
        template <typename F, typename Self>
        static consteval bool s_can_do_error_or_else_() {
            constexpr bool is_xible   = [] -> bool {
                constexpr bool is_copy_op = std::is_lvalue_reference_v<Self>;
                if constexpr (is_copy_op)
                    return std::is_copy_constructible_v<E>;
                else
                    return std::is_move_constructible_v<E>;
            }();
            if constexpr (!is_xible)
                return false;
            else {
                using Td = __util::merge_cvref_t<T, Self>;
                if constexpr (std::is_invocable_v<F, Td>)
                    return std::convertible_to<std::invoke_result_t<F, Td>, E>;
                else if constexpr (std::is_invocable_v<F>)
                    return std::convertible_to<std::invoke_result_t<F>, E>;
                else
                    return false;
            }
        }

        /// ::s_error_or_else_noexcept_
        template <typename F, typename Self>
        static consteval bool s_error_or_else_noexcept_() {
            using Td = __util::merge_cvref_t<T, Self>;
            bool result;
            if constexpr (std::is_lvalue_reference_v<Self>)
                result = std::is_nothrow_copy_constructible_v<E>;
            else
                result = std::is_nothrow_move_constructible_v<E>;
            if constexpr (std::is_invocable_v<F, Td>)
                result = result && std::is_nothrow_invocable_v<F, Td>
                                && std::is_nothrow_convertible_v<std::invoke_result_t<F, Td>, E>;
            else
                result = result && std::is_nothrow_invocable_v<F>
                                && std::is_nothrow_convertible_v<std::invoke_result_t<F>, E>;
            return result;
        }

        /// ::s_can_do_and_then_
        template <typename F, typename Self>
        static consteval bool s_can_do_and_then_() {
            using Td = __util::merge_cvref_t<T, Self>;
            using Ed = __util::merge_cvref_t<E, Self>;
            if constexpr (std::invocable<F, Td>) {
                using Ud = std::invoke_result_t<F, Td>;
                using U  = std::remove_cvref_t<Ud>;
                if constexpr (std::constructible_from<E, Ed> &&
                              __expected::is_expected_v<U>)
                    return std::same_as<typename U::error_type, E>;
                return false;
            }
            return false;
        }

        /// ::s_can_do_or_else_
        template <typename F, typename Self>
        static consteval bool s_can_do_or_else_() {
            return __expected::or_else_invocable<F,
                __util::merge_cvref_t<T, Self>,
                __util::merge_cvref_t<E, Self>
            >;
        }

        /// ::s_or_else_
        template <typename F, typename Self>
        static constexpr auto s_or_else_(F&& f, Self&& self) {
            using Result = __expected::or_else_result<
                F,
                __util::merge_cvref_t<T, Self>,
                __util::merge_cvref_t<E, Self>
            >;
            using U      = Result::type;
            if constexpr (std::is_void_v<U>) {
                if (!self.m_has_val_) {
                    if constexpr (Result::is_nullary)
                        gold::invoke_r<void>(std::forward<F>(f));
                    else
                        gold::invoke_r<void>(std::forward<F>(f), std::forward_like<Self>(self.m_unex_));
                }
            } else {
                if (self.m_has_val_)
                    return U(std::in_place, std::forward_like<Self>(self.m_val_));
                else {
                    if constexpr (Result::is_nullary)
                        return gold::invoke(std::forward<F>(f));
                    else
                        return gold::invoke(std::forward<F>(f), std::forward_like<Self>(self.m_unex_));
                }
            }
        }

        /// ::s_can_do_transform_
        template <typename F, typename Self>
        static consteval bool s_can_do_transform_() {
            using Td = __util::merge_cvref_t<T, Self>;
            using Ed = __util::merge_cvref_t<E, Self>;
            if constexpr (std::invocable<F, Td>) {
                using Ud = std::invoke_result_t<F, Td>;
                using U  = std::remove_cvref_t<Ud>;
                if constexpr (__expected::can_be_transform_result<U>)
                    return std::constructible_from<E, Ed>
                       && (std::is_void_v<U> || std::constructible_from<U, Ud>);
                return false;
            }
            return false;
        }

        /// ::s_can_do_transform_error_
        template <typename F, typename Self>
        static consteval bool s_can_do_transform_error_() {
            using Td = __util::merge_cvref_t<T, Self>;
            using Ed = __util::merge_cvref_t<E, Self>;
            if constexpr (std::invocable<F, Ed>) {
                using Gd = std::invoke_result_t<F, Ed>;
                using G  = std::remove_cvref_t<Gd>;
                if constexpr (__expected::can_be_transform_error_result<G>)
                    return std::constructible_from<T, Td>
                        && std::constructible_from<G, Gd>;
                return false;
            }
            return false;
        }

        /// ::s_transform_
        template <typename U, typename F, typename Self>
        static constexpr auto s_transform_(F&& f, Self&& self) {
            if (self.m_has_val_) {
                if constexpr (std::is_void_v<U>) {
                    gold::invoke_r<void>(std::forward<F>(f), std::forward_like<Self>(self.m_val_));
                    return gold::expected<U, E>();
                } else {
                    return gold::expected<U, E>(std::in_place, gold::invoke(std::forward<F>(f), std::forward_like<Self>(self.m_val_)));
                }
            } else {
                return gold::expected<U, E>(unexpect, std::forward_like<Self>(self.m_unex_));
            }
        }

      public:
        using value_type = T;
        using error_type = E;
        using unexpected_type = unexpected<E>;

        template <typename U>
        using rebind = expected<U, error_type>;

        template <typename G>
        using rebind_error = expected<value_type, G>;

        //// ctors
        constexpr expected() noexcept(std::is_nothrow_default_constructible_v<T>)
            requires std::default_initializable<T>
        : m_val_(), m_has_val_(true) {}

        // provide deleted overload for better diagnostic
        expected() = delete;

        constexpr expected(const expected&)
            requires gold::trivially_copy_constructible<T>
                  && gold::trivially_copy_constructible<E>
        = default;

        constexpr expected(const expected& other)
            noexcept(std::is_nothrow_copy_constructible_v<T>
                  && std::is_nothrow_copy_constructible_v<E>)
            requires std::copy_constructible<T>
                  && std::copy_constructible<E>
        : m_invalid_(), m_has_val_(other.m_has_val_) {
            if (other.m_has_val_)
                gold::construct_at(std::addressof(m_val_), other.m_val_);
            else
                gold::construct_at(std::addressof(m_unex_), other.m_unex_);
        }

        // provide deleted overload for better diagnostic
        expected(const expected&) = delete;

        template <typename U, typename G>
            requires std::constructible_from<T, const U&>
                  && std::constructible_from<E, const G&>
                  && (!__expected::constructible_from_expected<T, E, expected<U, G>>)
        constexpr explicit(__expected::explicit_expected_convertible<T, E, const U&, const G&>)
        expected(const expected<U, G>& other)
            noexcept(std::is_nothrow_constructible_v<T, const U&>
                  && std::is_nothrow_constructible_v<E, const G&>)
        : m_invalid_(), m_has_val_(other.m_has_val_) {
            if (other.m_has_val_)
                gold::construct_at(std::addressof(m_val_), other.m_val_);
            else
                gold::construct_at(std::addressof(m_unex_), other.m_unex_);
        }

        constexpr expected(expected&&)
            requires gold::trivially_move_constructible<T>
                  && gold::trivially_move_constructible<E>
        = default;

        constexpr expected(expected&& other)
            noexcept(std::is_nothrow_move_constructible_v<T>
                  && std::is_nothrow_move_constructible_v<E>)
            requires std::move_constructible<T>
                  && std::move_constructible<E>
        : m_invalid_(), m_has_val_(other.m_has_val_) {
            if (other.m_has_val_)
                gold::construct_at(std::addressof(m_val_), std::move(other.m_val_));
            else
                gold::construct_at(std::addressof(m_unex_), std::move(other.m_unex_));
        }

        // provide deleted overload for better diagnostic
        expected(expected&&) = delete;

        template <typename U, typename G>
            requires std::constructible_from<T, U>
                  && std::constructible_from<E, G>
                  && (!__expected::constructible_from_expected<T, E, expected<U, G>>)
        constexpr explicit(__expected::explicit_expected_convertible<T, E, U, G>)
        expected(expected<U, G>&& other)
            noexcept(std::is_nothrow_constructible_v<T, U>
                  && std::is_nothrow_constructible_v<E, G>)
        : m_invalid_(), m_has_val_(other.m_has_val_) {
            if (other.m_has_val_)
                gold::construct_at(std::addressof(m_val_), std::move(other.m_val_));
            else
                gold::construct_at(std::addressof(m_unex_), std::move(other.m_unex_));
        }

        template <typename... Args>
            requires std::constructible_from<T, Args...>
        constexpr explicit expected(std::in_place_t, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<T, Args...>)
        : m_val_(std::forward<Args>(args)...), m_has_val_(true) {}

        template <typename U, typename... Args>
            requires std::constructible_from<T, std::initializer_list<U>&, Args...>
        constexpr explicit expected(std::in_place_t, std::initializer_list<U> il, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args...>)
        : m_val_(il, std::forward<Args>(args)...), m_has_val_(true) {}

        template <typename... Args>
            requires std::constructible_from<E, Args...>
        constexpr explicit expected(unexpect_t, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<E, Args...>)
        : m_unex_(std::forward<Args>(args)...), m_has_val_(false) {}

        template <typename U, typename... Args>
            requires std::constructible_from<E, std::initializer_list<U>&, Args...>
        constexpr explicit expected(unexpect_t, std::initializer_list<U> il, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<E, std::initializer_list<U>&, Args...>)
        : m_unex_(il, std::forward<Args>(args)...), m_has_val_(false) {}

        template <typename G = E>
            requires std::constructible_from<E, const G&>
        constexpr explicit(!std::is_convertible_v<const G&, E>)
        expected(const unexpected<G>& unex)
            noexcept(std::is_nothrow_constructible_v<E, const G&>)
        : m_unex_(unex.error()), m_has_val_(false) {}

        template <typename G = E>
            requires std::constructible_from<E, G>
        constexpr explicit(!std::is_convertible_v<G, E>)
        expected(unexpected<G>&& unex)
            noexcept(std::is_nothrow_constructible_v<E, G>)
        : m_unex_(std::move(unex).error()), m_has_val_(false) {}

        template <typename U = T>
            requires std::constructible_from<T, U>
        constexpr explicit(!std::is_convertible_v<U, T>)
        expected(U&& val)
            noexcept(std::is_nothrow_constructible_v<T, U>)
        : m_val_(std::forward<U>(val)), m_has_val_(true) {}

        //// dtor
        constexpr ~expected()
            requires gold::trivially_destructible<T>
                  && gold::trivially_destructible<E>
        = default;

        constexpr ~expected() {
            if (m_has_val_)
                gold::destroy_at(std::addressof(m_val_));
            else
                gold::destroy_at(std::addressof(m_unex_));
        }

        //// assignments
        constexpr expected& operator=(const expected& other)
            noexcept(std::is_nothrow_copy_constructible_v<T>
                  && std::is_nothrow_copy_constructible_v<E>
                  && std::is_nothrow_copy_assignable_v<T>
                  && std::is_nothrow_copy_assignable_v<E>)
            requires std::copy_constructible<T>
                  && std::copy_constructible<E>
                  && (std::is_nothrow_move_constructible_v<T>
                   || std::is_nothrow_move_constructible_v<E>)
        {
            if (other.m_has_val_)
                mf_assign_val_(other.m_val_);
            else
                mf_assign_unex_(other.m_unex_);
        }

        // provide deleted overload for better diagnostic
        expected& operator=(const expected&) = delete;

        constexpr expected& operator=(expected&& other)
            noexcept(std::is_nothrow_move_constructible_v<T>
                  && std::is_nothrow_move_constructible_v<E>
                  && std::is_nothrow_move_assignable_v<T>
                  && std::is_nothrow_move_assignable_v<E>)
            requires std::move_constructible<T>
                  && std::move_constructible<E>
                  && (std::is_nothrow_move_constructible_v<T>
                   || std::is_nothrow_move_constructible_v<E>)
        {
            if (other.m_has_val_)
                mf_assign_val_(std::move(other.m_val_));
            else
                mf_assign_unex_(std::move(other.m_unex_));
        }

        // provide deleted overload for better diagnostic
        expected& operator=(expected&&) = delete;

        template <typename G = E>
            requires (std::constructible_from<E, const G&>
                   || std::assignable_from<E&, const G&>)
                  && (std::is_nothrow_constructible_v<E, const G&>
                   || std::is_nothrow_move_constructible_v<T>
                   || std::is_nothrow_move_constructible_v<E>)
        constexpr expected& operator=(const unexpected<G>& unex) {
            mf_assign_unex_(unex.error());
            return *this;
        }

        template <typename G = E>
            requires (std::constructible_from<E, G>
                   || std::assignable_from<E&, G>)
                  && (std::is_nothrow_constructible_v<E, G>
                   || std::is_nothrow_move_constructible_v<T>
                   || std::is_nothrow_move_constructible_v<E>)
        constexpr expected& operator=(unexpected<G>&& unex) {
            mf_assign_unex_(std::move(unex).error());
            return *this;
        }

        template <typename U = T>
            requires (std::constructible_from<T, U>
                   || std::assignable_from<T&, U>)
                  && (std::is_nothrow_constructible_v<T, U>
                   || std::is_nothrow_move_constructible_v<T>
                   || std::is_nothrow_move_constructible_v<E>)
        constexpr expected& operator=(U&& u) {
            mf_assign_val_(std::forward<U>(u));
            return *this;
        }

        //// modifiers
        /// swap
        constexpr void swap(expected& other)
            noexcept(std::is_nothrow_move_constructible_v<T>
                  && std::is_nothrow_move_constructible_v<E>
                  && std::is_nothrow_swappable_v<T&>
                  && std::is_nothrow_swappable_v<E&>)
            requires std::swappable<T>
                  && std::swappable<E>
                  && std::move_constructible<T>
                  && std::move_constructible<E>
                  && (std::is_nothrow_move_constructible_v<T>
                   || std::is_nothrow_move_constructible_v<E>)
        {
            if (m_has_val_ && other.m_has_val_)
                std::ranges::swap(m_val_, other.m_val_);
            else if (m_has_val_)
                mf_swap_val_unex_(other);
            else if (other.m_has_val_)
                other.mf_swap_val_unex_(*this);
            else
                std::ranges::swap(m_unex_, other.m_unex_);
        }

        friend constexpr void swap(expected& lhs, expected& rhs)
            noexcept(noexcept(lhs.swap(rhs)))
            requires requires { lhs.swap(rhs); }
        { lhs.swap(rhs); }

        /// emplace
        template <typename... Args>
            requires std::constructible_from<T, Args...>
        constexpr T& emplace(Args&&... args)
            noexcept(std::is_nothrow_constructible_v<T, Args...>)
        {
            if (m_has_val_) {
                gold::destroy_at(std::addressof(m_val_));
            } else {
                gold::destroy_at(std::addressof(m_unex_));
                m_has_val_ = true;
            }
            return *gold::construct_at(std::addressof(m_val_), std::forward<Args>(args)...);
        }

        template <typename U, typename... Args>
            requires std::constructible_from<T, std::initializer_list<U>&, Args...>
        constexpr T& emplace(std::initializer_list<U> il, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>&, Args...>)
        {
            if (m_has_val_) {
                gold::destroy_at(std::addressof(m_val_));
            } else {
                gold::destroy_at(std::addressof(m_unex_));
                m_has_val_ = true;
            }
            return *gold::construct_at(std::addressof(m_val_), il, std::forward<Args>(args)...);
        }

        /// emplace_error
        template <typename... Args>
            requires std::constructible_from<E, Args...>
        constexpr E& emplace_error(Args&&... args)
            noexcept(std::is_nothrow_constructible_v<E, Args...>)
        {
            if (m_has_val_) {
                gold::destroy_at(std::addressof(m_val_));
                m_has_val_ = false;
            } else {
                gold::destroy_at(std::addressof(m_unex_));
            }
            return *gold::construct_at(std::addressof(m_unex_), std::forward<Args>(args)...);
        }

        template <typename U, typename... Args>
            requires std::constructible_from<E, std::initializer_list<U>&, Args...>
        constexpr E& emplace_error(std::initializer_list<U> il, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<E, std::initializer_list<U>&, Args...>)
        {
            if (m_has_val_) {
                gold::destroy_at(std::addressof(m_val_));
                m_has_val_ = false;
            } else {
                gold::destroy_at(std::addressof(m_unex_));
            }
            return *gold::construct_at(std::addressof(m_unex_), il, std::forward<Args>(args)...);
        }

        //// observers
        /// operator bool
        constexpr explicit operator bool() const noexcept { return m_has_val_; }
        constexpr bool has_value() const noexcept { return m_has_val_; }
        constexpr bool has_error() const noexcept { return !m_has_val_; }

        /// operator->
        constexpr T* operator->() noexcept { return std::addressof(m_val_); }
        constexpr const T* operator->() const noexcept { return std::addressof(m_val_); }

        /// operator*
        template <typename Self>
        constexpr decltype(auto) operator*(this Self&& self) noexcept {
            return std::forward_like<Self>(self.m_val_);
        }

        /// value
        template <typename Self>
        constexpr decltype(auto) value(this Self&& self) {
            if (!self.m_has_val_) [[unlikely]]
                __expected::throw_bad_expected_access(std::forward_like<Self>(self.m_unex_));
            return std::forward_like<Self>(self.m_val_);
        }

        /// error
        template <typename Self>
        constexpr decltype(auto) error(this Self&& self) {
            if (self.m_has_val_) [[unlikely]]
                __expected::throw_bad_unexpected_access();
            return std::forward_like<Self>(self.m_unex_);
        }

        /// value_or
        template <typename U>
            requires std::copy_constructible<T>
                  && std::convertible_to<U, T>
        constexpr T value_or(U&& u) const&
            noexcept(std::is_nothrow_copy_constructible_v<T>
                  && std::is_nothrow_convertible_v<U, T>)
        {
            return m_has_val_ ? m_val_ : static_cast<T>(std::forward<U>(u));
        }

        template <typename U>
            requires std::move_constructible<T>
                  && std::convertible_to<U, T>
        constexpr T value_or(U&& u) &&
            noexcept(std::is_nothrow_move_constructible_v<T>
                  && std::is_nothrow_convertible_v<U, T>)
        {
            return m_has_val_ ? std::move(m_val_) : static_cast<T>(std::forward<U>(u));
        }

        /// value_or_else
        template <typename Self, typename F>
            requires (s_can_do_value_or_else_<F, Self>())
        constexpr T value_or_else(this Self&& self, F&& f) noexcept(s_value_or_else_noexcept_<F, Self>()) {
            using Ed = __util::merge_cvref_t<E, Self>;
            if constexpr (std::is_invocable_v<F, Ed>)
                return self.m_has_val_
                     ? std::forward_like<Self>(self.m_val_)
                     : static_cast<T>(gold::invoke(std::forward<F>(f), std::forward_like<Self>(self.m_unex_)));
            else
                return self.m_has_val_
                     ? std::forward_like<Self>(self.m_val_)
                     : static_cast<T>(gold::invoke(std::forward<F>(f)));
        }

        /// error_or
        template <typename G>
            requires std::copy_constructible<E>
                  && std::convertible_to<G, E>
        constexpr E error_or(G&& g) const&
            noexcept(std::is_nothrow_copy_constructible_v<E>
                  && std::is_nothrow_convertible_v<G, E>)
        {
            return m_has_val_ ? static_cast<E>(std::forward<G>(g)) : m_unex_;
        }

        template <typename G>
            requires std::move_constructible<E>
                  && std::convertible_to<G, E>
        constexpr E error_or(G&& g) &&
            noexcept(std::is_nothrow_move_constructible_v<E>
                  && std::is_nothrow_convertible_v<G, E>)
        {
            return m_has_val_ ? static_cast<E>(std::forward<G>(g)) : std::move(m_unex_);
        }

        /// error_or_else
        template <typename Self, typename F>
            requires (s_can_do_error_or_else_<F, Self>())
        constexpr E error_or_else(this Self&& self, F&& f) noexcept(s_error_or_else_noexcept_<F, Self>()) {
            using Td = __util::merge_cvref_t<T, Self>;
            if constexpr (std::is_invocable_v<F, Td>)
                return !self.m_has_val_
                      ? std::forward_like<Self>(self.m_unex_)
                      : static_cast<E>(gold::invoke(std::forward<F>(f), std::forward_like<Self>(self.m_val_)));
            else
                return !self.m_has_val_
                      ? std::forward_like<Self>(self.m_unex_)
                      : static_cast<E>(gold::invoke(std::forward<F>(f)));
        }

        //// monadic operations
        /// and_then
        template <typename Self, typename F>
            requires (s_can_do_and_then_<F, Self>())
        constexpr auto and_then(this Self&& self, F&& f) {
            using U = std::remove_cvref_t<std::invoke_result_t<F, __util::merge_cvref_t<T, Self>>>;
            if (self.m_has_val_)
                return gold::invoke(std::forward<F>(f), std::forward_like<Self>(self.m_val_));
            else
                return U(unexpect, std::forward_like<Self>(self.m_unex_));
        }

        /// or_else
        template <typename Self, typename F>
            requires (s_can_do_or_else_<F, Self>())
        constexpr auto or_else(this Self&& self, F&& f) {
            return s_or_else_(std::forward<F>(f), std::forward<Self>(self));
        }

        /// transform
        template <typename Self, typename F>
            requires (s_can_do_transform_<F, Self>())
        constexpr auto transform(this Self&& self, F&& f) {
            using U = std::remove_cvref_t<std::invoke_result_t<F, __util::merge_cvref_t<T, Self>>>;
            return s_transform_<U>(std::forward<F>(f), std::forward<Self>(self));
        }

        /// transform_error
        template <typename Self, typename F>
            requires (s_can_do_transform_error_<F, Self>())
        constexpr auto transform_error(this Self&& self, F&& f) {
            using G = std::remove_cvref_t<std::invoke_result_t<F, __util::merge_cvref_t<E, Self>>>;
            if (self.m_has_val_)
                return gold::expected<T, G>(std::in_place, std::forward_like<Self>(self.m_val_));
            else
                return gold::expected<T, G>(unexpect, gold::invoke(std::forward<F>(f), std::forward_like<Self>(self.m_unex_)));
        }

        //// equality operators
        /// operator==
        template <typename U, typename G>
            requires (!std::is_void_v<U>)
        friend constexpr bool operator==(const expected& lhs, const expected<U, G>& rhs)
            noexcept(noexcept(bool(*lhs == *rhs))
                  && noexcept(bool(lhs.m_unex_) == rhs.m_unex_))
        {
            if (lhs.has_value())
                return rhs.has_value() && bool(*lhs == *rhs);
            else
                return !rhs.has_value() && bool(lhs.error() == rhs.error());
        }

        template <typename U>
        friend constexpr bool operator==(const expected& lhs, const U& rhs)
            noexcept(noexcept(bool(*lhs == rhs)))
        { return lhs.has_value() && bool(*lhs == rhs); }

        template <typename G>
        friend constexpr bool operator==(const expected& lhs, const unexpected<G>& rhs)
            noexcept(noexcept(bool(lhs.m_unex_ == rhs.error())))
        { return !lhs.has_value() && bool(lhs.error() == rhs.error()); }

    };

    /// expected<Void>
    template <typename T, typename E>
        requires std::is_void_v<T>
              && __expected::can_be_expected<T, E>
    class expected<T, E> {
      private:
        union {
            __util::empty_t m_void_;
            E m_unex_;
        };
        bool m_has_val_;

        /// ::mf_assign_unex_
        template <typename G>
        constexpr void mf_assign_unex_(G&& g) {
            if (m_has_val_) {
                gold::construct_at(std::addressof(m_unex_), std::forward<G>(g));
                m_has_val_ = false;
            } else {
                if constexpr (std::is_assignable_v<E&, G>) {
                    m_unex_ = std::forward<G>(g);
                } else {
                    gold::destroy_at(std::addressof(m_unex_));
                    gold::construct_at(std::addressof(m_unex_), std::forward<G>(g));
                }
            }
        }

        /// ::s_can_do_and_then_
        template <typename F, typename Self>
        static consteval bool s_can_do_and_then_() {
            if constexpr (std::invocable<F>) {
                using U  = std::remove_cvref_t<std::invoke_result_t<F>>;
                using Ed = __util::merge_cvref_t<E, Self>;
                if constexpr (std::constructible_from<E, Ed>) {
                    if constexpr (__expected::is_expected_v<U>)
                        return std::same_as<typename U::error_type, E>;
                    else
                        return false;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }

        /// ::s_can_do_or_else_
        template <typename F, typename Self>
        static consteval bool s_can_do_or_else_() {
            return __expected::or_else_invocable<
                F,
                __util::merge_cvref_t<T, Self>,
                __util::merge_cvref_t<E, Self>
            >;
        }

        /// ::s_or_else_
        template <typename F, typename Self>
        static constexpr auto s_or_else_(F&& f, Self&& self) {
            using Result = __expected::or_else_result<
                F,
                __util::merge_cvref_t<T, Self>,
                __util::merge_cvref_t<E, Self>
            >;
            using U      = Result::type;
            if constexpr (std::is_void_v<U>) {
                if (!self.m_has_val_) {
                    if constexpr (Result::is_nullary)
                        gold::invoke_r<void>(std::forward<F>(f));
                    else
                        gold::invoke_r<void>(std::forward<F>(f), std::forward_like<Self>(self.m_unex_));
                }
            } else {
                if (self.m_has_val_)
                    return U();
                else {
                    if constexpr (Result::is_nullary)
                        return gold::invoke(std::forward<F>(f));
                    else
                        return gold::invoke(std::forward<F>(f), std::forward_like<Self>(self.m_unex_));
                }
            }
        }

        /// ::s_can_do_transform_
        template <typename F, typename Self>
        static consteval bool s_can_do_transform_() {
            if constexpr (std::invocable<F>) {
                using Ud = std::invoke_result_t<F>;
                using U  = std::remove_cvref_t<Ud>;
                if constexpr (__expected::can_be_transform_result<U>) {
                    using Ed = __util::merge_cvref_t<E, Self>;
                    return std::constructible_from<E, Ed>
                        && (std::is_void_v<U> || std::constructible_from<U, Ud>);
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }

        /// ::s_can_do_transform_error_
        template <typename F, typename Self>
        static consteval bool s_can_do_transform_error_() {
            using Ed = __util::merge_cvref_t<E, Self>;
            if constexpr (std::invocable<F, Ed>) {
                using Gd = std::invoke_result_t<F, Ed>;
                using G  = std::remove_cvref_t<Gd>;
                if constexpr (__expected::can_be_transform_error_result<G>)
                    return std::constructible_from<G, Gd>;
                else
                    return false;
            } else {
                return false;
            }
        }

        /// ::s_transform_
        template <typename U, typename F, typename Self>
        static constexpr auto s_transform_(F&& f, Self&& self) {
            if (self.m_has_val_) {
                if constexpr (std::is_void_v<U>) {
                    gold::invoke_r<void>(std::forward<F>(f));
                    return gold::expected<U, E>();
                } else {
                    return gold::expected<U, E>(std::in_place, gold::invoke(std::forward<F>(f)));
                }
            } else {
                return gold::expected<U, E>(unexpect, std::forward_like<Self>(self.m_unex_));
            }
        }

      public:
        using value_type = T;
        using error_type = E;
        using unexpected_type = unexpected<E>;

        template <typename U>
        using rebind = expected<U, error_type>;

        template <typename G>
        using rebind_error = expected<value_type, G>;

        //// ctors
        constexpr expected() noexcept
        : m_void_(), m_has_val_(true) {}

        constexpr expected(const expected&)
            requires gold::trivially_copy_constructible<E>
        = default;

        constexpr expected(const expected& other)
            noexcept(std::is_nothrow_copy_constructible_v<E>)
            requires std::copy_constructible<E>
        : m_void_(), m_has_val_(other.m_has_val_) {
            if (!other.m_has_val_)
                gold::construct_at(std::addressof(m_unex_), other.m_unex_);
        }

        // deleted overload for better diagnostic
        expected(const expected&) = delete;

        constexpr expected(expected&&)
            requires gold::trivially_move_constructible<E>
        = default;

        constexpr expected(expected&& other)
            noexcept(std::is_nothrow_move_constructible_v<E>)
            requires std::move_constructible<E>
        : m_void_(), m_has_val_(other.m_has_val_) {
            if (!other.m_has_val_)
                gold::construct_at(std::addressof(m_unex_), std::move(other.m_unex_));
        }

        // deleted overload for better diagnostic
        expected(expected&&) = delete;

        template <typename U, typename G>
            requires std::is_void_v<U>
                  && std::constructible_from<E, const G&>
                  && (!__expected::void_constructible_from_expected<U, G>)
        constexpr explicit(!std::is_convertible_v<const G&, E>)
        expected(const expected<U, G>& other)
            noexcept(std::is_nothrow_constructible_v<E, const G&>)
        : m_void_(), m_has_val_(other.m_has_val_) {
            if (!other.m_has_val_)
                gold::construct_at(std::addressof(m_unex_), other.m_unex_);
        }

        template <typename U, typename G>
            requires std::is_void_v<U>
                  && std::constructible_from<E, G>
                  && (!__expected::void_constructible_from_expected<U, G>)
        constexpr explicit(!std::is_convertible_v<G, E>)
        expected(expected<U, G>&& other)
            noexcept(std::is_nothrow_constructible_v<E, G>)
        : m_void_(), m_has_val_(other.m_has_val_) {
            if (!other.m_has_val_)
                gold::construct_at(std::addressof(m_unex_), std::move(other.m_unex_));
        }

        constexpr explicit expected(std::in_place_t) noexcept
        : expected() {}

        template <typename... Args>
            requires std::constructible_from<E, Args...>
        constexpr explicit expected(unexpect_t, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<E, Args...>)
        : m_unex_(std::forward<Args>(args)...), m_has_val_(false) {}

        template <typename U, typename... Args>
            requires std::constructible_from<E, std::initializer_list<U>&, Args...>
        constexpr explicit expected(unexpect_t, std::initializer_list<U> il, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<E, std::initializer_list<U>&, Args...>)
        : m_unex_(il, std::forward<Args>(args)...), m_has_val_(false) {}

        template <typename G = E>
            requires std::constructible_from<E, const G&>
        constexpr explicit(!std::is_convertible_v<const G&, E>)
        expected(const unexpected<G>& unex)
            noexcept(std::is_nothrow_constructible_v<E, const G&>)
        : m_unex_(unex.error()), m_has_val_(false) {}

        template <typename G = E>
            requires std::constructible_from<E, G>
        constexpr explicit(!std::is_convertible_v<G, E>)
        expected(unexpected<G>&& unex)
            noexcept(std::is_nothrow_constructible_v<E, G>)
        : m_unex_(std::move(unex).error()), m_has_val_(false) {}

        /// dtor
        constexpr ~expected()
            requires gold::trivially_destructible<E>
        = default;

        constexpr ~expected() {
            if (!m_has_val_)
                gold::destroy_at(std::addressof(m_unex_));
        }

        //// assignments
        /// operator=
        constexpr expected& operator=(const expected& other)
            noexcept(std::is_nothrow_copy_constructible_v<E>
                  && std::is_nothrow_copy_assignable_v<E>)
            requires std::copy_constructible<E>
        {
            if (other.m_has_val_)
                emplace();
            else
                m_assign_unex_(other.m_unex_);
            return *this;
        }

        constexpr expected& operator=(const expected&) = delete;

        constexpr expected& operator=(expected&& other)
            noexcept(std::is_nothrow_move_constructible_v<E>
                  && std::is_nothrow_move_assignable_v<E>)
            requires std::move_constructible<E>
        {
            if (other.m_has_val_)
                emplace();
            else
                m_assign_unex_(std::move(other.m_unex_));
            return *this;
        }

        constexpr expected& operator=(expected&&) = delete;

        template <typename G>
            requires std::constructible_from<E, const G&>
                  || std::assignable_from<E&, const G&>
        constexpr expected& operator=(const unexpected<G>& e) {
            m_assign_unex_(e.error());
            return *this;
        }

        template <typename G>
            requires std::constructible_from<E, G>
                  || std::assignable_from<E&, G>
        constexpr expected& operator=(unexpected<G>&& e) {
            m_assign_unex_(std::move(e.error()));
            return *this;
        }

        //// modifiers
        /// swap
        constexpr void swap(expected& other)
            noexcept(std::is_nothrow_swappable_v<E>
                  && std::is_nothrow_move_constructible_v<E>)
            requires std::swappable<E>
                  && std::move_constructible<E>
        {
            if (m_has_val_ == other.m_has_val_) {
                if (!m_has_val_)
                    std::ranges::swap(m_unex_, other.m_unex_);
            } else {
                if (m_has_val_) {
                    gold::relocate_at(std::addressof(m_unex_), std::addressof(other.m_unex_));
                    other.m_has_val_ = std::exchange(m_has_val_, false);
                } else {
                    gold::relocate_at(std::addressof(other.m_unex_), std::addressof(m_unex_));
                    m_has_val_ = std::exchange(other.m_has_val_, false);
                }
            }
        }

        friend constexpr void swap(expected& lhs, expected& rhs)
            noexcept(noexcept(lhs.swap(rhs)))
            requires requires { lhs.swap(rhs); }
        { lhs.swap(rhs); }

        /// emplace
        constexpr void emplace() noexcept {
            if (!m_has_val_) {
                gold::destroy_at(std::addressof(m_unex_));
                m_has_val_ = true;
            }
        }

        /// emplace_error
        template <typename... Args>
            requires std::constructible_from<E, Args...>
        constexpr E& emplace_error(Args&&... args)
            noexcept(std::is_nothrow_constructible_v<E, Args...>)
        {
            if (m_has_val_)
                m_has_val_ = false;
            else
                gold::destroy_at(std::addressof(m_unex_));
            return *gold::construct_at(std::addressof(m_unex_), std::forward<Args>(args)...);
        }

        template <typename U, typename... Args>
            requires std::constructible_from<E, std::initializer_list<U>&, Args...>
        constexpr E& emplace_error(std::initializer_list<U> il, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<E, std::initializer_list<U>&, Args...>)
        {
            if (m_has_val_)
                m_has_val_ = false;
            else
                gold::destroy_at(std::addressof(m_unex_));
            return *gold::construct_at(std::addressof(m_unex_), il, std::forward<Args>(args)...);
        }

        //// observers
        /// operator bool
        constexpr explicit operator bool() const noexcept { return m_has_val_; }

        /// has_value
        constexpr bool has_value() const noexcept { return m_has_val_; }

        /// has_error
        constexpr bool has_error() const noexcept { return !m_has_val_; }

        /// operator*
        constexpr void operator*() const noexcept { /* assuming has_value() is true */ }

        /// value
        template <typename Self>
        constexpr void value(this Self&& self) {
            if (!self.m_has_val_) [[unlikely]]
                __expected::throw_bad_expected_access(std::forward_like<Self>(self.m_unex_));
        }

        /// error
        template <typename Self>
        constexpr decltype(auto) error(this Self&& self) {
            if (self.m_has_val_) [[unlikely]]
                __expected::throw_bad_unexpected_access();
            return std::forward_like<Self>(self.m_unex_);
        }

        /// error_or
        template <typename G>
            requires std::copy_constructible<E>
                  && std::convertible_to<G, E>
        constexpr E error_or(G&& g) const&
            noexcept(std::is_nothrow_copy_constructible_v<E>
                  && std::is_nothrow_convertible_v<G, E>)
        {
            return m_has_val_ ? static_cast<E>(std::forward<G>(g)) : m_unex_;
        }

        template <typename G>
            requires std::move_constructible<E>
                  && std::convertible_to<G, E>
        constexpr E error_or(G&& g) &&
            noexcept(std::is_nothrow_move_constructible_v<E>
                  && std::is_nothrow_convertible_v<G, E>)
        {
            return m_has_val_ ? static_cast<E>(std::forward<G>(g)) : std::move(m_unex_);
        }

        /// error_or_else
        template <typename F>
            requires std::copy_constructible<E> &&
                     std::invocable<F> &&
                     std::convertible_to<std::invoke_result_t<F>, E>
        constexpr E error_or_else(F&& f) const&
            noexcept(std::is_nothrow_copy_constructible_v<E>
                  && std::is_nothrow_invocable_v<F>
                  && std::is_nothrow_convertible_v<std::invoke_result_t<F>, E>)
        {
            return m_has_val_ ? static_cast<E>(gold::invoke(std::forward<F>(f))) : m_unex_;
        }

        template <typename F>
            requires std::move_constructible<E> &&
                     std::invocable<F> &&
                     std::convertible_to<std::invoke_result_t<F>, E>
        constexpr E error_or_else(F&& f) &&
            noexcept(std::is_nothrow_move_constructible_v<E>
                  && std::is_nothrow_invocable_v<F>
                  && std::is_nothrow_convertible_v<std::invoke_result_t<F>, E>)
        {
            return m_has_val_ ? static_cast<E>(gold::invoke(std::forward<F>(f))) : std::move(m_unex_);
        }

        //// monadic operations
        /// and_then
        template <typename Self, typename F>
            requires (s_can_do_and_then_<F, Self>())
        constexpr auto and_then(this Self&& self, F&& f) {
            using U = std::remove_cvref_t<std::invoke_result_t<F>>;
            if (self.m_has_val_)
                return gold::invoke(std::forward<F>(f));
            else
                return U(unexpect, std::forward_like<Self>(self.m_unex_));
        }

        /// or_else
        template <typename Self, typename F>
            requires (s_can_do_or_else_<F, Self>())
        constexpr auto or_else(this Self&& self, F&& f) {
            return s_or_else_(std::forward<F>(f), std::forward<Self>(self));
        }

        /// transform
        template <typename Self, typename F>
            requires (s_can_do_transform_<F, Self>())
        constexpr auto transform(this Self&& self, F&& f) {
            using U = std::remove_cvref_t<std::invoke_result_t<F>>;
            return s_transform_<U>(std::forward<F>(f), std::forward<Self>(self));
        }

        /// transform_error
        template <typename Self, typename F>
            requires (s_can_do_transform_error_<F, Self>())
        constexpr auto transform_error(this Self&& self, F&& f) {
            using Ed = __util::merge_cvref_t<E, Self>;
            using G = std::remove_cvref_t<std::invoke_result_t<F, Ed>>;
            if (self.m_has_val_)
                return gold::expected<T, G>();
            else
                return gold::expected<T, G>(unexpect, gold::invoke(std::forward<F>(f), std::forward_like<Self>(self.m_unex_)));
        }

        //// comparisons
        /// operator==
        template <typename U, typename G>
        friend constexpr bool operator==(const expected& lhs, const expected<U, G>& rhs)
            requires requires { { lhs.m_unex_ == rhs.m_unex_ } -> std::convertible_to<bool>; }
        {
            if (lhs.has_value())
                return rhs.has_value();
            else
                return !rhs.has_value() && bool(lhs.error() == rhs.error());
        }

        template <typename G>
        friend constexpr bool operator==(const expected& lhs, const unexpected<G>& rhs)
            noexcept(noexcept(bool(lhs.m_unex_ == rhs.error())))
            requires requires { { lhs.m_unex_ == rhs.error() } -> std::convertible_to<bool>; }
        { return !lhs.has_value() && bool(lhs.error() == rhs.error()); }

    };

    namespace __expected {

        /// __expected::is_expected_v<expected>
        template <typename T, typename E>
        inline constexpr bool is_expected_v<expected<T, E>> = true;

    } // namespace __expected

} // namespace gold

#endif // __GOLD_BITS_EXPECTED_IMPL_HPP
