// <gold/bits/functional/try_invoke.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_FUNCTIONAL_TRY_INVOKE_HPP
#define __GOLD_BITS_FUNCTIONAL_TRY_INVOKE_HPP

#include <optional>
#include <bits/invoke.h>
#include <bits/refwrap.h>
#include <gold/bits/void_like.hpp>
#include <gold/bits/__util/is_valid_consteval.hpp>
#include <gold/bits/__util/uneval.hpp>

namespace gold {

    namespace __functional {

        /// __functional::invoke_result_kind
        enum class invoke_result_kind {
            Void,   // void
            Ref,    // l-value reference
            Regular // regular
        };

        /// __functional::select_invoke_result_kind
        template <typename T>
        inline constexpr auto select_invoke_result_kind = []{
            // is void
            if constexpr (__is_same(T, void))
                return invoke_result_kind::Void;
            // is l-value reference
            else if constexpr (__is_same(__remove_reference(T)&, T))
                return invoke_result_kind::Ref;
            // regular type
            else
                return invoke_result_kind::Regular;
        }();

        /// __functional::try_invoke_result
        template <typename F, typename... Args>
        struct try_invoke_result {
          private:
            using result_type = std::invoke_result_t<F, Args...>;
            inline static constexpr auto result_kind = __functional::select_invoke_result_kind<result_type>;

          public:
            using type = decltype([]{
                if constexpr (result_kind == __functional::invoke_result_kind::Void)
                    return __util::uneval<std::optional<gold::void_like>>;
                else if constexpr (result_kind == __functional::invoke_result_kind::Ref)
                    return __util::uneval<std::optional<std::reference_wrapper<__remove_reference(result_type)>>>;
                else
                    return __util::uneval<std::optional<__remove_reference(result_type)>>;
            }()());
        };

        /// __functional::try_invoke_result_t
        template <typename F, typename... Args>
        using try_invoke_result_t = try_invoke_result<F, Args...>::type;

    } // namespace __functional

    /// try_invoke
    template <typename F, typename... Args>
        requires std::invocable<F, Args...>
    constexpr __functional::try_invoke_result_t<F, Args...> try_invoke(F&& f, Args&&... args) noexcept {
        using return_type = __functional::try_invoke_result_t<F, Args...>;
        constexpr bool has_void = __is_same(typename return_type::value_type, gold::void_like);

        return_type result = std::nullopt;
        if constexpr (std::is_nothrow_invocable_v<F, Args...>) {
            if constexpr (has_void) {
                std::__invoke_r<void>(std::forward<F>(f), std::forward<Args>(args)...);
                result.emplace();
            } else {
                result.emplace(std::__invoke(std::forward<F>(f), std::forward<Args>(args)...));
            }
        } else {
            if consteval {
                if constexpr (has_void) {
                    if (gold::__util::is_valid_consteval(std::forward<F>(f), std::forward<Args>(args)...))
                        result.emplace();
                } else {
                    auto fn = [&] { result.emplace(std::__invoke(std::forward<F>(f), std::forward<Args>(args)...)); };
                    if (!gold::__util::is_valid_consteval(fn))
                        result.reset();
                }
            } else {
                try {
                    if constexpr (has_void) {
                        std::__invoke_r<void>(std::forward<F>(f), std::forward<Args>(args)...);
                        result.emplace();
                    } else {
                        result.emplace(std::__invoke(std::forward<F>(f), std::forward<Args>(args)...));
                    }
                } catch (...) {
                    result.reset();
                }
            }
        }
        return result;
    }

} // namespace gold

#endif // __GOLD_BITS_FUNCTIONAL_TRY_INVOKE_HPP
