// <gold/bits/coroutine/generator.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once

#ifndef __GOLD_BITS_CORO_GENERATOR_HPP
#define __GOLD_BITS_CORO_GENERATOR_HPP

#include <coroutine>
#include <bits/exception_ptr.h>
#include <gold/bits/assume.hpp>
#include <gold/bits/coroutine/yieldable.hpp>
#include <gold/bits/coroutine/promise_allocator.hpp>
#include <gold/bits/type_traits/specialization_of.hpp>
#include <gold/bits/type_traits/type_transformations.hpp>
#include <utility>

namespace gold {

    namespace __coro {

        /// __coro::gen_value_t
        template <typename Ref, typename Val>
        using gen_value_t = gold::conditional_t<std::is_void_v<Val>, std::remove_cvref_t<Ref>, Val>;

        /// __coro::gen_ref_t
        template <typename Ref, typename Val>
        using gen_ref_t = gold::conditional_t<std::is_void_v<Val>, Ref&&, Ref>;

        /// __coro::gen_rref_t
        template <typename Ref, typename Val, typename U = gen_ref_t<Ref, Val>>
        using gen_rref_t = gold::conditional_t<std::is_lvalue_reference_v<U>, std::remove_reference_t<U>&&, U>;

        /// __coro::gen_yield_t
        template <typename Ref>
        using gen_yield_t = gold::conditional_t<std::is_reference_v<Ref>, Ref, const Ref&>;

        /// __coro::viable_generator_params_impl
        template <typename Ref, typename RRef, typename Value, typename Alloc>
        concept viable_generator_params_impl =
            !gold::is_specialization_of_v<std::remove_cvref_t<Value>, gold::ranges::yieldable_view>()
            && std::same_as<std::remove_cvref_t<Value>, Value>
            && std::is_object_v<Value>
            && (std::is_reference_v<Ref>
                || std::is_object_v<Ref> && std::same_as<std::remove_cv_t<Ref>, Ref> && std::copy_constructible<Ref>)
            && std::common_reference_with<Ref&&, Value&>
            && std::common_reference_with<Ref&&, RRef&&>
            && std::common_reference_with<RRef&&, const Value&>
            && (std::is_void_v<Alloc> || gold::allocator_of<Alloc, std::byte>);

        /// __coro::viable_generator_params
        template <typename R, typename V, typename Alloc>
        concept viable_generator_params = viable_generator_params_impl<
            gen_ref_t<R, V>, gen_rref_t<R, V>, gen_value_t<R, V>, Alloc
        >;

    } // namespace __coro

    /// generator [fwd]
    template <typename R, typename V = void, typename Alloc = void>
        requires __coro::viable_generator_params<R, V, Alloc>
    class generator;

    namespace __coro {

        /// __coro::gen_promise_base
        template <typename Yielded>
            requires std::is_reference_v<Yielded>
        class gen_promise_base {
          private:

            /// __coro::handle_type
            using handle_type = std::coroutine_handle<gen_promise_base>;

            /// __coro::nest_info
            struct nest_info {
                std::exception_ptr m_except_;
                handle_type m_parent_;
                handle_type m_root_;
            };

            template <typename R, typename V, typename Alloc>
            friend class generator<R, V, Alloc>::iterator;

            // m_top_ and m_info_ are mutually exclusive,
            // and could potentially be merged
            handle_type m_top_                 = handle_type::from_promise(*this);
            std::add_pointer_t<Yielded> m_ptr_ = nullptr;
            nest_info* m_info_                 = nullptr;

            /// __coro::gen_promise_base::element_awaiter
            struct element_awaiter {
                std::remove_cvref_t<Yielded> val;

                [[nodiscard]] constexpr bool await_ready() const noexcept { return false; }

                template <typename Promise>
                    requires std::is_pointer_interconvertible_base_of_v<gen_promise_base, Promise>
                constexpr void await_suspend(std::coroutine_handle<Promise> handle) noexcept {
                    gen_promise_base& m_current_ = handle.promise();
                    m_current_.m_ptr_ = std::addressof(val);
                }

                constexpr void await_resume() const noexcept {}
            };

            /// __coro::gen_promise_base::final_awaiter
            struct final_awaiter {
                [[nodiscard]] bool await_ready() noexcept { return false; };

                template <typename Promise>
                    requires std::is_pointer_interconvertible_base_of_v<gen_promise_base, Promise>
                [[nodiscard]] std::coroutine_handle<> await_suspend(std::coroutine_handle<Promise> handle) noexcept {
                    gen_promise_base& m_current_ = handle.promise();
                    if (!m_current_.m_info_)
                        return std::noop_coroutine();

                    handle_type m_cont_ = m_current_.m_info_->m_parent_;
                    m_current_.m_info_->m_root_.promise().m_top_ = m_cont_;
                    m_current_.m_info_ = nullptr;

                    return m_cont_;
                }

                void await_resume() noexcept {}
            };

            /// __coro::nested_awaitable
            template <typename R, typename V, typename Alloc>
                requires std::same_as<gen_yield_t<gen_ref_t<R, V>>, Yielded>
            struct nested_awaitable {
                nest_info              m_nested_;
                generator<R, V, Alloc> m_gen_;

                explicit nested_awaitable(generator<R, V, Alloc>&& gen) noexcept
                : m_gen_(std::move(gen)) {}

                [[nodiscard]] bool await_ready() noexcept { return !m_gen_.m_coro_; }

                template <typename Promise>
                    requires std::is_pointer_interconvertible_base_of_v<gen_promise_base, Promise>
                [[nodiscard]] handle_type await_suspend(std::coroutine_handle<Promise> current) noexcept {
                    auto target = handle_type::from_address(m_gen_.m_coro_.address());
                    m_nested_.m_parent_ = handle_type::from_address(current.address());
                    gen_promise_base& parent_promise = m_nested_.m_parent_.promise();
                    if (parent_promise.m_info_)
                        m_nested_.m_root_ = parent_promise.m_info_->m_root_;
                    else
                        m_nested_.m_root_ = m_nested_.m_parent_;
                    m_nested_.m_root_.promise().m_top_ = target;
                    target.promise().m_info_ = std::addressof(m_nested_);
                    return target;
                }

                void await_resume() {
                    if (m_nested_.m_except_)
                        std::rethrow_exception(std::move(m_nested_.m_except_));
                }
            };

          public:
            std::suspend_always initial_suspend() noexcept { return {}; }
            [[nodiscard]] auto final_suspend() noexcept { return final_awaiter{}; }

            [[nodiscard]] std::suspend_always yield_value(Yielded val) noexcept {
                m_ptr_ = std::addressof(val);
                return {};
            }

            [[nodiscard]] auto yield_value(const std::remove_reference_t<Yielded>& val)
                noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t<Yielded>,
                                                         const std::remove_reference_t<Yielded>&>)
                requires std::is_rvalue_reference_v<Yielded>
                      && std::constructible_from<std::remove_cvref_t<Yielded>,
                                                 const std::remove_reference_t<Yielded>&>
            {
                return element_awaiter { val };
            }

            template <typename R, typename V, typename Alloc, typename Unused>
                requires std::same_as<gen_yield_t<gen_ref_t<R, V>>, Yielded>
            [[nodiscard]] auto yield_value(ranges::yieldable_view<generator<R, V, Alloc>&&, Unused> elem) noexcept {
                return nested_awaitable<R, V, Alloc> { std::move(elem.range) };
            }

            template <std::ranges::input_range R, typename Alloc>
                requires std::convertible_to<std::ranges::range_reference_t<R>, Yielded>
            [[nodiscard]] auto yield_value(ranges::yieldable_view<R, Alloc> elem) noexcept {
                using V = std::ranges::range_value_t<R>;
                auto& range_ = elem.range;
                return nested_awaitable<Yielded, V, Alloc> {
                    [](std::allocator_arg_t, Alloc, std::ranges::iterator_t<R> iter, std::ranges::sentinel_t<R> sent)
                        -> generator<Yielded, V, Alloc> {
                        for (; iter != sent; ++iter)
                            co_yield static_cast<Yielded>(*iter);
                    }(std::allocator_arg, elem.allocator,
                      std::ranges::begin(range_), std::ranges::end(range_))
                };
            }

            void await_transform(auto&&...) = delete;

            void return_void() noexcept {}

            void unhandled_exception() {
                if (m_info_)
                    m_info_->m_except_ = std::current_exception();
                else
                    throw;
            }

        };

        /// __coro::gen_secret_tag
        struct gen_secret_tag {};

    } // namespace __coro

    template <typename R, typename V, typename Alloc>
        requires __coro::viable_generator_params<R, V, Alloc>
    class generator : public std::ranges::view_interface<generator<R, V, Alloc>> {
      public:
        /// generator::value_type
        using value_type   = __coro::gen_value_t<R, V>;

        /// generator::reference
        using reference    = __coro::gen_ref_t<R, V>;

        /// generator::yielded_type
        using yielded_type = __coro::gen_yield_t<reference>;

        /// generator::promise_type
        struct promise_type : __coro::promise_allocator<Alloc>,
                              __coro::gen_promise_base<yielded_type> {

            friend struct iterator;

            [[nodiscard]] generator get_return_object() noexcept {
                return generator { __coro::gen_secret_tag{}, handle_type::from_promise(*this) };
            }

        };

      private:
        friend __coro::gen_promise_base<yielded_type>;

        using handle_type = std::coroutine_handle<promise_type>;

        class iterator {
          private:
            friend generator;

            using handle_type = std::coroutine_handle<__coro::gen_promise_base<yielded_type>>;

            handle_type m_coro_;

            explicit iterator(__coro::gen_secret_tag, handle_type coro) noexcept
            : m_coro_(coro) {}

          public:
            using value_type      = generator::value_type;
            using difference_type = std::ptrdiff_t;

            iterator(iterator&& other) noexcept
            : m_coro_(std::exchange(other.m_coro_, {})) {}

            iterator& operator=(iterator&& other) noexcept {
                if (this != &other)
                    m_coro_ = std::exchange(other.m_coro_, {});
                return *this;
            }

            [[nodiscard]] reference operator*() const noexcept {
                gold::assume(!m_coro_.done());
                return static_cast<reference>(*m_coro_.promise().m_top_.promise().m_ptr_);
            }

            iterator& operator++() {
                gold::assume(!m_coro_.done());
                m_coro_.promise().m_top_.resume();
                return *this;
            }

            void operator++(int) { ++*this; }

            [[nodiscard]] bool operator==(std::default_sentinel_t) const noexcept {
                return m_coro_.done();
            }
        };

        handle_type m_coro_ = nullptr;

        explicit generator(__coro::gen_secret_tag, handle_type coro) noexcept : m_coro_(coro) {}

      public:
        generator() noexcept = default;

        generator(generator&& other) noexcept : m_coro_(std::exchange(other.m_coro_, {})) {}

        ~generator() {
            if (m_coro_)
                m_coro_.destroy();
        }

        generator& operator=(generator other) noexcept {
            swap(other);
            return *this;
        }

        [[nodiscard]] iterator begin() {
            // Pre: m_coro_ is suspended at its initial suspend point
            gold::assume(static_cast<bool>(m_coro_));
            m_coro_.resume();

            return iterator {
                __coro::gen_secret_tag{}, iterator::handle_type::from_address(m_coro_.address())
            };
        }

        [[nodiscard]] constexpr std::default_sentinel_t end() const noexcept {
            return std::default_sentinel;
        }

        void swap(generator& other) noexcept {
            std::ranges::swap(m_coro_, other.m_coro_);
        }

        friend void swap(generator& lhs, generator& rhs) noexcept {
            lhs.swap(rhs);
        }

    };

} // namespace gold

#endif // __GOLD_BITS_CORO_GENERATOR_HPP
