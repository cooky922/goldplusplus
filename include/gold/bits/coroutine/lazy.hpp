// <gold/bits/coroutine/lazy.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CORO_LAZY_HPP
#define __GOLD_BITS_CORO_LAZY_HPP

#include <utility>
#include <bits/exception_ptr.h>
#include <gold/bits/coroutine/awaitable.hpp>
#include <gold/bits/coroutine/promise_allocator.hpp>
#include <gold/bits/memory/ops.hpp>
#include <gold/bits/type_traits/conditional.hpp>
#include <gold/bits/assume.hpp>
#include <gold/bits/unreachable.hpp>

namespace gold {

    /// lazy [fwd]
    template <typename T = void, typename = void>
        requires std::is_void_v<T>
              || std::is_reference_v<T>
              || (std::is_object_v<T> && std::move_constructible<T>)
    class [[nodiscard]] lazy;

    namespace __coro {

        /// __coro::lazy_final_awaiter
        template <typename Promise>
        struct lazy_final_awaiter {

            [[nodiscard]] bool await_ready() noexcept { return false; }

            template <typename U>
                requires std::is_pointer_interconvertible_base_of_v<Promise, U>
            [[nodiscard]] std::coroutine_handle<> await_suspend(std::coroutine_handle<U> coro) noexcept {
                Promise& current = coro.promise();
                return current.m_cont_ ? current.m_cont_ : std::noop_coroutine();
            }

            void await_resume() noexcept {}

        };

        /// __coro::lazy_promise_state
        enum class lazy_promise_state : unsigned char {
            empty_state, exception_state, data_state
        };

        /// __coro::lazy_promise_base
        template <typename T>
        class lazy_promise_base {
          private:
            template <typename U, typename Alloc>
                requires std::is_void_v<U>
                      || std::is_reference_v<U>
                      || (std::is_object_v<U> && std::move_constructible<U>)
            friend class gold::lazy;

            template <typename>
            friend class lazy_final_awaiter;

            union {
                std::exception_ptr m_except_;
                gold::conditional_t<std::is_reference_v<T>, std::add_pointer_t<T>, T> m_data_;
            };
            lazy_promise_state m_state_ = lazy_promise_state::empty_state;
            std::coroutine_handle<> m_cont_;

            struct awaiter {
                std::coroutine_handle<lazy_promise_base> m_coro_;

                [[nodiscard]] bool await_ready() noexcept { return !m_coro_; }

                [[nodiscard]] std::coroutine_handle<lazy_promise_base>
                await_suspend(std::coroutine_handle<> cont) noexcept {
                    m_coro_.promise().m_cont_ = cont;
                    return m_coro_;
                }

                T await_resume() {
                    auto& m_promise_ = m_coro_.promise();
                    switch (m_promise_.m_state_) {
                        using enum lazy_promise_state;
                        case data_state:
                            if constexpr (std::is_reference_v<T>)
                                return static_cast<T>(*m_promise_.m_data_);
                            else
                                return std::move(m_promise_.m_data_);
                            break;
                        case exception_state:
                            gold::assume(static_cast<bool>(m_promise_.m_except_));
                            std::rethrow_exception(std::move(m_promise_.m_except_));
                            break;
                        default:;
                    }
                    gold::unreachable();
                }

            };

          public:
            lazy_promise_base() noexcept {}

            lazy_promise_base(lazy_promise_base&& other)
                noexcept(std::is_nothrow_move_constructible_v<T>)
            : m_state_(other.m_state_) {
                switch (m_state_) {
                    using enum lazy_promise_state;
                    case empty_state:
                        break;
                    case data_state:
                        gold::relocate_at(std::addressof(other.m_data_), std::addressof(m_data_));
                        break;
                    case exception_state:
                        gold::relocate_at(std::addressof(other.m_except_), std::addressof(m_except_));
                        break;
                }
                other.m_state_ = lazy_promise_state::empty_state;
            }

            ~lazy_promise_base() {
                switch (m_state_) {
                    using enum lazy_promise_state;
                    case empty_state:
                        break;
                    case data_state:
                        gold::destroy_at(std::addressof(m_data_));
                        break;
                    case exception_state:
                        gold::destroy_at(std::addressof(m_except_));
                        break;
                }
            }

            std::suspend_always initial_suspend() noexcept { return {}; }

            [[nodiscard]] auto final_suspend() noexcept { return lazy_final_awaiter<lazy_promise_base>{}; }

            void return_value(T val) noexcept requires std::is_reference_v<T> {
                switch (m_state_) {
                    using enum lazy_promise_state;
                    case empty_state:
                        // m_data_ is a pointer, we can begin its lifetime by assigning
                        [[fallthrough]];
                    case data_state:
                        m_data_ = std::addressof(val);
                        m_state_ = lazy_promise_state::data_state;
                        break;
                    case exception_state:
                        break;
                }
            }

            template <typename U>
                requires (!std::is_reference_v<T>)
                        && std::convertible_to<U, T>
                        && std::constructible_from<T, U>
            void return_value(U&& val) noexcept(std::is_nothrow_constructible_v<U, T>) {
                switch (m_state_) {
                    using enum lazy_promise_state;
                    case data_state:
                        gold::destroy_at(std::addressof(m_data_));
                        [[fallthrough]];
                    case empty_state:
                        gold::construct_at(std::addressof(m_data_), std::forward<U>(val));
                        m_state_ = lazy_promise_state::data_state;
                        break;
                    case exception_state:
                        break;
                }
            }

            void unhandled_exception() {
                switch (m_state_) {
                    using enum lazy_promise_state;
                    case exception_state:
                        break;
                    case data_state:
                        gold::destroy_at(std::addressof(m_data_));
                        [[fallthrough]];
                    case empty_state:
                        gold::construct_at(std::addressof(m_except_), std::current_exception());
                        m_state_ = lazy_promise_state::exception_state;
                        break;
                }
            }

        };

        /// __coro::lazy_promise_base<Void>
        template <typename T>
            requires std::is_void_v<T>
        class lazy_promise_base<T> {
          private:
            template <typename U, typename Alloc>
                requires std::is_void_v<U>
                      || std::is_reference_v<U>
                      || (std::is_object_v<U> && std::move_constructible<U>)
            friend class gold::lazy;

            template <typename>
            friend class lazy_final_awaiter;

            union { std::exception_ptr m_except_; };
            lazy_promise_state m_state_ = lazy_promise_state::empty_state;
            std::coroutine_handle<> m_cont_;

            struct awaiter {
                std::coroutine_handle<lazy_promise_base> m_coro_;

                [[nodiscard]] bool await_ready() noexcept { return !m_coro_; }

                [[nodiscard]] std::coroutine_handle<lazy_promise_base>
                await_suspend(std::coroutine_handle<> cont) noexcept {
                    m_coro_.promise().m_cont_ = cont;
                    return m_coro_;
                }

                void await_resume() {
                    auto& m_promise_ = m_coro_.promise();
                    if (m_promise_.m_state_ == lazy_promise_state::exception_state)
                        std::rethrow_exception(std::move(m_promise_.m_except_));
                }

            };

          public:
            lazy_promise_base() noexcept {}

            lazy_promise_base(lazy_promise_base&& other) noexcept
            : m_state_(other.m_state_) {
                if (m_state_ == lazy_promise_state::exception_state) {
                    gold::relocate_at(std::addressof(other.m_except_), std::addressof(m_except_));
                    other.m_state_ = lazy_promise_state::empty_state;
                }
            }

            ~lazy_promise_base() {
                if (m_state_ == lazy_promise_state::exception_state)
                    gold::destroy_at(std::addressof(m_except_));
            }

            std::suspend_always initial_suspend() noexcept { return {}; }

            [[nodiscard]] auto final_suspend() noexcept { return lazy_final_awaiter<lazy_promise_base>{}; }

            void return_void() noexcept {}

            void unhandled_exception() {
                if (m_state_ == lazy_promise_state::empty_state) {
                    gold::construct_at(std::addressof(m_except_), std::current_exception());
                    m_state_ = lazy_promise_state::exception_state;
                }
            }

        };

    } // namespace __coro

    /// lazy
    template <typename T, typename Alloc>
        requires std::is_void_v<T>
              || std::is_reference_v<T>
              || (std::is_object_v<T> && std::move_constructible<T>)
    class [[nodiscard]] lazy {
      public:
        struct promise_type : __coro::promise_allocator<Alloc>, __coro::lazy_promise_base<T> {
            [[nodiscard]] lazy get_return_object() noexcept {
                return lazy { std::coroutine_handle<promise_type>::from_promise(*this) };
            }
        };

      private:
        friend __coro::lazy_promise_base<T>;

        std::coroutine_handle<promise_type> m_coro_ = nullptr;

        explicit lazy(std::coroutine_handle<promise_type> coro) noexcept
        : m_coro_(coro) {}

      public:
        lazy(lazy&& other) noexcept : m_coro_(std::exchange(other.m_coro_, {})) {}

        ~lazy() {
            if (m_coro_)
                m_coro_.destroy();
        }

        [[nodiscard]] __coro::lazy_promise_base<T>::awaiter operator co_await() {
            // precondition: m_coro_ refers to a coroutine suspended at its initial suspend point
            gold::assume(m_coro_ && !m_coro_.done());
            auto& promise_base = static_cast<__coro::lazy_promise_base<T>&>(m_coro_.promise());
            return typename __coro::lazy_promise_base<T>::awaiter {
                std::coroutine_handle<__coro::lazy_promise_base<T>>::from_promise(promise_base)
            };
        }

        [[nodiscard]] T sync_await() {
            // precondition: m_coro_ refers to a coroutine suspended at its initial suspend point
            gold::assume(m_coro_ && !m_coro_.done());
            gold::simple_awaitable<std::noop_coroutine_handle> auto simple = operator co_await();
            gold::assume(!simple.await_ready());
            simple.await_suspend(std::noop_coroutine()).resume();
            return simple.await_resume();
        }
    };

} // namespace gold

#endif // __GOLD_BITS_CORO_LAZY_HPP
