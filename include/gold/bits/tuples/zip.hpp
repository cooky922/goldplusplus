// <gold/bits/tuples/zip.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_ZIP_HPP
#define __GOLD_BITS_TUPLES_ZIP_HPP

#include <tuple>
#include <gold/bits/algo/min_max.hpp>
#include <gold/bits/tuples/get.hpp>
#include <gold/bits/tuples/concepts.hpp>
#include <gold/bits/__util/uneval.hpp>

namespace gold::tuples {

    namespace __cpo_access {

        /// tuples::__cpo_access::minimum_tuple_size
        template <typename... Tuples>
        inline constexpr std::size_t minimum_tuple_size =
            __algo::min_element<std::size_t>({__tuples::decayed_tuple_size<Tuples>...});

        /// tuples::__cpo_access::uncollapsed_type_fn
        template <typename T>
        T uncollapsed_type_fn(T&&);

        /// tuples::__cpo_access::uncollapsed_type_t
        template <typename T>
        using uncollapsed_type_t = decltype(__cpo_access::uncollapsed_type_fn(__util::declval<T>()));

        /// tuples::__cpo_access::zipped_element
        template <typename Tuple, std::size_t I>
        struct zipped_element {
            using type = __cpo_access::uncollapsed_type_t<__tuples::get_result_t<Tuple, I>>;
        };

        template <typename Tuple, std::size_t I>
            requires std::is_reference_v<tuples::tuple_element_t<I, std::remove_cvref_t<Tuple>>>
        struct zipped_element<Tuple, I> {
            using type = tuples::tuple_element_t<I, std::remove_cvref_t<Tuple>>;
        };

        template <typename Tuple, std::size_t I>
            requires (!std::is_reference_v<Tuple>) &&
                     (std::is_const_v<std::remove_reference_t<Tuple>> ||
                      std::is_const_v<tuples::tuple_element_t<I, std::remove_cvref_t<Tuple>>>)
        struct zipped_element<Tuple, I> {
            using type = const tuples::tuple_element_t<I, std::remove_cvref_t<Tuple>>;
        };

        /// tuples::__cpo_access::zipped_element_t
        template <typename Tuple, std::size_t I>
        using zipped_element_t = zipped_element<Tuple, I>::type;

        /// tuples::__cpo_access::as_zipped_element
        template <typename ZippedElement, typename T>
        constexpr ZippedElement as_zipped_element(T&& op) {
            if constexpr (!__is_convertible(T, ZippedElement) && std::is_reference_v<ZippedElement>)
                return static_cast<ZippedElement>(op);
            else
                return std::forward<T>(op);
        }

        /// tuples::__cpo_access::zip_at
        template <std::size_t I, typename... Tuples>
        constexpr auto zip_at(Tuples&&... tuples) {
            return std::tuple<__cpo_access::zipped_element_t<Tuples, I>...>(
                __cpo_access::as_zipped_element<__cpo_access::zipped_element_t<Tuples, I>>(tuples::get <I> (std::forward<Tuples>(tuples))) ...
            );
        }

        /// tuples::__cpo_access::zip_fn
        struct zip_fn {
          private:
            template <typename... Tuples>
            static consteval bool s_noexcept_() {
                if constexpr (sizeof...(Tuples) == 0)
                    return true;
                else {
                    constexpr std::size_t min_size = __cpo_access::minimum_tuple_size<Tuples...>;
                    if constexpr (min_size == 0)
                        return true;
                    else
                        return [&]<std::size_t... Is>(gold::index_seq_t<Is...>) {
                            return ([&]<std::size_t I> {
                                return (std::is_nothrow_convertible_v<__tuples::get_result_t<Tuples, I>, zipped_element_t<Tuples, I>> && ...);
                            }.template operator()<Is>() && ...);
                        }(gold::make_index_seq<min_size>);
                }
            }

            template <typename Tuple, typename... Rest>
            static constexpr decltype(auto) sf_first_tuple_(Tuple&& tup, Rest&&...) {
                return std::forward<Tuple>(tup);
            }

          public:
            template <tuples::tuple_like... Tuples>
            static constexpr auto operator()(Tuples&&... tuples) noexcept(s_noexcept_<Tuples...>()) {
                if constexpr (sizeof...(Tuples) == 0)
                    return std::tuple<>();
                else if constexpr (sizeof...(Tuples) == 1)
                    return sf_first_tuple_(std::forward<Tuples>(tuples)...);
                else {
                    constexpr std::size_t min_size = __cpo_access::minimum_tuple_size<Tuples...>;
                    if constexpr (min_size == 0)
                        return std::tuple<>();
                    else
                        return [&]<std::size_t... Is>(gold::index_seq_t<Is...>) {
                            return std::make_tuple (__cpo_access::zip_at<Is>(std::forward<Tuples>(tuples)...)...);
                        }(gold::make_index_seq<min_size>);
                }
            }
        };

    } // namespace __cpo_access

    inline namespace __cpo {

        /// tuples::zip
        inline constexpr __cpo_access::zip_fn zip {};

    } // inline namespace __cpo

} // namespace gold::tuples

#endif // __GOLD_BITS_TUPLES_ZIP_HPP
