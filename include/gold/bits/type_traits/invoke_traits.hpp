// <gold/bits/type_traits/invoke_traits.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TYPE_INVOKE_TRAITS_HPP
#define __GOLD_BITS_TYPE_TRAITS_TYPE_INVOKE_TRAITS_HPP

#include <gold/pack>
#include <gold/bits/type_traits/pm_traits.hpp>

namespace gold {

    /// is_noexcept_v
    template <typename T>
    inline constexpr bool is_noexcept_v = false;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args...) noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args......) noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args...) & noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args......) & noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args...) && noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args......) && noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args...) const noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args......) const noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args...) const& noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args......) const& noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args...) const&& noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args......) const&& noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args...) volatile noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args......) volatile noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args...) volatile& noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args......) volatile& noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args...) volatile&& noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args......) volatile&& noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args...) const volatile noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args......) const volatile noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args...) const volatile& noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args......) const volatile& noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args...) const volatile&& noexcept> = true;

    template <typename R, typename... Args>
    inline constexpr bool is_noexcept_v<R(Args......) const volatile&& noexcept> = true;

    /// is_noexcept
    template <typename T>
    using is_noexcept = std::bool_constant<is_noexcept_v<T>>;

    /// remove_noexcept
    template <typename T>
    struct remove_noexcept
    : std::type_identity<T> {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args...) noexcept>
    : std::type_identity<R(Args...)> {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args......) noexcept>
    : std::type_identity<R(Args......)> {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args...) & noexcept>
    : std::type_identity<R(Args...) & > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args......) & noexcept>
    : std::type_identity<R(Args......) & > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args...) && noexcept>
    : std::type_identity<R(Args...) && > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args......) && noexcept>
    : std::type_identity<R(Args......) && > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args...) const noexcept>
    : std::type_identity<R(Args...) const> {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args......) const noexcept>
    : std::type_identity<R(Args......) const> {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args...) const& noexcept>
    : std::type_identity<R(Args...) const& > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args......) const& noexcept>
    : std::type_identity<R(Args......) const& > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args...) const&& noexcept>
    : std::type_identity<R(Args...) const&& > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args......) const&& noexcept>
    : std::type_identity<R(Args......) const&& > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args...) volatile noexcept>
    : std::type_identity<R(Args...) volatile> {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args......) volatile noexcept>
    : std::type_identity<R(Args......) volatile> {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args...) volatile& noexcept>
    : std::type_identity<R(Args...) volatile& > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args......) volatile& noexcept>
    : std::type_identity<R(Args......) volatile& > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args...) volatile&& noexcept>
    : std::type_identity<R(Args...) volatile&& > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args......) volatile&& noexcept>
    : std::type_identity<R(Args......) volatile&& > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args...) const volatile noexcept>
    : std::type_identity<R(Args...) const volatile> {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args......) const volatile noexcept>
    : std::type_identity<R(Args......) const volatile> {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args...) const volatile& noexcept>
    : std::type_identity<R(Args...) const volatile& > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args......) const volatile& noexcept>
    : std::type_identity<R(Args......) const volatile& > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args...) const volatile&& noexcept>
    : std::type_identity<R(Args...) const volatile&& > {};

    template <typename R, typename... Args>
    struct remove_noexcept<R(Args......) const volatile&& noexcept>
    : std::type_identity<R(Args......) const volatile&& > {};

    /// remove_noexcept_t
    template <typename T>
    using remove_noexcept_t = typename remove_noexcept<T>::type;

    /// add_noexcept
    template <typename T>
    struct add_noexcept
    : std::type_identity<T> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args...)>
    : std::type_identity<R(Args...) noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args......)>
    : std::type_identity<R(Args......) noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args...) & >
    : std::type_identity<R(Args...) & noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args......) & >
    : std::type_identity<R(Args......) & noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args...) && >
    : std::type_identity<R(Args...) && noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args......) && >
    : std::type_identity<R(Args......) && noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args...) const>
    : std::type_identity<R(Args...) const noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args......) const>
    : std::type_identity<R(Args......) const noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args...) const& >
    : std::type_identity<R(Args...) const& noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args......) const& >
    : std::type_identity<R(Args......) const& noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args...) const&& >
    : std::type_identity<R(Args...) const&& noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args......) const&& >
    : std::type_identity<R(Args......) const&& noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args...) volatile>
    : std::type_identity<R(Args...) volatile noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args......) volatile>
    : std::type_identity<R(Args......) volatile noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args...) volatile&>
    : std::type_identity<R(Args...) volatile& noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args......) volatile& >
    : std::type_identity<R(Args......) volatile& noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args...) volatile&& >
    : std::type_identity<R(Args...) volatile&& noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args......) volatile&& >
    : std::type_identity<R(Args......) volatile&& noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args...) const volatile>
    : std::type_identity<R(Args...) const volatile noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args......) const volatile>
    : std::type_identity<R(Args......) const volatile noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args...) const volatile& >
    : std::type_identity<R(Args...) const volatile& noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args......) const volatile& >
    : std::type_identity<R(Args......) const volatile& noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args...) const volatile&& >
    : std::type_identity<R(Args...) const volatile&& noexcept> {};

    template <typename R, typename... Args>
    struct add_noexcept<R(Args......) const volatile&& >
    : std::type_identity<R(Args......) const volatile&& noexcept> {};

    /// add_noexcept_t
    template <typename T>
    using add_noexcept_t = typename add_noexcept<T>::type;

    /// has_lvalue_reference_qualifier_v
    template <typename T>
    inline constexpr bool has_lvalue_reference_qualifier_v = false;

    template <typename R, typename... Args>
    inline constexpr bool has_lvalue_reference_qualifier_v<R(Args...) &> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_lvalue_reference_qualifier_v<R(Args......) &> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_lvalue_reference_qualifier_v<R(Args...) const&> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_lvalue_reference_qualifier_v<R(Args......) const&> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_lvalue_reference_qualifier_v<R(Args...) volatile&> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_lvalue_reference_qualifier_v<R(Args......) volatile&> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_lvalue_reference_qualifier_v<R(Args...) const volatile&> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_lvalue_reference_qualifier_v<R(Args......) const volatile&> = true;

    template <typename T>
        requires is_noexcept_v<T>
    inline constexpr bool has_lvalue_reference_qualifier_v<T> = has_lvalue_reference_qualifier_v<remove_noexcept_t<T>>;

    /// has_lvalue_reference_qualifier
    template <typename T>
    using has_lvalue_reference_qualifier = std::bool_constant<has_lvalue_reference_qualifier_v<T>>;

    /// has_rvalue_reference_qualifier_v
    template <typename T>
    inline constexpr bool has_rvalue_reference_qualifier_v = false;

    template <typename R, typename... Args>
    inline constexpr bool has_rvalue_reference_qualifier_v<R(Args...) &&> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_rvalue_reference_qualifier_v<R(Args......) &&> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_rvalue_reference_qualifier_v<R(Args...) const&&> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_rvalue_reference_qualifier_v<R(Args......) const&&> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_rvalue_reference_qualifier_v<R(Args...) volatile&&> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_rvalue_reference_qualifier_v<R(Args......) volatile&&> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_rvalue_reference_qualifier_v<R(Args...) const volatile&&> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_rvalue_reference_qualifier_v<R(Args......) const volatile&&> = true;

    template <typename T>
        requires is_noexcept_v<T>
    inline constexpr bool has_rvalue_reference_qualifier_v<T> = has_rvalue_reference_qualifier_v<remove_noexcept_t<T>>;

    /// has_rvalue_reference_qualifier
    template <typename T>
    using has_rvalue_reference_qualifier = std::bool_constant<has_rvalue_reference_qualifier_v<T>>;

    /// has_reference_qualifier_v
    template <typename T>
    inline constexpr bool has_reference_qualifier_v =
        has_lvalue_reference_qualifier_v<T> ||
        has_rvalue_reference_qualifier_v<T>;

    /// has_reference_qualifier
    template <typename T>
    using has_reference_qualifier = std::bool_constant<has_reference_qualifier_v<T>>;

    /// remove_reference_qualifier
    template <typename T>
    struct remove_reference_qualifier
    : std::type_identity<T> {};

    template <typename T>
        requires is_noexcept_v<T>
    struct remove_reference_qualifier<T>
    : std::type_identity<add_noexcept_t<typename remove_reference_qualifier<remove_noexcept_t<T>>::type>> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args...) & >
    : std::type_identity<R(Args...)> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args......) & >
    : std::type_identity<R(Args......)> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args...) && >
    : std::type_identity<R(Args...)> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args......) && >
    : std::type_identity<R(Args......)> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args...) const& >
    : std::type_identity<R(Args...) const> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args......) const& >
    : std::type_identity<R(Args......) const> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args...) const&& >
    : std::type_identity<R(Args...) const> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args......) const&& >
    : std::type_identity<R(Args......) const> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args...) volatile& >
    : std::type_identity<R(Args...) volatile> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args......) volatile& >
    : std::type_identity<R(Args......) volatile> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args...) volatile&& >
    : std::type_identity<R(Args...) volatile> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args......) volatile&& >
    : std::type_identity<R(Args......) volatile> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args...) const volatile& >
    : std::type_identity<R(Args...) const volatile> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args......) const volatile& >
    : std::type_identity<R(Args......) const volatile> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args...) const volatile&& >
    : std::type_identity<R(Args...) const volatile> {};

    template <typename R, typename... Args>
    struct remove_reference_qualifier<R(Args......) const volatile&& >
    : std::type_identity<R(Args......) const volatile> {};

    /// remove_reference_qualifier_t
    template <typename T>
    using remove_reference_qualifier_t = typename remove_reference_qualifier<T>::type;

    /// add_lvalue_reference_qualifier
    template <typename T>
    struct add_lvalue_reference_qualifier
    : std::type_identity<T> {};

    template <typename T>
        requires is_noexcept_v<T>
    struct add_lvalue_reference_qualifier<T>
    : std::type_identity<add_noexcept_t<typename add_lvalue_reference_qualifier<remove_noexcept_t<T>>::type>> {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args...)>
    : std::type_identity<R(Args...) & > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args......)>
    : std::type_identity<R(Args......) & > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args...) && >
    : std::type_identity<R(Args...) & > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args......) && >
    : std::type_identity<R(Args......) & > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args...) const>
    : std::type_identity<R(Args...) const& > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args......) const>
    : std::type_identity<R(Args......) const& > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args...) const&& >
    : std::type_identity<R(Args...) const& > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args......) const&& >
    : std::type_identity<R(Args......) const& > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args...) volatile>
    : std::type_identity<R(Args...) volatile& > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args......) volatile>
    : std::type_identity<R(Args......) volatile& > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args...) volatile&& >
    : std::type_identity<R(Args...) volatile& > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args......) volatile&& >
    : std::type_identity<R(Args......) volatile& > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args...) const volatile>
    : std::type_identity<R(Args...) const volatile& > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args......) const volatile>
    : std::type_identity<R(Args......) const volatile& > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args...) const volatile&& >
    : std::type_identity<R(Args...) const volatile& > {};

    template <typename R, typename... Args>
    struct add_lvalue_reference_qualifier<R(Args......) const volatile&& >
    : std::type_identity<R(Args......) const volatile& > {};

    /// add_lvalue_reference_qualifier_t
    template <typename T>
    using add_lvalue_reference_qualifier_t = typename add_lvalue_reference_qualifier<T>::type;

    /// add_rvalue_reference_qualifier
    template <typename T>
    struct add_rvalue_reference_qualifier
    : std::type_identity<T> {};

    template <typename T>
        requires is_noexcept_v<T>
    struct add_rvalue_reference_qualifier<T>
    : std::type_identity<add_noexcept_t<typename add_rvalue_reference_qualifier<remove_noexcept_t<T>>::type>> {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args...)>
    : std::type_identity<R(Args...) && > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args......)>
    : std::type_identity<R(Args......) && > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args...) & >
    : std::type_identity<R(Args...) & > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args......) & >
    : std::type_identity<R(Args......) & > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args...) const>
    : std::type_identity<R(Args...) const&& > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args......) const>
    : std::type_identity<R(Args......) const&& > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args...) const& >
    : std::type_identity<R(Args...) const& > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args......) const& >
    : std::type_identity<R(Args......) const& > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args...) volatile>
    : std::type_identity<R(Args...) volatile&& > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args......) volatile>
    : std::type_identity<R(Args......) volatile&& > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args...) volatile& >
    : std::type_identity<R(Args...) volatile& > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args......) volatile& >
    : std::type_identity<R(Args......) volatile& > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args...) const volatile>
    : std::type_identity<R(Args...) const volatile&& > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args......) const volatile>
    : std::type_identity<R(Args......) const volatile&& > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args...) const volatile& >
    : std::type_identity<R(Args...) const volatile& > {};

    template <typename R, typename... Args>
    struct add_rvalue_reference_qualifier<R(Args......) const volatile& >
    : std::type_identity<R(Args......) const volatile& > {};

    /// add_rvalue_reference_qualifier_t
    template <typename T>
    using add_rvalue_reference_qualifier_t = typename add_rvalue_reference_qualifier<T>::type;

    /// has_const_qualifier_v
    template <typename T>
    inline constexpr bool has_const_qualifier_v = false;

    template <typename T>
        requires (is_noexcept_v<T> && has_reference_qualifier_v<T>)
    inline constexpr bool has_const_qualifier_v<T> = has_const_qualifier_v<
        remove_reference_qualifier_t<remove_noexcept_t<T>>
    >;

    template <typename T>
        requires (is_noexcept_v<T> && !has_reference_qualifier_v<T>)
    inline constexpr bool has_const_qualifier_v<T> = has_const_qualifier_v<remove_noexcept_t<T>>;

    template <typename T>
        requires (!is_noexcept_v<T> && has_reference_qualifier_v<T>)
    inline constexpr bool has_const_qualifier_v<T> = has_const_qualifier_v<remove_reference_qualifier_t<T>>;

    template <typename R, typename... Args>
    inline constexpr bool has_const_qualifier_v<R(Args...) const> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_const_qualifier_v<R(Args......) const> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_const_qualifier_v<R(Args...) const volatile> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_const_qualifier_v<R(Args......) const volatile> = true;

    /// has_const_qualifier
    template <typename T>
    using has_const_qualifier = std::bool_constant<has_const_qualifier_v<T>>;

    /// has_volatile_qualifier_v
    template <typename T>
    inline constexpr bool has_volatile_qualifier_v = false;

    template <typename T>
        requires (is_noexcept_v<T> && has_reference_qualifier_v<T>)
    inline constexpr bool has_volatile_qualifier_v<T> = has_volatile_qualifier_v<
        remove_reference_qualifier_t<remove_noexcept_t<T>>
    >;

    template <typename T>
        requires (is_noexcept_v<T> && !has_reference_qualifier_v<T>)
    inline constexpr bool has_volatile_qualifier_v<T> = has_volatile_qualifier_v<remove_noexcept_t<T>>;

    template <typename T>
        requires (!is_noexcept_v<T> && has_reference_qualifier_v<T>)
    inline constexpr bool has_volatile_qualifier_v<T> = has_volatile_qualifier_v<remove_reference_qualifier_t<T>>;

    template <typename R, typename... Args>
    inline constexpr bool has_volatile_qualifier_v<R(Args...) volatile> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_volatile_qualifier_v<R(Args......) volatile> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_volatile_qualifier_v<R(Args...) const volatile> = true;

    template <typename R, typename... Args>
    inline constexpr bool has_volatile_qualifier_v<R(Args......) const volatile> = true;

    /// has_const_qualifier
    template <typename T>
    using has_const_qualifier = std::bool_constant<has_const_qualifier_v<T>>;

    /// has_cv_qualifier_v
    template <typename T>
    inline constexpr bool has_cv_qualifier_v = has_const_qualifier_v<T> || has_volatile_qualifier_v<T>;

    /// has_cv_qualifier
    template <typename T>
    using has_cv_qualifier = std::bool_constant<has_cv_qualifier_v<T>>;

    /// has_cvref_qualifier_v
    template <typename T>
    inline constexpr bool has_cvref_qualifier_v = has_cv_qualifier_v<T> || has_reference_qualifier_v<T>;

    /// has_cvref_qualifier
    template <typename T>
    using has_cvref_qualifier = std::bool_constant<has_cvref_qualifier_v<T>>;

    /// has_cvref_qualifier_or_noexcept_v
    template <typename T>
    inline constexpr bool has_cvref_qualifier_or_noexcept_v = has_cvref_qualifier_v<T> || is_noexcept_v<T>;

    /// has_cvref_qualifier_or_noexcept
    template <typename T>
    using has_cvref_qualifier_or_noexcept = std::bool_constant<has_cvref_qualifier_or_noexcept_v<T>>;

    /// remove_const_qualifier
    template <typename T>
    struct remove_const_qualifier
    : std::type_identity<T> {};

    template <typename T>
        requires (is_noexcept_v<T> && has_lvalue_reference_qualifier_v<T>)
    struct remove_const_qualifier<T>
    : std::type_identity<
        add_noexcept_t<
            add_lvalue_reference_qualifier_t<
                typename remove_const_qualifier<
                    remove_reference_qualifier_t<
                        remove_noexcept_t<T>
                    >
                >::type
            >
        >
    > {};

    template <typename T>
        requires (is_noexcept_v<T> && has_rvalue_reference_qualifier_v<T>)
    struct remove_const_qualifier<T>
    : std::type_identity<
        add_noexcept_t<
            add_rvalue_reference_qualifier_t<
                typename remove_const_qualifier<
                    remove_reference_qualifier_t<
                        remove_noexcept_t<T>
                    >
                >::type
            >
        >
    > {};

    template <typename T>
        requires (is_noexcept_v<T> && !has_reference_qualifier_v<T>)
    struct remove_const_qualifier<T>
    : std::type_identity<
        add_noexcept_t<
            typename remove_const_qualifier<
                remove_noexcept_t<T>
            >::type
        >
    > {};

    template <typename T>
        requires (!is_noexcept_v<T> && has_lvalue_reference_qualifier_v<T>)
    struct remove_const_qualifier<T>
    : std::type_identity<
        add_lvalue_reference_qualifier_t<
            typename remove_const_qualifier<
                remove_reference_qualifier_t<T>
            >::type
        >
    > {};

    template <typename T>
        requires (!is_noexcept_v<T> && has_rvalue_reference_qualifier_v<T>)
    struct remove_const_qualifier<T>
    : std::type_identity<
        add_rvalue_reference_qualifier_t<
            typename remove_const_qualifier<
                remove_reference_qualifier_t<T>
            >::type
        >
    > {};

    template <typename R, typename... Args>
    struct remove_const_qualifier<R(Args...) const>
    : std::type_identity<R(Args...)> {};

    template <typename R, typename... Args>
    struct remove_const_qualifier<R(Args......) const>
    : std::type_identity<R(Args......)> {};

    template <typename R, typename... Args>
    struct remove_const_qualifier<R(Args...) const volatile>
    : std::type_identity<R(Args...) volatile> {};

    template <typename R, typename... Args>
    struct remove_const_qualifier<R(Args......) const volatile>
    : std::type_identity<R(Args......) volatile> {};

    /// remove_const_qualifier_t
    template <typename T>
    using remove_const_qualifier_t = typename remove_const_qualifier<T>::type;

    /// remove_volatile_qualifier
    template <typename T>
    struct remove_volatile_qualifier
    : std::type_identity<T> {};

    template <typename T>
        requires (is_noexcept_v<T> && has_lvalue_reference_qualifier_v<T>)
    struct remove_volatile_qualifier<T>
    : std::type_identity<
        add_noexcept_t<
            add_lvalue_reference_qualifier_t<
                typename remove_volatile_qualifier<
                    remove_reference_qualifier_t<
                        remove_noexcept_t<T>
                    >
                >::type
            >
        >
    > {};

    template <typename T>
        requires (is_noexcept_v<T> && has_rvalue_reference_qualifier_v<T>)
    struct remove_volatile_qualifier<T>
    : std::type_identity<
        add_noexcept_t<
            add_rvalue_reference_qualifier_t<
                typename remove_volatile_qualifier<
                    remove_reference_qualifier_t<
                        remove_noexcept_t<T>
                    >
                >::type
            >
        >
    > {};

    template <typename T>
        requires (is_noexcept_v<T> && !has_reference_qualifier_v<T>)
    struct remove_volatile_qualifier<T>
    : std::type_identity<
        add_noexcept_t<
            typename remove_volatile_qualifier<
                remove_noexcept_t<T>
            >::type
        >
    > {};

    template <typename T>
        requires (!is_noexcept_v<T> && has_lvalue_reference_qualifier_v<T>)
    struct remove_volatile_qualifier<T>
    : std::type_identity<
        add_lvalue_reference_qualifier_t<
            typename remove_volatile_qualifier<
                remove_reference_qualifier_t<T>
            >::type
        >
    > {};

    template <typename T>
        requires (!is_noexcept_v<T> && has_rvalue_reference_qualifier_v<T>)
    struct remove_volatile_qualifier<T>
    : std::type_identity<
        add_rvalue_reference_qualifier_t<
            typename remove_volatile_qualifier<
                remove_reference_qualifier_t<T>
            >::type
        >
    > {};

    template <typename R, typename... Args>
    struct remove_volatile_qualifier<R(Args...) volatile>
    : std::type_identity<R(Args...)> {};

    template <typename R, typename... Args>
    struct remove_volatile_qualifier<R(Args......) volatile>
    : std::type_identity<R(Args......)> {};

    template <typename R, typename... Args>
    struct remove_volatile_qualifier<R(Args...) const volatile>
    : std::type_identity<R(Args...) const> {};

    template <typename R, typename... Args>
    struct remove_volatile_qualifier<R(Args......) const volatile>
    : std::type_identity<R(Args......) const> {};

    /// remove_volatile_qualifier_t
    template <typename T>
    using remove_volatile_qualifier_t = typename remove_volatile_qualifier<T>::type;

    /// remove_cv_qualifier_t
    template <typename T>
    using remove_cv_qualifier_t = remove_const_qualifier_t<remove_volatile_qualifier_t<T>>;

    /// remove_cv_qualifier
    template <typename T>
    using remove_cv_qualifier = std::type_identity<remove_cv_qualifier_t<T>>;

    /// remove_cvref_qualifier_t
    template <typename T>
    using remove_cvref_qualifier_t = remove_cv_qualifier_t<remove_reference_qualifier_t<T>>;

    /// remove_cvref_qualifier
    template <typename T>
    using remove_cvref_qualifier = std::type_identity<remove_cvref_qualifier_t<T>>;

    /// remove_cvref_qualifier_noexcept_t
    template <typename T>
    using remove_cvref_qualifier_noexcept_t = remove_cvref_qualifier_t<remove_noexcept_t<T>>;

    /// remove_cvref_qualifier_noexcept
    template <typename T>
    using remove_cvref_qualifier_noexcept = std::type_identity<remove_cvref_qualifier_noexcept_t<T>>;

    /// add_const_qualifier
    template <typename T>
    struct add_const_qualifier
    : std::type_identity<T> {};

    template <typename T>
        requires (is_noexcept_v<T> && has_lvalue_reference_qualifier_v<T>)
    struct add_const_qualifier<T>
    : std::type_identity<
        add_noexcept_t<
            add_lvalue_reference_qualifier_t<
                typename add_const_qualifier<
                    remove_reference_qualifier_t<
                        remove_noexcept_t<T>
                    >
                >::type
            >
        >
    > {};

    template <typename T>
        requires (is_noexcept_v<T> && has_rvalue_reference_qualifier_v<T>)
    struct add_const_qualifier<T>
    : std::type_identity<
        add_noexcept_t<
            add_rvalue_reference_qualifier_t<
                typename add_const_qualifier<
                    remove_reference_qualifier_t<
                        remove_noexcept_t<T>
                    >
                >::type
            >
        >
    > {};

    template <typename T>
        requires (is_noexcept_v<T> && !has_reference_qualifier_v<T>)
    struct add_const_qualifier<T>
    : std::type_identity<
        add_noexcept_t<
            typename add_const_qualifier<
                remove_noexcept_t<T>
            >::type
        >
    > {};

    template <typename T>
        requires (!is_noexcept_v<T> && has_lvalue_reference_qualifier_v<T>)
    struct add_const_qualifier<T>
    : std::type_identity<
        add_lvalue_reference_qualifier_t<
            typename add_const_qualifier<
                remove_reference_qualifier_t<T>
            >::type
        >
    > {};

    template <typename T>
        requires (!is_noexcept_v<T> && has_rvalue_reference_qualifier_v<T>)
    struct add_const_qualifier<T>
    : std::type_identity<
        add_rvalue_reference_qualifier_t<
            typename add_const_qualifier<
                remove_reference_qualifier_t<T>
            >::type
        >
    > {};

    template <typename R, typename... Args>
    struct add_const_qualifier<R(Args...)>
    : std::type_identity<R(Args...) const> {};

    template <typename R, typename... Args>
    struct add_const_qualifier<R(Args......)>
    : std::type_identity<R(Args......) const> {};

    template <typename R, typename... Args>
    struct add_const_qualifier<R(Args...) volatile>
    : std::type_identity<R(Args...) const volatile> {};

    template <typename R, typename... Args>
    struct add_const_qualifier<R(Args......) volatile>
    : std::type_identity<R(Args......) const volatile> {};

    /// add_const_qualifier_t
    template <typename T>
    using add_const_qualifier_t = typename add_const_qualifier<T>::type;

    /// add_volatile_qualifier
    template <typename T>
    struct add_volatile_qualifier
    : std::type_identity<T> {};

    template <typename T>
        requires (is_noexcept_v<T> && has_lvalue_reference_qualifier_v<T>)
    struct add_volatile_qualifier<T>
    : std::type_identity<
        add_noexcept_t<
            add_lvalue_reference_qualifier_t<
                typename add_volatile_qualifier<
                    remove_reference_qualifier_t<
                        remove_noexcept_t<T>
                    >
                >::type
            >
        >
    > {};

    template <typename T>
        requires (is_noexcept_v<T> && has_rvalue_reference_qualifier_v<T>)
    struct add_volatile_qualifier<T>
    : std::type_identity<
        add_noexcept_t<
            add_rvalue_reference_qualifier_t<
                typename add_volatile_qualifier<
                    remove_reference_qualifier_t<
                        remove_noexcept_t<T>
                    >
                >::type
            >
        >
    > {};

    template <typename T>
        requires (is_noexcept_v<T> && !has_reference_qualifier_v<T>)
    struct add_volatile_qualifier<T>
    : std::type_identity<
        add_noexcept_t<
            typename add_volatile_qualifier<
                remove_noexcept_t<T>
            >::type
        >
    > {};

    template <typename T>
        requires (!is_noexcept_v<T> && has_lvalue_reference_qualifier_v<T>)
    struct add_volatile_qualifier<T>
    : std::type_identity<
        add_lvalue_reference_qualifier_t<
            typename add_volatile_qualifier<
                remove_reference_qualifier_t<T>
            >::type
        >
    > {};

    template <typename T>
        requires (!is_noexcept_v<T> && has_rvalue_reference_qualifier_v<T>)
    struct add_volatile_qualifier<T>
    : std::type_identity<
        add_rvalue_reference_qualifier_t<
            typename add_volatile_qualifier<
                remove_reference_qualifier_t<T>
            >::type
        >
    > {};

    template <typename R, typename... Args>
    struct add_volatile_qualifier<R(Args...)>
    : std::type_identity<R(Args...) volatile> {};

    template <typename R, typename... Args>
    struct add_volatile_qualifier<R(Args......)>
    : std::type_identity<R(Args......) volatile> {};

    template <typename R, typename... Args>
    struct add_volatile_qualifier<R(Args...) const>
    : std::type_identity<R(Args...) const volatile> {};

    template <typename R, typename... Args>
    struct add_volatile_qualifier<R(Args......) const>
    : std::type_identity<R(Args......) const volatile> {};

    /// add_volatile_qualifier_t
    template <typename T>
    using add_volatile_qualifier_t = typename add_volatile_qualifier<T>::type;

    /// add_cv_qualifier_t
    template <typename T>
    using add_cv_qualifier_t = add_volatile_qualifier_t<add_const_qualifier_t<T>>;

    /// add_cv_qualifier
    template <typename T>
    using add_cv_qualifier = std::type_identity<add_cv_qualifier_t<T>>;

    /// has_va_args_v
    template <typename T>
    inline constexpr bool has_va_args_v = false;

    template <typename T>
        requires (is_noexcept_v<T> && has_cvref_qualifier_v<T>)
    inline constexpr bool has_va_args_v<T> = has_va_args_v<remove_cvref_qualifier_t<remove_noexcept_t<T>>>;

    template <typename T>
        requires (!is_noexcept_v<T> && has_cvref_qualifier_v<T>)
    inline constexpr bool has_va_args_v<T> = has_va_args_v<remove_cvref_qualifier_t<T>>;

    template <typename T>
        requires (is_noexcept_v<T> && !has_cvref_qualifier_v<T>)
    inline constexpr bool has_va_args_v<T> = has_va_args_v<remove_noexcept_t<T>>;

    template <typename R, typename... Args>
    inline constexpr bool has_va_args_v<R(Args......)> = true;

    /// has_va_args
    template <typename T>
    using has_va_args = std::bool_constant<has_va_args_v<T>>;

    /// invoke_arity
    template <typename T>
    struct invoke_arity {};

    template <typename T>
        requires (has_cvref_qualifier_v<T> && is_noexcept_v<T>)
    struct invoke_arity<T> : invoke_arity<remove_cvref_qualifier_t<remove_noexcept_t<T>>> {};

    template <typename T>
        requires (has_cvref_qualifier_v<T> && !is_noexcept_v<T>)
    struct invoke_arity<T> : invoke_arity<remove_cvref_qualifier_t<T>> {};

    template <typename T>
        requires (!has_cvref_qualifier_v<T> && is_noexcept_v<T>)
    struct invoke_arity<T> : invoke_arity<remove_noexcept_t<T>> {};

    template <typename R, typename... Args>
    struct invoke_arity<R(Args...)> : std::integral_constant<std::size_t, sizeof...(Args)> {};

    template <typename R, typename... Args>
    struct invoke_arity<R(Args......)> : std::integral_constant<std::size_t, 1 + sizeof...(Args)> {};

    template <typename R, typename... Args>
    struct invoke_arity<R(*)(Args...)> : std::integral_constant<std::size_t, sizeof...(Args)> {};

    template <typename R, typename... Args>
    struct invoke_arity<R(*)(Args......)> : std::integral_constant<std::size_t, 1 + sizeof...(Args)> {};

    template <typename T>
        requires std::is_member_object_pointer_v<T>
    struct invoke_arity<T> : std::integral_constant<std::size_t, 1> {};

    template <typename T>
        requires std::is_member_function_pointer_v<T>
    struct invoke_arity<T> : std::integral_constant<std::size_t, 1 + invoke_arity<pm_member_type_t<T>>::value> {};

    template <typename T>
        requires requires {
            &T::operator();
        }
    struct invoke_arity<T> : std::integral_constant<
        std::size_t, invoke_arity<pm_member_type_t<decltype(&T::operator())>>::value
    > {};

    /// invoke_arity_v
    // note: this variable template shall only apply to any invocable types and non-templated or non-overloaded operator()
    template <typename T>
    inline constexpr std::size_t invoke_arity_v = invoke_arity<T>::value;

    /// is_overloaded_v
    template <typename T>
    inline constexpr bool is_overloaded_v = !requires { &T::operator(); };

    /// invoke_return
    template <typename T>
    struct invoke_return {};

    template <typename T>
        requires (has_cvref_qualifier_v<T> && is_noexcept_v<T>)
    struct invoke_return<T> : invoke_return<remove_cvref_qualifier_t<remove_noexcept_t<T>>> {};

    template <typename T>
        requires (has_cvref_qualifier_v<T> && !is_noexcept_v<T>)
    struct invoke_return<T> : invoke_return<remove_cvref_qualifier_t<T>> {};

    template <typename T>
        requires (!has_cvref_qualifier_v<T> && is_noexcept_v<T>)
    struct invoke_return<T> : invoke_return<remove_noexcept_t<T>> {};

    template <typename R, typename... Args>
    struct invoke_return<R(Args...)> : std::type_identity<R> {};

    template <typename R, typename... Args>
    struct invoke_return<R(Args......)> : std::type_identity<R> {};

    template <typename R, typename... Args>
    struct invoke_return<R(*)(Args...)> : std::type_identity<R> {};

    template <typename R, typename... Args>
    struct invoke_return<R(*)(Args......)> : std::type_identity<R> {};

    template <typename T>
        requires std::is_member_object_pointer_v<T>
    struct invoke_return<T> : std::type_identity<pm_member_type_t<T>> {};

    template <typename T>
        requires std::is_member_function_pointer_v<T>
    struct invoke_return<T> : invoke_return<pm_member_type_t<T>> {};

    template <typename T>
        requires requires {
            &T::operator();
        }
    struct invoke_return<T> : invoke_return<decltype(&T::operator())> {};

    /// invoke_return_t
    template <typename T>
    using invoke_return_t = typename invoke_return<T>::type;

    /// va_args_tag_t
    struct va_args_tag_t {
        constexpr explicit va_args_tag_t() = default;
    };

    /// va_args_tag
    inline constexpr va_args_tag_t va_args_tag {};

    /// invoke_parameter
    template <typename, std::size_t>
    struct invoke_parameter {};

    template <typename T, std::size_t I>
        requires (has_cvref_qualifier_v<T> && is_noexcept_v<T>)
    struct invoke_parameter<T, I> : invoke_parameter<remove_cvref_qualifier_t<remove_noexcept_t<T>>, I> {};

    template <typename T, std::size_t I>
        requires (has_cvref_qualifier_v<T> && !is_noexcept_v<T>)
    struct invoke_parameter<T, I> : invoke_parameter<remove_cvref_qualifier_t<T>, I> {};

    template <typename T, std::size_t I>
        requires (!has_cvref_qualifier_v<T> && is_noexcept_v<T>)
    struct invoke_parameter<T, I> : invoke_parameter<remove_noexcept_t<T>, I> {};

    template <std::size_t I, typename R, typename... Args>
    struct invoke_parameter<R(Args...), I> : std::type_identity<gold::pack_element_at<I, Args...>> {};

    template <std::size_t I, typename R, typename... Args>
    struct invoke_parameter<R(Args......), I> : std::conditional<
        I == sizeof...(Args),
        va_args_tag_t,
        gold::pack_element_at<I, Args...>
    > {};

    template <std::size_t I, typename R, typename... Args>
    struct invoke_parameter<R(*)(Args...), I> : std::type_identity<gold::pack_element_at<I, Args...>> {};

    template <std::size_t I, typename R, typename... Args>
    struct invoke_parameter<R(*)(Args......), I> : std::conditional<
        I == sizeof...(Args),
        va_args_tag_t,
        gold::pack_element_at<I, Args...>
    > {};

    template <std::size_t I, typename T>
        requires (std::is_member_object_pointer_v<T> && I == 0)
    struct invoke_parameter<T, I> : std::type_identity<pm_class_type_t<T>> {};

    template <std::size_t I, typename T>
        requires (std::is_member_function_pointer_v<T> && I == 0)
    struct invoke_parameter<T, I> : std::type_identity<pm_class_type_t<T>> {};

    template <std::size_t I, typename T>
        requires (std::is_member_function_pointer_v<T> && I > 0)
    struct invoke_parameter<T, I> : invoke_parameter<pm_member_type_t<T>, I - 1> {};

    template <std::size_t I, typename T>
        requires requires {
            &T::operator();
        }
    struct invoke_parameter<T, I> : invoke_parameter<decltype(&T::operator()), I> {};

    /// invoke_parameter_t
    template <typename T, std::size_t I>
    using invoke_parameter_t = typename invoke_parameter<T, I>::type;

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_TYPE_INVOKE_TRAITS_HPP
