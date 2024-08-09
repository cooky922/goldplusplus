// <gold/bits/functional/overload.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FUNCTIONAL_OVERLOAD_HPP
#define __GOLD_BITS_FUNCTIONAL_OVERLOAD_HPP

#include <gold/pack>
#include <gold/bits/non_type.hpp>
#include <gold/bits/type_traits/specialization_of.hpp>
#include <gold/bits/type_traits/invoke_traits.hpp>
#include <gold/bits/functional/unref.hpp>

namespace gold {

    namespace __functional {

        /// __functional::maybe_wrap_function
        template <typename F>
            requires std::is_function_v<std::remove_cvref_t<F>>
        constexpr auto maybe_wrap_function(F&& f) {
            using G = std::remove_cvref_t<F>;
            return gold::pack_apply_index<gold::invoke_arity_v<G>>([&]<std::size_t... Is>{
                return [f = std::forward<F>(f)](gold::invoke_parameter_t<G, Is>... args)
                    noexcept(noexcept(f(static_cast<gold::invoke_parameter_t<G, Is>>(args)...)))
                    -> gold::invoke_return_t<G>
                {
                    return f(static_cast<gold::invoke_parameter_t<G, Is>>(args)...);
                };
            });
        }

        template <typename F>
            requires std::is_function_v<F>
        constexpr auto maybe_wrap_function(F* f) {
            return gold::pack_apply_index<gold::invoke_arity_v<F>>([&]<std::size_t... Is>{
                return [f](gold::invoke_parameter_t<F, Is>... args)
                    noexcept(noexcept(f(static_cast<gold::invoke_parameter_t<F, Is>>(args)...)))
                    -> gold::invoke_return_t<F>
                {
                    return f(static_cast<gold::invoke_parameter_t<F, Is>>(args)...);
                };
            });
        }

        template <auto* F>
            requires std::is_pointer_v<decltype(F)> &&
                     std::is_function_v<std::remove_const_t<std::remove_pointer_t<decltype(F)>>>
        constexpr auto maybe_wrap_function(gold::non_type_t<F>) {
            using T = std::remove_const_t<std::remove_pointer_t<decltype(F)>>;
            static constexpr std::size_t N = gold::invoke_arity_v<T>;
            return gold::pack_apply_index<N>([]<std::size_t... Is> {
                return [](gold::invoke_parameter_t<T, Is>... args) static
                    noexcept(noexcept(F(static_cast<gold::invoke_parameter_t<T, Is>>(args)...)))
                -> gold::invoke_return_t<T>
                {
                    return F(static_cast<gold::invoke_parameter_t<T, Is>>(args)...);
                };
            });
        }

        template <typename F>
        constexpr decltype(auto) maybe_wrap_function(F&& f) {
            return std::forward<F>(f);
        }

        /// __functional::overload
        template <typename... Fs>
        struct overload : Fs ... {

            using Fs::operator() ...;

            // automatic reference wrapper unwrapping
            template <typename... Args>
                requires ((gold::is_specialization_of_v<Args, std::reference_wrapper>() || ...))
            constexpr decltype(auto) operator()(Args&&... args) const noexcept {
                return (*this)(gold::unwrap_ref(std::forward<Args>(args))...);
            }

        };

        template <typename... Fs>
        overload(Fs...) -> overload<Fs...>;

    } // namespace __functional

    /// make_overload
    // constraint: no pointer to member is allowed!
    template <typename... Fs>
    constexpr auto make_overload(Fs&&... fs) noexcept {
        return __functional::overload { __functional::maybe_wrap_function(std::forward<Fs>(fs)) ... };
    }

    // deleted overload
    void make_overload() = delete;

} // namespace gold

#endif // __GOLD_BITS_FUNCTIONAL_OVERLOAD_HPP
