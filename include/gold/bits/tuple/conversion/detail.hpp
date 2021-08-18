// <gold/bits/tuple/conversion/detail.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLE_CONVERSION_DETAIL_HPP
#define __GOLD_BITS_TUPLE_CONVERSION_DETAIL_HPP

#include <bits/utility.h>
#include <bits/stl_pair.h>
#include <bits/refwrap.h>
#include <variant>
#include <array>
#include <optional>
#include <any>
#include <stdexcept>

#include <gold/bits/tuple/tuple_concepts.hpp>

namespace gold::__detail {

    // are_closely_same_
    template <typename>
    struct are_closely_same_;

    template <typename T, typename... Ts>
    struct are_closely_same_<std::tuple<T, Ts...>> {
        inline static constexpr bool value = std::conjunction_v<
            std::is_same<std::remove_cvref_t<T>, std::remove_cvref_t<Ts>>...
        >;
    };

    // are_closely_same_v_
    template <typename T>
    inline constexpr bool are_closely_same_v_ = are_closely_same_<T>::value;

    // unique_impl_
    template <typename T, typename... Ts>
    struct unique_impl_ {
        using type = T;
    };

    template <typename... Ts, typename U, typename... Us>
    struct unique_impl_<std::tuple<Ts...>, U, Us...> {
        using type = std::conditional_t<
            (std::is_same_v<U, Ts> || ...),
            typename unique_impl_<std::tuple<Ts...>, Us...>::type,
            typename unique_impl_<std::tuple<Ts..., U>, Us...>::type
        >;
    };

    template <typename... Ts>
    using unique_impl_t_ = typename unique_impl_<std::tuple<>, Ts...>::type;

    // unique_
    template <typename>
    struct unique_ {};

    template <typename... Ts>
    struct unique_<std::tuple<Ts...>> {
        using type = unique_impl_t_<Ts...>;
    };

    // unique_t_
    template <typename Tup>
    using unique_t_ = typename unique_<Tup>::type;

    // unpack_to_variant_type_
    template <typename>
    struct unpack_to_variant_type_ {};

    template <typename... Ts>
    struct unpack_to_variant_type_<std::tuple<Ts...>> {
        using type = std::variant<Ts...>;
    };

    // unpack_to_variant_type_t_
    template <typename Tup>
    using unpack_to_variant_type_t_ = typename unpack_to_variant_type_<Tup>::type;

    // to_array_type_
    template <typename>
    struct to_array_type_ {};

    template <typename... Ts>
    struct to_array_type_<std::tuple<Ts...>> {
        using type = std::array<
            unpack_to_variant_type_t_<unique_impl_t_<Ts...>>,
            sizeof...(Ts)
        >;
    };

    template <typename T0, typename T1>
    struct to_array_type_<std::pair<T0, T1>> {
        using type = std::array<std::variant<T0, T1>, 2>;
    };

    template <typename T>
    struct to_array_type_<std::pair<T, T>> {
        using type = std::array<std::variant<T>, 2>;
    };

    // to_array_type_t_
    template <typename T>
    using to_array_type_t_ = typename to_array_type_<T>::type;

    // to_array_ref_type_
    template <typename>
    struct to_array_ref_type_ {};

    template <typename... Ts>
    struct to_array_ref_type_<std::tuple<Ts...>> {
        using type = std::array<
            unpack_to_variant_type_t_<unique_impl_t_<std::reference_wrapper<Ts>...>>,
            sizeof...(Ts)
        >;
    };

    template <typename T0, typename T1>
    struct to_array_ref_type_<std::pair<T0, T1>> {
        using type = std::array<
            std::variant<
                std::reference_wrapper<T0>, std::reference_wrapper<T1>
            >, 2
        >;
    };

    template <typename T>
    struct to_array_ref_type_<std::pair<T, T>> {
        using type = std::array<
            std::variant<
                std::reference_wrapper<T>
            >, 2
        >;
    };

    // to_array_ref_type_t_
    template <typename T>
    using to_array_ref_type_t_ = typename to_array_ref_type_<T>::type;

    // get_to_variant_
    template <typename Tuple>
    constexpr decltype(auto) get_to_variant_(Tuple&& t, std::size_t i) {

        constexpr auto size_ = std::tuple_size_v<std::remove_reference_t<Tuple>>;

        if (i >= size_) throw std::out_of_range("invalid range...");

        return [&]<std::size_t... Is>(std::index_sequence<Is...>) constexpr {
            using ret_type = unpack_to_variant_type_t_<
                unique_impl_t_<std::tuple_element_t<Is, std::remove_reference_t<Tuple>>...>
            >;

            std::optional<ret_type> value_ = std::nullopt;

            ([&]<std::size_t I>{
                if (i == I) value_ = std::get<Is>(std::forward<Tuple>(t));
             }.template operator()<Is>(), ...);

             return *value_;

        }(std::make_index_sequence<size_>{});
    }

    // get_to_variant_ref_ [[incomplete]]
    template <typename Tuple>
    constexpr decltype(auto) get_to_variant_ref_(Tuple& t, std::size_t i) {
        constexpr auto size_ = std::tuple_size_v<std::remove_reference_t<Tuple>>;
        if (i >= size_) throw std::out_of_range("invalid range...");
            return [&]<std::size_t... Is>(std::index_sequence<Is...>) constexpr {
                using ret_type = unpack_to_variant_type_t_<
                    unique_impl_t_<std::reference_wrapper<std::tuple_element_t<Is, std::remove_reference_t<Tuple>>>...>
                >;

                std::optional<ret_type> value_ = std::nullopt;

                ([&]<std::size_t I>{
                    if (i == I) value_ = std::ref(std::get<Is>(t));
                }.template operator()<Is>(), ...);

                return *value_;

        }(std::make_index_sequence<size_>{});
    }
} // namespace gold::__detail

#endif // __GOLD_BITS_TUPLE_CONVERSION_DETAIL_HPP

/*
template <typename... Args, std::size_t... I>
constexpr std::array<std::variant<Args...>, sizeof...(Args)>
tuple_to_array_impl(std::tuple<Args...> const& tuple,
                    std::index_sequence<I...>)
{
    using V = std::variant<Args...>;
    std::array<V, sizeof...(Args)> array = {
        {V(std::in_place_index_t<I>{}, std::get<I>(tuple))...}};
    return array;
}

template <typename... Args>
constexpr std::array<std::variant<Args...>, sizeof...(Args)>
tuple_to_array(std::tuple<Args...> const& tuple)

{
    return tuple_to_array_impl(tuple, std::index_sequence_for<Args...>{});
}
*/
