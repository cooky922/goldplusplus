// <gold/bits/type_traits/type_transformations.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TYPE_TRANSFORMATIONS_HPP
#define __GOLD_BITS_TYPE_TRAITS_TYPE_TRANSFORMATIONS_HPP

#include <type_traits>
#include <gold/bits/type_traits/conditional.hpp>

namespace gold {

    /// conditional[_t] [ defined in <gold/bits/type_traits/conditional.hpp> ]

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

    namespace __type_trait {

        /// __type_trait::copy_ref
        template <typename To, typename From>
        using copy_ref = gold::conditional_t<std::is_lvalue_reference_v<From>, To&, To>;

        /// __type_trait::remove_ref_if_xval
        template <typename T>
        using remove_ref_if_xval = gold::conditional_t<std::is_rvalue_reference_v<T>, std::remove_reference_t<T>, T>;

        /// __type_trait::copy_cv
        template <typename To, typename From>
        using copy_cv = gold::conditional_t<std::is_const_v<From>, const To, To>;

    } // namespace __type_trait

    /// copy_cvref_t
    template <typename To, typename From>
    using copy_cvref_t = __type_trait::copy_ref<__type_trait::copy_cv<To, From>, From>;

    /// forward_like_t
    // without rvalue references
    template <typename To, typename From>
    using forward_like_t = __type_trait::remove_ref_if_xval<copy_cvref_t<To, From>>;

} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_TYPE_TRANSFORMATIONS_HPP
