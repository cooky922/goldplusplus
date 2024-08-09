// <gold/bits/__util/like_t.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_LIKE_T_HPP
#define __GOLD_BITS_UTIL_LIKE_T_HPP

#include <gold/bits/__util/uneval.hpp>

namespace gold::__util {

    namespace __impl {

        /// __util::cvref_kind
        using cvref_kind = unsigned char;
        // T         => 0b000u
        // const T   => 0b001u
        // T&        => 0b010u
        // const T&  => 0b011u
        // T&&       => 0b100u
        // const T&& => 0b101u

        /// __util::cvref_kind_of
        template <typename>
        inline constexpr auto cvref_kind_of =
            cvref_kind(0b000u);

        template <typename T>
        inline constexpr auto cvref_kind_of<const T> =
            cvref_kind(0b001u);

        template <typename T>
        inline constexpr auto cvref_kind_of<T&> =
            cvref_kind(0b010u);

        template <typename T>
        inline constexpr auto cvref_kind_of<const T&> =
            cvref_kind(0b011u);

        template <typename T>
        inline constexpr auto cvref_kind_of<T&&> =
            cvref_kind(0b100u);

        template <typename T>
        inline constexpr auto cvref_kind_of<const T&&> =
            cvref_kind(0b101u);

        consteval bool has_const(cvref_kind kind) {
            return static_cast<bool>(kind & 0b001u);
        }

        consteval bool has_ref(cvref_kind kind) {
            return static_cast<bool>(kind >> 1);
        }

        consteval bool has_lref(cvref_kind kind) {
            return static_cast<bool>(kind & 0b010u);
        }

        consteval bool has_rref(cvref_kind kind) {
            return static_cast<bool>(kind & 0b100u);
        }

        /// __util::__impl::copy_const_t
        template <typename To, typename From>
        using copy_const_t = __type_pack_element<
            int(__impl::has_const(__impl::cvref_kind_of<From>)),
            To, const To
        >;

        /// __util::__impl::merge_ref_t
        template <typename To, typename From>
        using merge_ref_t = __type_pack_element<
            int(__impl::cvref_kind_of<From> >> 1),
            To,
            To&,
            To&&
        >;

        /// __util::__impl::copy_ref_t
        template <typename To, typename From>
        using copy_ref_t = __type_pack_element<
            int(__impl::cvref_kind_of<From> >> 1),
            To,
            To&,
            __remove_reference(To) &&
        >;

        /// __util::__impl::member_like_impl
        template <typename T>
        struct member_like_impl {
            T member;
        };

    } // namespace __impl

    /// __util::merge_cvref_t
    template <typename To, typename From>
    using merge_cvref_t = __impl::merge_ref_t<
        __impl::copy_ref_t<__impl::copy_const_t<__remove_reference(To), __remove_reference(From)>, To>,
        From
    >;

    /// __util::copy_cvref_t
    template <typename To, typename From>
    using copy_cvref_t = __impl::copy_ref_t<
        __impl::copy_const_t<__remove_cvref(To), __remove_reference(From)>,
        From
    >;

    /// __util::member_like_t
    template <typename Member, typename Container>
    using member_like_t = decltype((__util::uneval<__util::copy_cvref_t<__impl::member_like_impl<Member>, Container>>().member));

} // namespace gold::__util

#endif // __GOLD_BITS_UTIL_LIKE_T_HPP
