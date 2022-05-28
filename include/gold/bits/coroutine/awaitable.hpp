// <gold/bits/coroutine/awaitable.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_COROUTINE_AWAITABLE_HPP
#define __GOLD_BITS_COROUTINE_AWAITABLE_HPP

#include <concepts>
#include <coroutine>
#include <gold/bits/type_traits/type_relationships.hpp>
#include <gold/bits/type_traits/specialization_of.hpp>

namespace gold {

    namespace __coro {

        /// __coro::valid_suspend_result
        template <typename T>
        concept valid_suspend_result = gold::is_any_of_v<T, bool, void>
                                    || gold::is_specialization_of_v<T, std::coroutine_handle>();

    } // namespace __coro

    /// simple_awaitable
    template <typename T, typename Promise = void>
    concept simple_awaitable = requires (T& t, const std::coroutine_handle<Promise>& h) {
        { t.await_ready()    } -> std::convertible_to<bool>;
        { t.await_suspend(h) } -> __coro::valid_suspend_result;
        { t.await_resume()   };
    };

    namespace __coro {

        /// __coro::has_member_co_await
        template <typename T, typename Promise = void>
        concept has_member_co_await = requires (T&& t) {
            { std::forward<T>(t).operator co_await() } -> simple_awaitable<Promise>;
        };

        /// __coro::has_adl_co_await
        template <typename T, typename Promise = void>
        concept has_adl_co_await = requires (T&& t) {
            { operator co_await(std::forward<T>(t)) } -> simple_awaitable<Promise>;
        };

    } // namespace __coro

    /// awaitable
    template <typename T, typename Promise = void>
    concept awaitable = __coro::has_member_co_await<T, Promise>
                     || __coro::has_adl_co_await<T, Promise>
                     || simple_awaitable<T, Promise>;

} // namespace gold

#endif // __GOLD_BITS_COROUTINE_AWAITABLE_HPP
