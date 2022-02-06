// <gold/bits/coroutine/coro_traits.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_COROUTINE_TRAITS_HPP
#define __GOLD_BITS_COROUTINE_TRAITS_HPP

#include <concepts>
#include <coroutine>

// reference: https://github.com/Quuxplusone/coro/blob/master/include/coro/concepts.h

namespace gold {

    namespace __detail::__coro {

        template <typename>
        inline constexpr bool is_valid_await_suspend_ret_type_v_ = false;

        template <>
        inline constexpr bool is_valid_await_suspend_ret_type_v_<bool> = true;

        template <>
        inline constexpr bool is_valid_await_suspend_ret_type_v_<void> = true;

        template <typename T>
        inline constexpr bool is_valid_await_suspend_ret_type_v_<std::coroutine_handle<T>> = true;

        template <typename T>
        concept valid_await_suspend_ret_type_ = is_valid_await_suspend_ret_type_v_<T>;

    } // namespace __detail::__coro

    /// awaiter
    template <typename T>
    concept awaiter = requires (T&& a, std::coroutine_handle<> h) {
        { a.await_ready()    } -> std::convertible_to<bool>;
        { a.await_suspend(h) } -> __detail::__coro::valid_await_suspend_ret_type_;
        { a.await_resume()   };
    };

    /// awaiter_of
    template <typename T, typename R>
    concept awaiter_of = awaiter<T> && requires (T&& a) {
        { a.await_resume() } -> std::convertible_to<R>;
        requires std::is_void_v<R> || std::constructible_from<R, decltype(a.await_resume())>;
    };

    namespace __detail::__coro {

        // none_such_
        struct none_such_ {};

        // co_await (non_such_)
        void operator co_await(none_such_);

        // priority_tag_
        template <std::size_t I> struct priority_tag_ : priority_tag_<I - 1> {};
        template <> struct priority_tag_<0> {};

        // get_awaiter_ [overload no. 1 - has member co_await]
        template <typename T>
        auto get_awaiter_(T&& t, priority_tag_<2>)
            noexcept(noexcept(static_cast<T&&>(t).operator co_await()))
        -> decltype(static_cast<T&&>(t).operator co_await())
        {
            return static_cast<T&&>(t).operator co_await();
        }

        // get_awaiter_ [overload no. 2 - has free function co_await]
        template <typename T>
        auto get_awaiter_(T&& t, priority_tag_<1>)
            noexcept(noexcept(operator co_await(static_cast<T&&>(t))))
        -> decltype(operator co_await(static_cast<T&&>(t)))
        {
            return operator co_await(static_cast<T&&>(t));
        }

        // get_awaiter_ [overload no. 3 - if only convertible]
        template <typename T>
        decltype(auto) get_awaiter_(T&& t, priority_tag_<0>) noexcept {
            return static_cast<T&&>(t);
        }


    } // namespace __detail::__coro

    /// get_awaiter
    template <typename T>
    decltype(auto) get_awaiter(T&& t)
        noexcept(noexcept(__detail::__coro::get_awaiter_(static_cast<T&&>(t), __detail::__coro::priority_tag_<2>{})))
    {
        return __detail::__coro::get_awaiter_(static_cast<T&&>(t), __detail::__coro::priority_tag_<2>{});
    }

    /// awaiter_type
    template <typename T>
    struct awaiter_type {
        using type = decltype(get_awaiter(std::declval<T>()));
    };

    /// awaiter_type_t
    template <typename T>
    using awaiter_type_t = typename awaiter_type<T>::type;

    /// awaitable
    template <typename T>
    concept awaitable = std::move_constructible<T> && requires (T&& t) {
        { get_awaiter(t) } -> awaiter;
    };

    /// awaitable_of
    template <typename T, typename R>
    concept awaitable_of = awaitable<T> && requires (T&& t) {
        { get_awaiter(t) } -> awaiter_of<R>;
    };

    /// await_result
    template <awaitable T>
    struct await_result {
        using type = decltype(std::declval<awaiter_type_t<T>&>().await_resume());
    };

    /// await_result_t
    template <awaitable T>
    using await_result_t = typename await_result<T>::type;

    /// executor
    template <typename T>
    concept executor = std::copy_constructible<T>
                    && std::is_nothrow_move_constructible<T>
                    && requires (T& t) {
        { t.schedule() } -> awaitable;
    };

}

#endif // __GOLD_BITS_COROUTINE_TRAITS_HPP

