// <gold/bits/__util/meta.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_UTIL_META_HPP
#define __GOLD_BITS_UTIL_META_HPP

// minimal library support of
// type and value reflection

namespace gold::__util::meta {

    /// __util::meta::info
    struct info {
        const void* p;
        friend constexpr bool operator==(info, info) noexcept = default;
    };

    namespace __meta {
        template <meta::info>
        struct meta_id {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-template-friend"
            friend constexpr auto $get_info(meta_id);
#pragma GCC diagnostic pop
        };

        enum class info_kind {
            is_invalid,
            is_type,
            is_value
        };

        struct meta_invalid_info {
            static constexpr auto kind = info_kind::is_invalid;
            static constexpr bool $info_tag {};
            friend constexpr auto $get_info(meta_id<meta::info{&$info_tag}>) {
                return meta_invalid_info {};
            }
        };

        template <typename T>
        struct meta_type_info {
            using type = T;
            static constexpr auto kind = info_kind::is_type;
            static constexpr bool $info_tag {};
            friend constexpr auto $get_info(meta_id<meta::info{&$info_tag}>) {
                return meta_type_info {};
            }
        };

        template <auto V>
        struct meta_value_info {
            static constexpr auto value = V;
            using type = decltype(V);
            static constexpr auto kind = info_kind::is_value;
            static constexpr bool $info_tag {};
            friend constexpr auto $get_info(meta_id<meta::info{&$info_tag}>) {
                return meta_value_info {};
            }
        };
    }

    /// __util::meta::reflect_type
    template <typename T>
    inline constexpr info reflect_type =
        info { &__meta::meta_type_info<T>::$info_tag };

    /// __util::meta::reflect_value
    template <auto V>
    inline constexpr info reflect_value =
        info { &__meta::meta_value_info<V>::$info_tag };

    namespace __meta {

        template <typename T>
        consteval info reflect_impl() {
            return reflect_type<T>;
        }

        template <auto V>
        consteval info reflect_impl() {
            return reflect_value<V>;
        }

#define $reflexpr(...) gold::__util::meta::__meta::reflect_impl<__VA_ARGS__>()

    } // namespace __meta

    /// __util::meta::splice_info
    template <info M>
    using splice_info = decltype($get_info(__meta::meta_id<M>{}));

    /// __util::meta::type_of
    template <info M>
    using type_of = splice_info<M>::type;

    /// __util::meta::value_of
    template <info M>
        requires requires { splice_info<M>::value; }
    inline constexpr auto value_of = splice_info<M>::value;

} // namespace gold::__util::meta

#endif // __GOLD_BITS_UTIL_META_HPP
