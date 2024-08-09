// <gold/bits/functional/compose.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FUNCTIONAL_COMPOSE_HPP
#define __GOLD_BITS_FUNCTIONAL_COMPOSE_HPP

#include <gold/bits/functional/invoke.hpp>
#include <gold/bits/__util/like_t.hpp>

namespace gold {

    namespace __functional {

        /// __functinoal::nttp_composer
        template <auto F, auto G>
        class nttp_composer {
          private:
            using F_Type = const __remove_reference(decltype(F))&;
            using G_Type = const __remove_reference(decltype(G))&;

            template <typename... Args>
            static consteval bool s_noexcept_() {
                return std::is_nothrow_invocable_v<G_Type, Args...>
                    && std::is_nothrow_invocable_v<F_Type, std::invoke_result_t<G_Type, Args...>>;
            }

          public:
            template <typename... Args>
                requires std::invocable<G_Type, Args...>
                      && (!std::is_void_v<std::invoke_result_t<G_Type, Args...>>)
                      && std::invocable<F_Type, std::invoke_result_t<G_Type, Args...>>
            static constexpr decltype(auto) operator()(Args&&... args) noexcept(s_noexcept_<Args...>()) {
                return gold::invoke(F, gold::invoke(G, std::forward<Args>(args)...));
            }

        };

        /// __functional::composer
        template <typename F, typename G>
        class composer {
          private:
            [[no_unique_address]] F m_f_ = F();
            [[no_unique_address]] G m_g_ = G();

            template <typename Self, typename... Args>
            static consteval bool s_composable_() {
                using Gu = __util::merge_cvref_t<G, Self>;
                using Fu = __util::merge_cvref_t<F, Self>;
                if constexpr (std::invocable<Gu, Args...>)
                    return (!std::is_void_v<std::invoke_result_t<Gu, Args...>>)
                          && std::invocable<Fu, std::invoke_result_t<Gu, Args...>>;
                else
                    return false;
            }

            template <typename Self, typename... Args>
            static consteval bool s_noexcept_() {
                using Gu = __util::merge_cvref_t<G, Self>;
                using Fu = __util::merge_cvref_t<F, Self>;
                return std::is_nothrow_invocable_v<Gu, Args...>
                    && std::is_nothrow_invocable_v<Fu, std::invoke_result_t<Gu, Args...>>;
            }

          public:
            constexpr composer() noexcept
                requires std::default_initializable<F>
                      && std::default_initializable<G>
            = default;

            constexpr composer(F f, G g)
            : m_f_(std::move(f)), m_g_(std::move(g)) {}

#ifdef __cpp_explicit_this_parameter
            template <typename Self, typename... Args>
                requires (s_composable_<Self, Args...>())
            constexpr decltype(auto) operator()(this Self&& self, Args&&... args) noexcept(s_noexcept_<Self, Args...>()) {
                return gold::invoke(
                    std::forward_like<Self>(self.m_f_),
                    gold::invoke(
                        std::forward_like<Self>(self.m_g_),
                        std::forward<Args>(args)...
                    )
                );
            }
#else
            template <typename... Args>
                requires std::invocable<G&, Args...>
                      && (!std::is_void_v<std::invoke_result_t<G&, Args...>>)
                      && std::invocable<F&, std::invoke_result_t<G&, Args...>>
            constexpr decltype(auto) operator()(Args&&... args) & noexcept(s_noexcept_<composer&, Args...>()) {
                return gold::invoke(m_f_, gold::invoke(m_g_, std::forward<Args>(args)...));
            }

            template <typename... Args>
                requires std::invocable<const G&, Args...>
                      && (!std::is_void_v<std::invoke_result_t<const G&, Args...>>)
                      && std::invocable<const F&, std::invoke_result_t<const G&, Args...>>
            constexpr decltype(auto) operator()(Args&&... args) const& noexcept(s_noexcept_<const composer&, Args...>()) {
                return gold::invoke(m_f_, gold::invoke(m_g_, std::forward<Args>(args)...));
            }

            template <typename... Args>
                requires std::invocable<G, Args...>
                      && (!std::is_void_v<std::invoke_result_t<G, Args...>>)
                      && std::invocable<F, std::invoke_result_t<G, Args...>>
            constexpr decltype(auto) operator()(Args&&... args) && noexcept(s_noexcept_<composer, Args...>()) {
                return gold::invoke(std::move(m_f_), gold::invoke(std::move(m_g_), std::forward<Args>(args)...));
            }

            template <typename... Args>
                requires std::invocable<const G, Args...>
                      && (!std::is_void_v<std::invoke_result_t<const G, Args...>>)
                      && std::invocable<const F, std::invoke_result_t<const G, Args...>>
            constexpr decltype(auto) operator()(Args&&... args) const&& noexcept(s_noexcept_<const composer, Args...>()) {
                return gold::invoke(std::move(m_f_), gold::invoke(std::move(m_g_), std::forward<Args>(args)...));
            }
#endif // C++23
        };

        // TODO: create generalized version of __functional::composer
        //       supporting more than 2 invocables

        /// __functional::compose_impl
        template <typename F, typename... Fs>
        constexpr auto compose_impl(F&& f, Fs&&... fs) {
            if constexpr (sizeof...(Fs) == 0)
                return std::forward<F>(f);
            else if constexpr (sizeof...(Fs) == 1)
                return __functional::composer<std::decay_t<F>, std::decay_t<Fs>...>{
                    std::forward<F>(f), std::forward<Fs>(fs)...
                };
            else
                return __functional::composer<
                    std::decay_t<F>,
                    decltype(__functional::compose_impl(std::forward<Fs>(fs)...))> {
                    std::forward<F>(f), __functional::compose_impl(std::forward<Fs>(fs)...)
                };
        }

        template <auto F, auto... Fs>
        constexpr auto compose_impl() {
            if constexpr (sizeof...(Fs) == 0)
                return F;
            else if constexpr (sizeof...(Fs) == 1)
                return __functional::nttp_composer<F, Fs...> {};
            else
                return __functional::nttp_composer<F, __functional::compose_impl<Fs...>()> {};
        }

    } // namespace __functional

    /// compose
    // supporting 1 or more than number of invocables
    template <typename F, typename... Fs>
        requires std::move_constructible<std::decay_t<F>>
              && (std::move_constructible<std::decay_t<Fs>> && ...)
    constexpr auto compose(F&& f, Fs&&... fs)
        noexcept(std::is_nothrow_move_constructible_v<std::decay_t<F>>
             && (std::is_nothrow_move_constructible_v<std::decay_t<Fs>> && ...))
    {
        return __functional::compose_impl(std::forward<F>(f), std::forward<Fs>(fs)...);
    }

    template <auto F, auto... Fs>
    constexpr auto compose() noexcept {
        return __functional::compose_impl<F, Fs...>();
    }

} // namespace gold

#endif // __GOLD_BITS_FUNCTIONAL_COMPOSE_HPP
