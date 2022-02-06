// <gold/bits/type_traits/specialization_of.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TYPE_TRAITS_TYPE_SPECIALIZATION_OF_HPP
#define __GOLD_BITS_TYPE_TRAITS_TYPE_SPECIALIZATION_OF_HPP

namespace gold {

    namespace __type_trait {

        /// notation :
        /// T = type
        /// V = value
        /// Tp [T] = type template
        /// Tp [V] = value template

        /// is_spec_of_types_v [ T ... ]
        template <typename, template <typename...> typename>
        inline constexpr bool is_spec_of_types_v = false;

        template <template <typename...> typename Temp, typename... Ts>
        inline constexpr bool is_spec_of_types_v<Temp<Ts...>, Temp> = true;

        /// is_spec_of_vals_v [ V ... ]
        template <typename, template <auto...> typename>
        inline constexpr bool is_spec_of_vals_v = false;

        template <template <auto...> typename Temp, auto... Ns>
        inline constexpr bool is_spec_of_vals_v<Temp<Ns...>, Temp> = true;

        /// is_spec_of_tvs_v [ T, V ... ]
        template <typename, template <typename, auto...> typename>
        inline constexpr bool is_spec_of_tvs_v = false;

        template <template <typename, auto...> typename Temp, typename T, auto... Ns>
        inline constexpr bool is_spec_of_tvs_v<Temp<T, Ns...>, Temp> = true;

        /// is_spec_of_vts [ V, T ... ]
        template <typename, template <auto, typename...> typename>
        inline constexpr bool is_spec_of_vts_v = false;

        template <template <auto, typename...> typename Temp, auto N, typename... Ts>
        inline constexpr bool is_spec_of_vts_v<Temp<N, Ts...>, Temp> = true;

        /// is_spec_of_tpt_v [ Tp [ T ... ] ... ]
        template <typename, template <template <typename...> typename...> typename>
        inline constexpr bool is_spec_of_tpt_v = false;

        template <template <template <typename...> typename...> typename Temp, template <typename...> typename... Tps>
        inline constexpr bool is_spec_of_tpt_v<Temp<Tps...>, Temp> = true;

        /// is_spec_of_tpv_v [ Tp [ V ... ] ... ]
        template <typename, template <template <auto...> typename...> typename>
        inline constexpr bool is_spec_of_tpv_v = false;

        template <template <template <auto...> typename...> typename Temp, template <auto...> typename... Tps>
        inline constexpr bool is_spec_of_tpv_v<Temp<Tps...>, Temp> = true;

        //// ADD MORE SPECIALIZATIONS AS YOU CAN!!


    } // namespace __type_trait

    /// is_specialization_of_v
    template <typename T, template <typename...> typename Temp>
    consteval bool is_specialization_of_v() {
        return __type_trait::is_spec_of_types_v<T, Temp>;
    }

    template <typename T, template <auto...> typename Temp>
    consteval bool is_specialization_of_v() {
        return __type_trait::is_spec_of_vals_v<T, Temp>;
    }

    template <typename T, template <typename, auto...> typename Temp>
    consteval bool is_specialization_of_v() {
        return __type_trait::is_spec_of_tvs_v<T, Temp>;
    }

    template <typename T, template <auto, typename...> typename Temp>
    consteval bool is_specialization_of_v() {
        return __type_trait::is_spec_of_vts_v<T, Temp>;
    }

    template <typename T, template <template <typename...> typename...> typename Temp>
    consteval bool is_specialization_of_v() {
        return __type_trait::is_spec_of_tpt_v<T, Temp>;
    }

    template <typename T, template <template <auto...> typename...> typename Temp>
    consteval bool is_specialization_of_v() {
        return __type_trait::is_spec_of_tpv_v<T, Temp>;
    }


} // namespace gold

#endif // __GOLD_BITS_TYPE_TRAITS_TYPE_SPECIALIZATION_OF_HPP
