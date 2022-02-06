// <gold/bits/type_traits/type_transformations.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TYPE_TRANSFORMATIONS_HPP
#define __GOLD_BITS_TYPE_TRAITS_TYPE_TRANSFORMATIONS_HPP

#include <type_traits>

namespace gold {

    /// conditional_t
    template <bool Cond, typename TrueType, typename FalseType>
    using conditional_t = std::__conditional_t<Cond, TrueType, FalseType>;

    template <bool Cond, typename TrueType, typename FalseType>
    using conditional = std::type_identity<conditional_t<Cond, TrueType, FalseType>>;

    /// conditionals
    template <typename Bool, typename T, typename... R>
    struct conditionals {
        using type = conditional_t<
            Bool::value,
            T,
            typename conditionals<R...>::type
        >;
    };

    template <typename Bool, typename T>
    struct conditionals<Bool, T> {
        using type = conditional_t<
            Bool::value,
            T,
            void
        >;
    };

    /// conditionals_t
    template <typename Bool, typename T, typename... R>
    using conditionals_t = typename conditionals<Bool, T, R...>::type;


    /// decay_if
    template <bool Condition, typename T>
    struct decay_if {
        using type = conditional_t<Condition, std::decay_t<T>, T>;
    };

    /// decay_if_t
    template <bool Condition, typename T>
    using decay_if_t = typename decay_if<Condition, T>::type;

    /// type_apply
    template <typename Type, template <typename> typename MetaTrans>
    struct type_apply {
        using type = typename MetaTrans<Type>::type;
    };

    /// type_apply_t
    template <typename Type, template <typename> typename MetaTrans>
    using type_apply_t = typename type_apply<Type, MetaTrans>::type;

    /// type_apply_if
    template <bool Condition, typename Type, template <typename> typename MetaTrans>
    struct type_apply_if {
        using type = conditional_t<Condition, type_apply_t<Type, MetaTrans>, Type>;
    };

    /// type_apply_if_t
    template <bool Condition, typename Type, template <typename> typename MetaTrans>
    using type_apply_if_t = typename type_apply_if<Condition, Type, MetaTrans>::type;

    /// disable_if
    template <bool, typename = void>
    struct disable_if {};

    template <typename T>
    struct disable_if<false, T> { using type = T; };

    /// disable_if_t
    template <bool B, typename T = void>
    using disable_if_t = typename disable_if<B, T>::type;

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_TYPE_TRANSFORMATIONS_HPP
