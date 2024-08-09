// <gold/bits/type_traits/constant.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_CONSTANT_HPP
#define __GOLD_BITS_TYPE_TRAITS_CONSTANT_HPP

namespace gold {

    /// constant_t [fwd]
    template <auto>
    struct constant_t;

    namespace __type_trait {

        /// __type_trait::is_constant
        template <typename>
        inline constexpr bool is_constant = false;

        template <auto V>
        inline constexpr bool is_constant<constant_t<V>> = true;

    } // namespace __type_trait

    /// constant_t
    template <auto V>
    struct constant_t {
        using value_type = __remove_cvref(decltype(V));
        using type = constant_t;

        constexpr operator value_type(this constant_t) noexcept { return V; }
        static constexpr value_type value = V;

        // this makes it template to avoid instantiation when not used
        template <auto W = V>
        consteval constant_t<+W> operator+(this constant_t) noexcept
        { return {}; }

        template <auto W = V>
        consteval constant_t<-W> operator-(this constant_t) noexcept
        { return {}; }

        template <auto W = V>
        consteval constant_t<~W> operator~(this constant_t) noexcept
        { return {}; }

        template <auto W = V>
        consteval constant_t<!W> operator!(this constant_t) noexcept
        { return {}; }

        template <auto W = V>
        consteval constant_t<&W> operator&(this constant_t) noexcept
        { return {}; }

        template <auto W = V>
        consteval constant_t<*W> operator*(this constant_t) noexcept
        { return {}; }

        template <typename... Args>
        consteval constant_t<V(Args::value...)> operator()(this constant_t, Args...)
        { return {}; }

        template <typename... Args>
        consteval constant_t<V[Args::value...]> operator[](this constant_t, Args...)
        { return {}; }

        template <auto U>
        friend consteval constant_t<V + U> operator+(constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        friend consteval constant_t<V - U> operator-(constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        friend consteval constant_t<V * U> operator*(constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        friend consteval constant_t<V / U> operator/(constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        friend consteval constant_t<V % U> operator%(constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        friend consteval constant_t<(V << U)> operator<<(constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        friend consteval constant_t<(V >> U)> operator>>(constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        friend consteval constant_t<V & U> operator&(constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        friend consteval constant_t<V | U> operator|(constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        friend consteval constant_t<V ^ U> operator^(constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        friend consteval constant_t<V && U> operator&&(constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        friend consteval constant_t<V || U> operator||(constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        friend consteval constant_t<(V, U)> operator,(constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        consteval constant_t<V == U> operator==(this constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        consteval constant_t<V != U> operator!=(this constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        consteval constant_t<(V <=> U)> operator<=>(this constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        consteval constant_t<(V < U)> operator<(this constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        consteval constant_t<(V <= U)> operator<=(this constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        consteval constant_t<(V > U)> operator>(this constant_t, constant_t<U>) noexcept
        { return {}; }

        template <auto U>
        consteval constant_t<(V >= U)> operator>=(this constant_t, constant_t<U>) noexcept
        { return {}; }
    };

    /// constant
    template <auto V>
    inline constexpr constant_t<V> constant {};

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_CONSTANT_HPP
