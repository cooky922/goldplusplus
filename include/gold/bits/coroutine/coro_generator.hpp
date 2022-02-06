// <gold/bits/coroutine/coro_generator.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_COROUTINE_GENERATOR_HPP
#define __GOLD_BITS_COROUTINE_GENERATOR_HPP

#include <utility>
#include <optional>
#include <variant>
#include <bits/refwrap.h>

#include <gold/bits/coroutine/coro_base.hpp>
#include <gold/bits/coroutine/yieldable.hpp>

namespace gold {

    /// next_result_t
    template <typename T>
    struct next_result_t {
        std::optional<T> value;
        bool done;
        operator bool() { return !done; }
    };

    /// generator
    template <std::movable T>
        requires (!__detail::is_yieldable_v_<T>)
    class [[nodiscard]] generator {
        /// note:
        /// the state was all driven by the iterator :>
      public:
        /// forward declarations
        class promise_type;
        class iterator;

        /// value_type alias
        using value_type = std::remove_cvref_t<T>;

        /// handle_type alias
        using handle_type = std::coroutine_handle<promise_type>;

        /// sentinel_type alias
        using sentinel_type = std::default_sentinel_t;

        /// range_type alias
        using range_type = std::ranges::subrange<
            iterator, sentinel_type
        >;

      private:
        /// handle object
        mutable handle_type handle_;

        /// iterator
        mutable iterator current_iter_ = iterator{handle_};

        /// start signal
        mutable bool has_started_;

        /// explicit conversion
        explicit generator(handle_type handle)
        : handle_(std::move(handle)), has_started_(false) {}

        // hidden member functions for exposition only

        /// current_val_
        value_type current_val_();

        /// is_done_
        bool is_done_() const noexcept;

        /// is_not_done_
        bool is_not_done_() const noexcept;

        /// resume_
        void resume_();

        /// next_
        gold::next_result_t<value_type> next_();

        /// iter_begin_
        auto iter_begin_();

      public:
        /// promise_type
        class promise_type {
          private:
            /// friend declarations
            friend iterator;

            /// underlying base
            mutable std::variant<const value_type*, range_type> value_ = nullptr;

          public:
            /// get_return_object
            generator get_return_object();

            /// initial_suspend
            std::suspend_always initial_suspend();

            /// final_suspend
            std::suspend_always final_suspend() noexcept;

            /// unhandled_exception
            [[noreturn]] void unhandled_exception();

            /// deleted await_transform
            template <typename U>
            void await_transform(U&&) = delete;

            /// yield_value [normal type]
            std::suspend_always yield_value(const value_type& value) noexcept {
                value_.template emplace<0>(std::addressof(value));
                return {};
            }

            /// yield_value [normal type, for convertible]
            template <typename U>
                requires (
                   !std::same_as<U, std::remove_cvref_t<value_type>> &&
                    std::convertible_to<U, value_type>
                )
            std::suspend_always yield_value(const U& value) noexcept {
                const static value_type temp_ (value);
                value_.template emplace<0>(std::addressof(temp_));
                return {};
            }

            /// yield_value [yieldable range]
            suspend_maybe yield_value(yieldable<value_type>&& source_) noexcept {
                using type = yieldable<value_type>;
                range_type range_ = std::forward<type>(source_).subrange();
                value_.template emplace<1>(range_);
                return suspend_maybe(range_.empty());
            }

            /// return_void
            void return_void() noexcept;

        }; // class promise_type

        /// default constructor
        generator() noexcept = default;

        /// deleted copy constructor
        generator(const generator&) = delete;

        /// move constructor
        generator(generator&& other) noexcept
        : handle_(std::exchange(other.handle_, {})),
          has_started_(other.has_started_) {}

        /// destructor
        ~generator() { if (handle_) handle_.destroy(); }

        /// deleted copy assignment
        generator& operator=(const generator&) = delete;

        /// move assignment
        generator& operator=(generator&& other) noexcept {
            if (this != &other) {
                if (handle_) handle_.destroy();
                handle_ = std::exchange(other.handle_, {});
                has_started_ = other.has_started_;
            }
            return *this;
        }

        /// swap
        void swap(generator& other) noexcept {
            using std::swap;
            swap(handle_, other.handle_);
            swap(has_started_, other.has_started_);
        }

        /// iterator
        class iterator {
          private:
            friend generator;

            std::reference_wrapper<handle_type> handle_;

            explicit iterator(handle_type& handle) noexcept
            : handle_(handle) {}

          public:
            using iterator_concept = std::input_iterator_tag;
            using value_type       = generator<T>::value_type;
            using difference_type  = std::ptrdiff_t;

            bool operator==(sentinel_type) const noexcept {
                return handle_.get().done();
            }

            iterator& operator++() {
                struct visitor_ {
                    handle_type handle_;
                    void operator()(const value_type*) { handle_.resume(); }
                    void operator()(range_type& r) {
                        if (r.advance(1).empty()) handle_.resume();
                    }
                };
                std::visit(visitor_(handle_.get()), handle_.get().promise().value_);
                return *this;
            }

            iterator operator++(int) {
                auto temp_ = *this;
                ++*this;
                return temp_;
            }

            const value_type* operator->() const noexcept {
                struct visitor_ {
                    const value_type* operator()(const value_type* x)
                    { return x; }
                    const value_type* operator()(range_type& r) {
                        return r.begin().operator->();
                    }
                };
                return std::visit(visitor_{}, handle_.get().promise().value_);
            }

            const value_type& operator*() const noexcept {
                return *operator->();
            }

        }; // class iterator

        /// begin
        iterator begin() noexcept {
            return iter_begin_();
        }

        /// end
        sentinel_type end() const noexcept {
            return std::default_sentinel;
        }

        /// operator*
        auto operator*() {
            return yieldable(std::move(*this));
        }

        /// resume
        void resume() { resume_(); }

        /// operator()
        value_type operator()() {
            return (resume_(), current_val_());
        }

        /// current_value
        auto get() { return current_val_(); }

        /// next
        auto next() { return next_(); }

        /// is_done
        bool is_done() const noexcept { return is_done_(); }

        /// is_not_done
        bool is_not_done() const noexcept { return is_not_done_(); }

    }; // class generator

}

/// implementation for some heavy functions
#include <gold/bits/coroutine/coro_generator.inl>

#endif // __GOLD_BITS_COROUTINE_GENERATOR_HPP
