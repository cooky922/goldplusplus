// <gold/bits/functional/overload.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FUNCTIONAL_OVERLOAD_HPP
#define __GOLD_BITS_FUNCTIONAL_OVERLOAD_HPP

#include <utility>
#include <gold/bits/type_traits/specialization_of.hpp>
#include <gold/bits/type_traits/invoke_traits.hpp>
#include <gold/bits/functional/unref.hpp>

namespace gold {

    namespace __functional {

        /// maybe_wrap_function
        template <typename F>
            requires std::is_function_v<std::remove_cvref_t<F>>
        constexpr decltype(auto) maybe_wrap_function(F&& f) {
            return [f = std::forward<F>(f)]<std::size_t... Is>(std::index_sequence<Is...>) {
                return [&f](gold::invoke_parameter_t<std::remove_cvref_t<F>, Is>... args)
                    noexcept(noexcept(f(args...))) -> gold::invoke_return_t<std::remove_cvref_t<F>> {

                    return f(args...);
                };
            }(std::make_index_sequence<gold::invoke_arity_v<std::remove_cvref_t<F>>>{});
        }

        template <typename F>
            requires std::is_function_v<F>
        constexpr decltype(auto) maybe_wrap_function(F* f) {
            return [f]<std::size_t... Is>(std::index_sequence<Is...>) {
                return [f](gold::invoke_parameter_t<F, Is>... args)
                    noexcept(noexcept(f(args...))) -> gold::invoke_return_t<F> {
                    return f(args...);
                };
            }(std::make_index_sequence<gold::invoke_arity_v<F>>{});
        }

        template <typename F>
        constexpr decltype(auto) maybe_wrap_function(F&& f) {
            return std::forward<F>(f);
        }

        template <typename... Fs>
        struct overload : Fs ... {

            using Fs::operator() ...;

            // automatic reference wrapper unwrapping
            template <typename... Args>
                requires ((is_specialization_of_v<Args, std::reference_wrapper>() || ...))
            constexpr decltype(auto) operator()(Args&&... args) const noexcept {
                return (*this)(gold::unref(std::forward<Args>(args))...);
            }

        };

        template <typename... Fs>
        overload(Fs...) -> overload<Fs...>;

    } // namespace __functional

    /// make_overload
    // constraint: no pointer to member is allowed!
    template <typename... Fs>
    constexpr auto make_overload(Fs&&... fs) {
        return __functional::overload { __functional::maybe_wrap_function(std::forward<Fs>(fs)) ... };
    }


} // namespace gold

#endif // __GOLD_BITS_FUNCTIONAL_OVERLOAD_HPP
