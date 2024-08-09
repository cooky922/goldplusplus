// <gold/bits/tuples/to.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_TUPLES_TO_HPP
#define __GOLD_BITS_TUPLES_TO_HPP

#include <gold/bits/tuples/apply.hpp>

namespace gold::tuples {

    namespace __cpo_access {

        /// tuples::__cpo_access::to_fn
        template <typename R>
        struct to_fn {
          private:
            inline static constexpr auto s_ctor_ = []<typename... Args>(Args&&... args) {
                return R(std::forward<Args>(args)...);
            };

            template <typename Tuple>
            static consteval bool s_noexcept_() {
                return []<std::size_t... Is>(std::index_sequence<Is...>) {
                    return std::is_nothrow_constructible_v<
                        R, __tuples::get_result_t<Tuple, Is>...
                    >;
                }(std::make_index_sequence<__tuples::decayed_tuple_size<Tuple>>{});
            }

          public:
            template <tuples::tuple_like Tuple>
                requires tuples::applicable<decltype(s_ctor_), Tuple>
            static constexpr R operator()(Tuple&& tup) noexcept(s_noexcept_<Tuple>()) {
                return tuples::apply(s_ctor_, std::forward<Tuple>(tup));
            }

        };

    } // namespace __cpo_access

    inline namespace __cpo {

        /// tuples::to
        template <typename R>
        inline constexpr __cpo_access::to_fn<R> to {};

    } // inline namespace __cpo

} // namespace gold::tuples

#endif // __GOLD_BITS_TUPLES_TO_HPP
