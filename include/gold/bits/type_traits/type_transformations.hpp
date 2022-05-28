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

    namespace __type_trait {

        // workaround since direct template alias of 'decltype(auto(<expr>))' is currently ICE
        /// __type_trait::decay
        template <typename>
        struct decay;

        // partial specialization
        template <typename T>
            requires requires { auto(std::declval<T>()); }
        struct decay<T> {
            using type = decltype(auto(std::declval<T>()));
        };

        // primary template definition
        template <typename T>
        struct decay {
            using type = std::remove_cvref_t<T>;
        };

    } // namespace __type_trait

    /// decay_t
    template <typename T>
    using decay_t = __type_trait::decay<T>::type;

    /// decay
    template <typename T>
    using decay = std::type_identity<decay_t<T>>;

    /// decay_if_t
    template <bool Cond, typename T>
    using decay_if_t = conditional_t<Cond, decay_t<T>, T>;

    /// decay_if
    template <bool Cond, typename T>
    using decay_if = std::type_identity<decay_if_t<Cond, T>>;

    /// invoke_class_template
    template <template <typename...> typename Temp, typename... Ts>
    struct invoke_class_template {
        using type = Temp<Ts...>;
    };

    /// invoke_class_template_t
    template <template <typename...> typename Temp, typename... Ts>
    using invoke_class_template_t = typename invoke_class_template<Temp, Ts...>::type;

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
