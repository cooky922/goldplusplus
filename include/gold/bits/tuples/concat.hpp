// <gold/bits/tuples/concat.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_CONCAT_HPP
#define __GOLD_BITS_TUPLES_CONCAT_HPP

#include <tuple>
#include <gold/bits/tuples/get.hpp>
#include <gold/bits/sequence/value_seq.hpp>
#include <gold/bits/sequence/type_seq.hpp>

namespace gold::tuples {

    namespace __tuples {

        /// tuples::__tuples::to_type_seq_impl
        template <typename Tuple>
        struct to_type_seq_impl {
            using type = decltype([]<std::size_t... Is>(gold::index_seq_t<Is...>){
                return gold::type_seq<tuples::tuple_element_t<Is, std::remove_cvref_t<Tuple>>...>;
            }(gold::make_index_seq<__tuples::decayed_tuple_size<Tuple>>));
        };

        /// tuples::__tuples::to_type_seq_t
        template <typename Tuple>
        using to_type_seq_t = to_type_seq_impl<Tuple>::type;

        /// tuples::__tuples::concat_result
        template <typename... Tuples>
        struct concat_result {
            using type = typename decltype([]{
                if constexpr (sizeof...(Tuples) == 0)
                    return gold::empty_type_seq;
                else
                    return gold::concat_type_seq<__tuples::to_type_seq_t<Tuples>...>;
            }())::template unpack<std::tuple>;
        };

        /// tuples::__tuples::concat_result_t
        template <typename... Tuples>
        using concat_result_t = concat_result<Tuples...>::type;

    } // namespace __tuples

    namespace __cpo_access {

        /// tuples::__cpo_access::make_outer_concat_index_seq_impl
        template <typename... Tuples>
        struct make_outer_concat_index_seq_impl {
            using type = decltype([]<std::size_t... Is>(gold::index_seq_t<Is...>) {
                return gold::concat_value_seq<
                    typename gold::make_index_seq_t<__tuples::decayed_tuple_size<Tuples>>::template fill<Is>
                ...>;
            }(gold::make_index_seq<sizeof...(Tuples)>));
        };

        /// tuples::__cpo_access::make_outer_concat_index_seq_t
        template <typename... Tuples>
        using make_outer_concat_index_seq_t = make_outer_concat_index_seq_impl<Tuples...>::type;

        /// tuples::__cpo_access::make_inner_concat_index_seq_t
        template <typename... Tuples>
        using make_inner_concat_index_seq_t = gold::concat_value_seq_t<
            gold::make_index_seq_t<__tuples::decayed_tuple_size<Tuples>>...
        >;

        /// tuples::__cpo_access::concat_impl
        template <typename R, std::size_t... Os, std::size_t... Is, typename Tuples>
        constexpr R concat_impl(gold::value_seq_t<Os...>, gold::value_seq_t<Is...>, Tuples tuples)
            noexcept(noexcept(R { tuples::get<Is>(tuples::get<Os>(std::move(tuples)))... }))
        {
            return R { tuples::get<Is>(tuples::get<Os>(std::move(tuples)))... };
        }

        /// tuples::__cpo_access::concat_fn
        struct concat_fn {
          private:
            template <typename... Tuples>
            static consteval bool s_noexcept_() {
                using R = __tuples::concat_result_t<Tuples...>;
                if constexpr (sizeof...(Tuples) == 0) {
                    return std::is_nothrow_default_constructible_v<R>;
                } else {
                    using OuterIdxSeq = make_outer_concat_index_seq_t<Tuples...>;
                    using InnerIdxSeq = make_inner_concat_index_seq_t<Tuples...>;
                    return noexcept(__cpo_access::concat_impl<R>(
                        OuterIdxSeq{},
                        InnerIdxSeq{},
                        std::forward_as_tuple(std::declval<Tuples>()...)
                    ));
                }
            }

          public:
            template <gold::tuples::tuple_like... Tuples>
            static constexpr __tuples::concat_result_t<Tuples...> operator()(Tuples&&... tuples) noexcept(s_noexcept_<Tuples...>()) {
                using R = __tuples::concat_result_t<Tuples...>;
                if constexpr (sizeof...(Tuples) == 0) {
                    return R {};
                } else {
                    using OuterIdxSeq = make_outer_concat_index_seq_t<Tuples...>;
                    using InnerIdxSeq = make_inner_concat_index_seq_t<Tuples...>;
                    return __cpo_access::concat_impl<R>(
                        OuterIdxSeq{},
                        InnerIdxSeq{},
                        std::forward_as_tuple(std::forward<Tuples>(tuples)...)
                    );
                }
            }

        };

    } // namespace __cpo_access

    inline namespace __cpo {

        /// tuples::concat
        inline constexpr __cpo_access::concat_fn concat {};

    } // inline namespace __cpo

} // namespace gold::tuples

#endif // __GOLD_BITS_TUPLES_CONCAT_HPP
