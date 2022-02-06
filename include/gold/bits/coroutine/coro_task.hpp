// <gold/bits/coroutine/coro_task.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_COROUTINE_TASK_HPP
#define __GOLD_BITS_COROUTINE_TASK_HPP

#include <type_traits>
#include <utility>
#include <functional>

#include <gold/bits/coroutine/coro_base.hpp>

namespace gold {

    /// task
    template <typename T = void>
    class [[nodiscard]] task {
      public:
        /// nested class declaration
        class promise_type;

        /// value_type
        using value_type = T;

        /// handle_type
        using handle_type = std::coroutine_handle<promise_type>;

      private:
        /// m_handle_
        handle_type m_handle_ = nullptr;

        /// friend declaration
        friend promise_type;

        /// explicit constructor
        explicit task(handle_type handle_)
        : m_handle_(handle_) {}

      public:

        /// default constructor
        task() noexcept = default;

        /// user-provided constructor
        task(promise_type& p) noexcept
        : m_handle_(handle_type::from_promise(p)) {}

        /// deleted copy constructor
        task(const task&) = delete;

        /// move constructor
        task(task&& other) noexcept
        : m_handle_(std::exchange(other.m_handle_, nullptr)) {}

        /// destructor
        ~task() { if (m_handle_) m_handle_.destroy(); }

        /// delete copy assignment
        task& operator=(const task&) = delete;

        /// move assignment
        task& operator=(task&& other) noexcept {
            if (this != &other) {
                if (m_handle_) m_handle_.destroy();
                m_handle_ = std::exchange(other.m_handle_, nullptr);
            }
            return *this;
        }

        /// swap
        void swap(task& other) noexcept {
            using std::swap;
            swap(m_handle_, other.m_handle_);
        }

        /// co_await
        auto operator co_await() const {
            class awaitable_ {
              private:
                std::coroutine_handle<promise_type> m_handle_;
              public:
                awaitable_(std::coroutine_handle<promise_type> handle_)
                : m_handle_(handle_) {}
                value_type await_resume() { return m_handle_.promise().result(); }
                bool await_ready() const noexcept { return m_handle_.done(); }
                std::coroutine_handle<> await_suspend(std::coroutine_handle<> parent_handle_) noexcept {
                    m_handle_.promise().set_continuation(parent_handle_);
                    return m_handle_;
                }
            };

            return awaitable_ { m_handle_ };
        }

        /// explicit resume
        void resume() { m_handle_.resume(); }

        /// implicit resume [[unsafe get]]
        value_type operator()() {
            m_handle_.resume();
            return m_handle_.promise().result();
        }

        /// operator bool
        operator bool() { return static_cast<bool>(m_handle_); }

        /// done
        bool is_done() noexcept { return m_handle_.done(); }

        /// not_done
        bool is_not_done() noexcept { return !m_handle_.done(); }

        /// return_value
        value_type get() {
            if (m_handle_.done())
                return m_handle_.promise().result();
            else
                throw coroutine_error("accessing value not yet done...");
        }

    }; // class template task

    /// class definition with reference
    template <typename T>
    class task<T>::promise_type final {
      private:
        /// continuation
        std::coroutine_handle<> m_cont_;
        /// exception pointer
        std::exception_ptr m_except_ = nullptr;
        /// value
        std::conditional_t<
            std::is_lvalue_reference_v<value_type>,
            std::remove_reference_t<value_type>*,
            value_type
        > m_value_;

      public:
        /// get_return_object
        task get_return_object() noexcept {
            return task { std::coroutine_handle<promise_type>::from_promise(*this) };
        }

        /// initial_suspend
        std::suspend_always initial_suspend() const noexcept { return {}; }

        /// final_suspend
        auto final_suspend() const noexcept {
            /// final_awaiter_
            struct final_awaiter_ {
                bool await_ready() noexcept { return false; }
                std::coroutine_handle<> await_suspend(handle_type handle_) noexcept {
                    auto cont_ = handle_.promise().m_cont_;
                    return cont_ ? cont_ : std::noop_coroutine();
                }
                void await_resume() noexcept {}
            };

            return final_awaiter_{};
        }

        /// set_continuation
        void set_continuation(std::coroutine_handle<> cont_) {
            m_cont_ = cont_;
        }

        /// unhandled_exception [[ inherited from promise_base ]]
        void unhandled_exception() { m_except_ = std::current_exception(); }

        /// return_value [[ inherited from promise_base (only if not 'void') ]]
        void return_value(value_type value_) requires (!std::is_lvalue_reference_v<value_type>) {
            m_value_ = std::move(value_);
        }

        void return_value(value_type& value_) requires (std::is_lvalue_reference_v<value_type>) {
            m_value_ = std::addressof(value_);
        }

        /// result
        value_type result() {
            if (m_except_) std::rethrow_exception(m_except_);
            if constexpr (std::is_lvalue_reference_v<value_type>)
                return *m_value_;
            else
                return m_value_;
        }

    };

    /// void specialization for task
    template <>
    class task<void>::promise_type final {
      private:
        /// continuation
        std::coroutine_handle<> m_cont_;
        /// exception pointer
        std::exception_ptr m_except_ = nullptr;

      public:
        /// get_return_object
        task get_return_object() noexcept {
            return task { std::coroutine_handle<promise_type>::from_promise(*this) };
        }

        /// initial_suspend
        std::suspend_always initial_suspend() const noexcept { return {}; }

        /// final_suspend
        auto final_suspend() const noexcept {
            /// final_awaiter_
            struct final_awaiter_ {
                bool await_ready() noexcept { return false; }
                std::coroutine_handle<> await_suspend(handle_type handle_) noexcept {
                    auto cont_ = handle_.promise().m_cont_;
                    return cont_ ? cont_ : std::noop_coroutine();
                }
                void await_resume() noexcept {}
            };

            return final_awaiter_{};
        }

        /// set_continuation
        void set_continuation(std::coroutine_handle<> cont_) {
            m_cont_ = cont_;
        }

        /// unhandled_exception [[ inherited from promise_base ]]
        void unhandled_exception() { m_except_ = std::current_exception(); }

        /// return_void
        void return_void() noexcept {}

        /// result
        void result() {
            if (m_except_) std::rethrow_exception(m_except_);
        }
    };

    /// make_task
    template <typename F>
    task<std::invoke_result_t<F>> make_task(F&& f) noexcept {
        co_return std::invoke(std::forward<F>(f));
    }

} // namespace gold

#endif // __GOLD_BITS_COROUTINE_TASK_HPP
