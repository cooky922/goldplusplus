// <gold/bits/functional/bind_partial.hpp> - gold++ library

// Copyright (C) [ 2021 - 2025 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FUNCTIONAL_BIND_PARTIAL_HPP
#define __GOLD_BITS_FUNCTIONAL_BIND_PARTIAL_HPP

#include <concepts>
#include <tuple>
#include <gold/bits/functional/invoke.hpp>
#include <gold/bits/__util/uneval.hpp>
#include <gold/bits/__util/like_t.hpp>

namespace gold {

    namespace __functional {

        /// __functional::binder_kind
        enum class binder_kind { front, back, rest };

        /// __functional::partial_nttp_binder_fn
        template <binder_kind Kind, auto F, typename... BoundArgs>
        class partial_nttp_binder_fn {
          private:
            std::tuple<BoundArgs...> m_bound_args_;

            using Indices = std::index_sequence_for<BoundArgs...>;
            using Fd      = const decltype(F)&;

            template <typename Self, typename... CallArgs>
            using invoke_with_result = decltype([]{
                if constexpr (Kind == binder_kind::back || Kind == binder_kind::rest)
                    return __util::uneval<std::invoke_result_t<Fd, CallArgs..., __util::merge_cvref_t<BoundArgs, Self>...>>;
                else
                    return __util::uneval<std::invoke_result_t<Fd, __util::merge_cvref_t<BoundArgs, Self>..., CallArgs...>>;
            }()());

            template <typename Self, typename... CallArgs>
            static consteval bool s_noexcept_() {
                if constexpr (Kind == binder_kind::back || Kind == binder_kind::rest)
                    return std::is_nothrow_invocable_v<Fd, CallArgs..., __util::merge_cvref_t<BoundArgs, Self>...>;
                else
                    return std::is_nothrow_invocable_v<Fd, __util::merge_cvref_t<BoundArgs, Self>..., CallArgs...>;
            }

            template <typename Self, typename... CallArgs>
            inline static constexpr bool s_invocable_with_ = [] {
                if constexpr (Kind == binder_kind::back || Kind == binder_kind::rest)
                    return std::invocable<Fd, CallArgs..., __util::merge_cvref_t<BoundArgs, Self>...>
                        && (Kind == binder_kind::rest ? sizeof...(CallArgs) == 1 : true);
                else
                    return std::invocable<Fd, __util::merge_cvref_t<BoundArgs, Self>..., CallArgs...>;
            }();

            template <typename Self, std::size_t... Is, typename... CallArgs>
            static constexpr decltype(auto) s_call_(Self&& self, std::index_sequence<Is...>, CallArgs&&... call_args) {
                if constexpr (Kind == binder_kind::back || Kind == binder_kind::rest)
                    return gold::invoke(F,
                                        std::forward<CallArgs>(call_args)...,
                                        std::get<Is>(std::forward_like<Self>(self.m_bound_args_))...);
                else
                    return gold::invoke(F,
                                        std::get<Is>(std::forward_like<Self>(self.m_bound_args_))...,
                                        std::forward<CallArgs>(call_args)...);
            }

          public:
            template <typename... Args>
                requires (sizeof...(Args) == sizeof...(BoundArgs))
                      && (std::constructible_from<BoundArgs, Args> && ...)
            constexpr explicit partial_nttp_binder_fn(Args&&... args)
                noexcept(std::conjunction_v<std::is_nothrow_constructible<BoundArgs, Args>...>)
            : m_bound_args_(std::forward<Args>(args)...) {}

            constexpr partial_nttp_binder_fn(const partial_nttp_binder_fn&) = default;
            constexpr partial_nttp_binder_fn(partial_nttp_binder_fn&&) noexcept = default;
            constexpr ~partial_nttp_binder_fn() noexcept = default;
            constexpr partial_nttp_binder_fn& operator=(const partial_nttp_binder_fn&) = default;
            constexpr partial_nttp_binder_fn& operator=(partial_nttp_binder_fn&&) noexcept = default;

            template <typename Self, typename... Args>
                requires s_invocable_with_<Self, Args...>
            constexpr decltype(auto) operator()(this Self&& self, Args&&... args)
                noexcept(s_noexcept_<Self>())
            { return s_call_(std::forward<Self>(self), Indices{}, std::forward<Args>(args)...); }
        };

        /// __functional::partial_binder_fn
        template <binder_kind Kind, typename F, typename... BoundArgs>
        class partial_binder_fn {
          private:
            F m_fn_;
            std::tuple<BoundArgs...> m_bound_args_;

            using Indices = std::index_sequence_for<BoundArgs...>;

            template <typename Self, typename... CallArgs>
            using invoke_with_result = decltype([]{
                using Fd = __util::merge_cvref_t<F, Self>;
                if constexpr (Kind == binder_kind::back || Kind == binder_kind::rest)
                    return __util::uneval<std::invoke_result_t<Fd, CallArgs..., __util::merge_cvref_t<BoundArgs, Self>...>>;
                else
                    return __util::uneval<std::invoke_result_t<Fd, __util::merge_cvref_t<BoundArgs, Self>..., CallArgs...>>;
            }()());

            template <typename Self, typename... CallArgs>
            static consteval bool s_noexcept_() {
                using Fd = __util::merge_cvref_t<F, Self>;
                if constexpr (Kind == binder_kind::back || Kind == binder_kind::rest)
                    return std::is_nothrow_invocable_v<Fd, CallArgs..., __util::merge_cvref_t<BoundArgs, Self>...>;
                else
                    return std::is_nothrow_invocable_v<Fd, __util::merge_cvref_t<BoundArgs, Self>..., CallArgs...>;
            }

            template <typename Self, typename... CallArgs>
            inline static constexpr bool s_invocable_with_ = [] {
                using Fd = __util::merge_cvref_t<F, Self>;
                if constexpr (Kind == binder_kind::back || Kind == binder_kind::rest)
                    return std::invocable<Fd, CallArgs..., __util::merge_cvref_t<BoundArgs, Self>...>
                        && (Kind == binder_kind::rest ? sizeof...(CallArgs) == 1 : true);
                else
                    return std::invocable<Fd, __util::merge_cvref_t<BoundArgs, Self>..., CallArgs...>;
            }();

            template <typename Self, std::size_t... Is, typename... CallArgs>
            static constexpr decltype(auto) s_call_(Self&& self, std::index_sequence<Is...>, CallArgs&&... call_args) {
                if constexpr (Kind == binder_kind::back || Kind == binder_kind::rest)
                    return gold::invoke(std::forward_like<Self>(self.m_fn_),
                                        std::forward<CallArgs>(call_args)...,
                                        std::get<Is>(std::forward_like<Self>(self.m_bound_args_))...);
                else
                    return gold::invoke(std::forward_like<Self>(self.m_fn_),
                                        std::get<Is>(std::forward_like<Self>(self.m_bound_args_))...,
                                        std::forward<CallArgs>(call_args)...);
            }

          public:
            template <typename Fn, typename... Args>
                requires (sizeof...(Args) == sizeof...(BoundArgs))
                      && std::constructible_from<F, Fn>
                      && (std::constructible_from<BoundArgs, Args> && ...)
            constexpr explicit partial_binder_fn(Fn&& fn, Args&&... args)
                noexcept(std::conjunction_v<std::is_nothrow_constructible<F, Fn>,
                                            std::is_nothrow_constructible<BoundArgs, Args>...>)
            : m_fn_(std::forward<Fn>(fn)),
              m_bound_args_(std::forward<Args>(args)...) {}

            constexpr partial_binder_fn(const partial_binder_fn&) = default;
            constexpr partial_binder_fn(partial_binder_fn&&) noexcept = default;
            constexpr ~partial_binder_fn() noexcept = default;
            constexpr partial_binder_fn& operator=(const partial_binder_fn&) = default;
            constexpr partial_binder_fn& operator=(partial_binder_fn&&) noexcept = default;

            template <typename Self, typename... Args>
                requires s_invocable_with_<Self, Args...>
            constexpr decltype(auto) operator()(this Self&& self, Args&&... args)
                noexcept(s_noexcept_<Self, Args...>())
            { return s_call_(std::forward<Self>(self), Indices{}, std::forward<Args>(args)...); }

        };

        /// __functional::front_binder_fn
        template <typename F, typename... BoundArgs>
        using front_binder_fn = partial_binder_fn<binder_kind::front, F, BoundArgs...>;

        /// __functional::back_binder_fn
        template <typename F, typename... BoundArgs>
        using back_binder_fn = partial_binder_fn<binder_kind::back, F, BoundArgs...>;

        /// __functional::rest_binder_fn
        template <typename F, typename... BoundArgs>
        using rest_binder_fn = partial_binder_fn<binder_kind::rest, F, BoundArgs...>;


    } // namespace __functional

    /// bind_front
    template <typename F, typename... Args>
        requires std::move_constructible<std::decay_t<F>>
              && (std::move_constructible<std::decay_t<Args>> && ...)
    constexpr auto bind_front(F&& f, Args&&... args)
        noexcept(std::is_nothrow_move_constructible_v<std::decay_t<F>>
             && (std::is_nothrow_move_constructible_v<std::decay_t<Args>> && ...))
    {
        if constexpr (sizeof...(Args) == 0)
            return std::forward<F>(f);
        else
            return __functional::front_binder_fn<std::decay_t<F>, std::decay_t<Args>...> {
                std::forward<F>(f), std::forward<Args>(args) ...
            };
    }

    template <auto F, typename... Args>
        requires (std::move_constructible<std::decay_t<Args>> && ...)
    constexpr auto bind_front(Args&&... args)
        noexcept((std::is_nothrow_move_constructible_v<std::decay_t<Args>> && ...))
    {
        if constexpr (sizeof...(Args) == 0)
            return F;
        else
            return __functional::partial_nttp_binder_fn<__functional::binder_kind::front, F, std::decay_t<Args>...> {
                std::forward<Args>(args) ...
            };
    }

    /// bind_back
    template <typename F, typename... Args>
        requires std::move_constructible<std::decay_t<F>>
              && (std::move_constructible<std::decay_t<Args>> && ...)
    constexpr auto bind_back(F&& f, Args&&... args)
        noexcept(std::is_nothrow_move_constructible_v<std::decay_t<F>>
             && (std::is_nothrow_move_constructible_v<std::decay_t<Args>> && ...))
    {
        if constexpr (sizeof...(Args) == 0)
            return std::forward<F>(f);
        else
            return __functional::back_binder_fn<std::decay_t<F>, std::decay_t<Args>...> {
                std::forward<F>(f), std::forward<Args>(args) ...
            };
    }

    template <auto F, typename... Args>
        requires (std::move_constructible<std::decay_t<Args>> && ...)
    constexpr auto bind_back(Args&&... args)
        noexcept((std::is_nothrow_move_constructible_v<std::decay_t<Args>> && ...))
    {
        if constexpr (sizeof...(Args) == 0)
            return F;
        else
            return __functional::partial_nttp_binder_fn<__functional::binder_kind::back, F, std::decay_t<Args>...> {
                std::forward<Args>(args) ...
            };
    }

    /// [TODO] move 'bind_rest' here

} // namespace gold

#endif // __GOLD_BITS_FUNCTIONAL_BIND_PARTIAL_HPP
