// <gold/bits/functional/function_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FUNCTIONAL_FUNCTION_VIEW_HPP
#define __GOLD_BITS_FUNCTIONAL_FUNCTION_VIEW_HPP

#include <type_traits>
#include <concepts>
#include <utility>
#include <gold/invocable_traits>
#include <gold/bits/memory/voidify.hpp>
#include <gold/bits/memory/ops.hpp>
#include <gold/bits/functional/invoke.hpp>
#include <gold/bits/non_type.hpp>
#include <gold/bits/unreachable.hpp>

namespace gold {

    namespace __functional {

        /// __functional::fn_view_kind
        enum class fn_view_kind : unsigned char {
            stateful,
            stateless,
            none
        };

    } // namespace __functional

    /// function_view
    template <typename...>
    class function_view;

    namespace __functional {

        /// __functional::fn_view_base
        template <typename, bool, bool>
        class fn_view_base;

        template <typename R, typename... Args, bool Const, bool Noexcept>
        class fn_view_base<R(Args...), Const, Noexcept> {
          private:
            using stateless_type = R(*)(Args...) noexcept(Noexcept);
            using stateful_type  = R(*)(void*, Args...) noexcept(Noexcept);

            void* m_state_;
            union {
                stateless_type m_stateless_fn_;
                stateful_type  m_stateful_fn_;
            };
            __functional::fn_view_kind m_kind_;

            constexpr bool mf_is_stateful_() const noexcept {
                return m_kind_ == __functional::fn_view_kind::stateful;
            }

            constexpr bool mf_is_stateless_() const noexcept {
                return m_kind_ == __functional::fn_view_kind::stateless;
            }

            constexpr bool mf_is_none_() const noexcept {
                return m_kind_ == __functional::fn_view_kind::none;
            }

            template <typename... Ts>
            inline static constexpr bool s_is_invocable_using_ = []{
                if constexpr (Noexcept)
                    return std::is_nothrow_invocable_r_v<R, Ts..., Args...>;
                else
                    return std::is_invocable_r_v<R, Ts..., Args...>;
            }();

            template <typename F>
            using maybe_const_t = std::conditional_t<Const, const F, F>;

          public:
            /// default ctor
            constexpr fn_view_base() noexcept
            : m_state_(nullptr), m_kind_(__functional::fn_view_kind::none) {}

            /// copy ctor
            constexpr fn_view_base(const fn_view_base&) noexcept = default;

            /// ctor: taking already-function references
            template <typename F>
                requires std::is_function_v<std::remove_cvref_t<F>> &&
                         s_is_invocable_using_<maybe_const_t<std::remove_cvref_t<F>>&>
            constexpr fn_view_base(F&& f) noexcept
            : m_state_(nullptr), m_stateless_fn_(gold::voidify(f)),
              m_kind_(__functional::fn_view_kind::stateless) {}

            /// ctor: taking already-function pointer
            template <typename F>
                requires std::is_function_v<F> && s_is_invocable_using_<F>
            constexpr fn_view_base(F* f) noexcept
            : m_state_(nullptr), m_stateless_fn_(f),
              m_kind_(__functional::fn_view_kind::stateless) {}

            /// ctor: taking any object convertible to function pointer
            template <typename F>
                requires (!std::is_function_v<std::remove_cvref_t<F>>)
                        && std::is_convertible_v<F, R(*)(Args...) noexcept(Noexcept)>
            constexpr fn_view_base(F&& f) noexcept
            : m_state_(nullptr), m_stateless_fn_(std::forward<F>(f)),
              m_kind_(__functional::fn_view_kind::stateless) {}

            /// ctor: NTTP member function pointer
            template <auto M>
                requires std::is_member_pointer_v<decltype(M)>
                      && s_is_invocable_using_<decltype(M)>
            constexpr fn_view_base(non_type_t<M>) noexcept
            : m_state_(nullptr), m_stateless_fn_([](Args... args) noexcept(Noexcept) -> R {
                return gold::invoke_r<R>(M, std::forward<Args>(args)...);
            }), m_kind_(__functional::fn_view_kind::stateless) {}

            /// ctor: NTTP member pointer with state
            template <auto M, typename T>
                requires std::is_member_pointer_v<decltype(M)>
                      && s_is_invocable_using_<decltype(M), maybe_const_t<T>&>
            constexpr fn_view_base(non_type_t<M>, T& state) noexcept
            : m_state_(gold::voidify(state)),
              m_stateful_fn_([](void* state, Args... args) noexcept(Noexcept) -> R {
                return gold::invoke_r<R>(M,
                    static_cast<maybe_const_t<T>&>(*static_cast<std::add_pointer_t<T>>(state)),
                    std::forward<Args>(args)...
                );
            }), m_kind_(__functional::fn_view_kind::stateful) {}

            /// ctor: NTTP member pointer with state pointer
            template <auto M, typename T>
                requires std::is_member_pointer_v<decltype(M)>
                      && s_is_invocable_using_<decltype(M), maybe_const_t<T>*>
            constexpr fn_view_base(non_type_t<M>, maybe_const_t<T>* state) noexcept
            : m_state_(state),
              m_stateful_fn_([](void* state, Args... args) noexcept(Noexcept) -> R {
                return gold::invoke_r<R>(M,
                    static_cast<std::add_pointer_t<maybe_const_t<T>>>(state),
                    std::forward<Args>(args)...
                );
            }), m_kind_(__functional::fn_view_kind::stateful) {}

            /// ctor: NTTP free function pointer
            template <auto* M>
                requires std::is_function_v<std::remove_pointer_t<decltype(M)>>
            constexpr fn_view_base(non_type_t<M>) noexcept
            : m_state_(nullptr), m_stateless_fn_(M), m_kind_(__functional::fn_view_kind::stateless) {}

            /// ctor: nullptr
            constexpr fn_view_base(std::nullptr_t) noexcept
            : m_state_(nullptr), m_kind_(__functional::fn_view_kind::none) {}

            /// ctor: non-function not convertible to function pointer
            template <typename F>
                requires (!std::is_function_v<std::remove_cvref_t<F>>)
                    && s_is_invocable_using_<maybe_const_t<std::remove_cvref_t<F>>&>
                    && (!std::is_convertible_v<F, R(*)(Args...) noexcept(Noexcept)>)
            constexpr fn_view_base(F&& f) noexcept
            : m_state_(gold::voidify(f)),
              m_stateful_fn_([](void* state, Args... args) noexcept(Noexcept) -> R {
                using U = std::remove_reference_t<F>;
                return gold::invoke_r<R>(
                    static_cast<maybe_const_t<U>&>(*static_cast<std::add_pointer_t<U>>(state)),
                    std::forward<Args>(args)...
                );
            }), m_kind_(__functional::fn_view_kind::stateful) {}

            /// copy assignment
            constexpr fn_view_base& operator=(const fn_view_base&) noexcept = default;

            /// assignment
            template <typename F>
            constexpr fn_view_base& operator=(F&& f) noexcept {
                fn_view_base(std::forward<F>(f)).do_swap(*this);
                return *this;
            }

            /// do_swap
            constexpr void do_swap(fn_view_base& other) noexcept {
                if (this->mf_is_stateful_() && other.mf_is_stateful_()) {
                    std::ranges::swap(m_state_, other.m_state_);
                    std::ranges::swap(m_stateful_fn_, other.m_stateful_fn_);
                } else if (this->mf_is_stateless_() && other.mf_is_stateless_()) {
                    std::ranges::swap(m_stateless_fn_, other.m_stateless_fn_);
                } else if (this->mf_is_none_() && other.mf_is_none_()) {
                } else if (this->mf_is_stateful_() && other.mf_is_stateless_()) {
                    auto this_temp_fn = m_stateful_fn_;
                    gold::destroy_at(&m_stateful_fn_);
                    auto other_temp_fn = other.m_stateless_fn_;
                    gold::destroy_at(&other.m_stateless_fn_);
                    gold::construct_at(&m_stateless_fn_, other_temp_fn);
                    gold::construct_at(&other.m_stateful_fn_, this_temp_fn);
                    other.m_state_ = std::exchange(m_state_, nullptr);
                } else if (this->mf_is_stateless_() && other.mf_is_stateful_()) {
                    auto this_temp_fn = m_stateless_fn_;
                    gold::destroy_at(&m_stateless_fn_);
                    auto other_temp_fn = other.m_stateful_fn_;
                    gold::destroy_at(&other.m_stateful_fn_);
                    gold::construct_at(&m_stateful_fn_, other_temp_fn);
                    gold::construct_at(&other.m_stateless_fn_, this_temp_fn);
                    m_state_ = std::exchange(other.m_state_, nullptr);
                } else if (this->mf_is_stateful_() && other.mf_is_none_()) {
                    auto this_temp_fn = m_stateful_fn_;
                    gold::destroy_at(&m_stateful_fn_);
                    gold::construct_at(&other.m_stateful_fn_, this_temp_fn);
                    other.m_state_ = std::exchange(m_state_, nullptr);
                } else if (this->mf_is_stateless_() && other.mf_is_none_()) {
                    auto this_temp_fn = m_stateless_fn_;
                    gold::destroy_at(&m_stateless_fn_);
                    gold::construct_at(&other.m_stateless_fn_, this_temp_fn);
                } else if (this->mf_is_none_() && other.mf_is_stateful_()) {
                    auto other_temp_fn = other.m_stateful_fn_;
                    gold::destroy_at(&other.m_stateful_fn_);
                    gold::construct_at(&m_stateful_fn_, other_temp_fn);
                    m_state_ = std::exchange(other.m_state_, nullptr);
                } else if (this->mf_is_none_() && other.mf_is_stateless_()) {
                    auto other_temp_fn = other.m_stateless_fn_;
                    gold::destroy_at(&other.m_stateless_fn_);
                    gold::construct_at(&m_stateless_fn_, other_temp_fn);
                }
                std::ranges::swap(m_kind_, other.m_kind_);
            }

            /// do_is_empty
            constexpr bool do_is_empty() const noexcept {
                return mf_is_none_() ||
                      (mf_is_stateless_() ? m_stateless_fn_ == nullptr :
                      (m_state_ == nullptr || m_stateful_fn_ == nullptr));
            }

            /// do_eq
            constexpr bool do_eq(const fn_view_base& other) const noexcept {
                if (m_kind_ != other.m_kind_)
                    return false;
                if (this->mf_is_stateful_() && other.mf_is_stateful_())
                    return m_state_ == other.m_state_ && m_stateful_fn_ == other.m_stateful_fn_;
                else if (this->mf_is_stateless_() && other.mf_is_stateless_())
                    return m_stateless_fn_ == other.m_stateless_fn_;
                return true;
            }

            /// operator()
            constexpr R operator()(Args... args) const noexcept(Noexcept) {

                // precondition, should not be empty, otherwise undefined behaviour
                if (do_is_empty())
                    gold::unreachable();

                switch (m_kind_) {
                    case __functional::fn_view_kind::stateless:
                        return m_stateless_fn_(std::forward<Args>(args)...);
                    case __functional::fn_view_kind::stateful:
                        return m_stateful_fn_(m_state_, std::forward<Args>(args)...);
                    default:;
                }

                gold::unreachable();

            }

        };

        template <typename R, typename... Args, bool Const, bool Noexcept>
        class fn_view_base<R(Args......), Const, Noexcept> {

        };

    } // namespace __functional

    /// function_view
    template <typename F>
        requires std::is_function_v<F>
    class function_view<F>
    : private __functional::fn_view_base<
        gold::remove_cvref_qualifier_noexcept_t<F>,
        gold::has_const_qualifier_v<F>,
        gold::is_noexcept_v<F>
      > {
      private:
        using base_type = __functional::fn_view_base<
            gold::remove_cvref_qualifier_noexcept_t<F>,
            gold::has_const_qualifier_v<F>,
            gold::is_noexcept_v<F>
        >;

      public:
        using base_type::base_type;
        using base_type::operator=;

        constexpr void swap(function_view& other) noexcept {
            base_type::do_swap(other);
        }

        friend constexpr void swap(function_view& lhs, function_view& rhs) noexcept {
            lhs.swap(rhs);
        }

        using base_type::operator();

        constexpr bool operator==(const function_view& other) const noexcept {
            return base_type::do_eq(other);
        }

        constexpr bool operator==(std::nullptr_t) const noexcept {
            return base_type::do_is_empty();
        }

        constexpr explicit operator bool() const noexcept { return base_type::do_is_empty(); }

    };

    /// deduction guides for gold::function_view
    template <typename F>
        requires std::is_function_v<F>
    function_view(F*) -> function_view<F>;

    template <typename F, typename U = std::remove_cvref_t<F>>
        requires (!gold::is_overloaded_v<U>)
    function_view(F&&) -> function_view<
        gold::remove_const_qualifier_t<
            gold::pm_member_type_t<decltype(&U::operator())>
        >
    >;

    template <auto* F>
        requires std::is_function_v<std::remove_pointer_t<decltype(F)>>
    function_view(non_type_t<F>) -> function_view<std::remove_pointer_t<decltype(F)>>;

    template <auto F, typename T = decltype(F)>
        requires std::is_member_pointer_v<T>
    function_view(non_type_t<F>, auto&) -> function_view<
        std::conditional_t<
            std::is_member_function_pointer_v<T>,
            gold::remove_reference_qualifier_t<gold::pm_member_type_t<T>>,
            gold::pm_member_type_t<T>()
        >
    >;

} // namespace gold

#endif // __GOLD_BITS_FUNCTIONAL_FUNCTION_VIEW_HPP
