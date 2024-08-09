// <gold/bits/memory/ops_algo.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MEMORY_OPS_ALGO_HPP
#define __GOLD_BITS_MEMORY_OPS_ALGO_HPP

#include <bits/ranges_base.h>
#if __has_include(<bits/ranges_algo_results.h>)
#include <bits/ranges_algo_results.h>
#else
#include <algorithm>
#endif
#include <gold/bits/memory/ops.hpp>
#include <gold/bits/concepts/types.hpp>

namespace gold {

    namespace __algo {

        /// __algo::iter_construct_at_fn
        struct iter_construct_at_fn {

            template <typename Iter, typename... Args>
            inline static constexpr bool s_noexcept_ =
                noexcept(std::construct_at(std::to_address(std::declval<Iter>()), std::declval<Args>()...));

            template <std::input_iterator Iter, typename... Args>
                requires std::constructible_from<std::iter_value_t<Iter>, Args...>
            static constexpr auto operator()(Iter iter, Args&&... args)
                noexcept(s_noexcept_<Iter, Args...>) {
                return std::construct_at(std::to_address(iter), std::forward<Args>(args)...);
            }

            template <std::input_iterator Iter, typename U, typename... Args>
                requires std::constructible_from<std::iter_value_t<Iter>, std::initializer_list<U>, Args...>
            static constexpr auto operator()(Iter iter, std::initializer_list<U> il, Args&&... args)
                noexcept(s_noexcept_<Iter, std::initializer_list<U>, Args...>) {
                return std::construct_at(std::to_address(iter), il, std::forward<Args>(args)...);
            }
        };

        /// __algo::iter_destroy_at_fn
        struct iter_destroy_at_fn {

            template <std::input_iterator Iter>
                requires std::destructible<std::iter_value_t<Iter>>
            static constexpr void operator()(Iter iter)
                noexcept(std::is_nothrow_destructible_v<std::iter_value_t<Iter>>) {
                std::destroy_at(std::to_address(iter));
            }

        };

        /// __algo::iter_relocate_at_fn
        struct iter_relocate_at_fn {

            template <std::input_iterator Iter>
                requires gold::relocatable<std::iter_value_t<Iter>>
            static constexpr auto operator()(Iter from, Iter to)
                noexcept(std::is_nothrow_move_constructible_v<std::iter_value_t<Iter>> &&
                               std::is_nothrow_destructible_v<Iter>) {
                    struct guard {
                        decltype(std::to_address(from)) ptr;
                        constexpr ~guard() { std::destroy_at(ptr); }
                    } g(std::to_address(from));
                    std::construct_at(std::to_address(to), std::move(*from));
                    return to;
                }

        };


    } // namespace __algo

    /// iter_construct_at
    inline constexpr __algo::iter_construct_at_fn iter_construct_at {};

    /// iter_destroy_at
    inline constexpr __algo::iter_destroy_at_fn iter_destroy_at {};

    /// iter_relocate_at
    inline constexpr __algo::iter_relocate_at_fn iter_relocate_at {};

    namespace __algo {

        /// __algo::destroy_fn
        struct destroy_fn {
            template <std::input_iterator Iter, std::sentinel_for<Iter> Sent>
                requires std::destructible<std::iter_value_t<Iter>>
            static constexpr Iter operator()(Iter iter, Sent sent) noexcept {
                for (; iter != sent; ++iter)
                    gold::iter_destroy_at(iter);
                return iter;
            }

            template <std::ranges::input_range R>
                requires std::destructible<std::ranges::range_value_t<R>>
            static constexpr std::ranges::borrowed_iterator_t<R> operator()(R&& r) noexcept {
                return operator()(std::ranges::begin(r), std::ranges::end(r));
            }
        };

        /// __algo::destroy_n_fn
        struct destroy_n_fn {
            template <std::input_iterator Iter>
                requires std::destructible<std::iter_value_t<Iter>>
            static constexpr Iter operator()(Iter iter, std::iter_difference_t<Iter> n) noexcept {
                for (; n != 0; (void) ++iter, --n)
                    gold::iter_destroy_at(iter);
                return iter;
            }
        };



    } // namespace __algo

    /// destroy
    inline constexpr __algo::destroy_fn destroy {};

    /// destroy_n
    inline constexpr __algo::destroy_n_fn destroy_n {};

    namespace __algo {

        /// __algo::uninit_default_construct_init
        struct uninit_default_construct_fn {
            template <std::forward_iterator Iter, std::sentinel_for<Iter> Sent>
                requires std::is_nothrow_default_constructible_v<std::iter_value_t<Iter>>
            static constexpr Iter operator()(Iter iter, Sent sent) noexcept {
                using element_type = std::remove_reference_t<std::iter_reference_t<Iter>>;
                if constexpr (std::is_trivially_default_constructible_v<element_type>)
                    return std::ranges::next(iter, sent);
                for (; iter != sent; ++iter)
                    gold::iter_construct_at(iter);
                return iter;
            }

            template <std::ranges::forward_range R>
                requires std::is_nothrow_default_constructible_v<std::ranges::range_value_t<R>>
            static constexpr std::ranges::borrowed_iterator_t<R> operator()(R&& r) noexcept {
                return operator()(std::ranges::begin(r), std::ranges::end(r));
            }

        };

        /// __algo::uninit_default_construct_n_fn
        struct uninit_default_construct_n_fn {
            template <std::forward_iterator Iter>
                requires std::is_nothrow_default_constructible_v<std::iter_value_t<Iter>>
            static constexpr Iter operator()(Iter iter, std::iter_difference_t<Iter> n) noexcept {
                for (; n != 0; (void) ++iter, --n)
                    gold::iter_construct_at(iter);
                return iter;
            }
        };

    } // namespace __algo

    /// uninit_default_construct
    inline constexpr __algo::uninit_default_construct_fn uninit_default_construct {};

    /// uninit_default_construct_n
    inline constexpr __algo::uninit_default_construct_n_fn uninit_default_construct_n {};

    namespace __algo {

        /// __algo::uninit_fill_fn
        struct uninit_fill_fn {

            template <std::forward_iterator Iter, std::sentinel_for<Iter> Sent, typename T>
                requires std::constructible_from<std::iter_value_t<Iter>, const T&>
            static constexpr Iter operator()(Iter iter, Sent sent, const T& op) {
                Iter rollback { iter };
                try {
                    for (; iter != sent; ++iter)
                        gold::iter_construct_at(iter, op);
                    return iter;
                } catch (...) {
                    for (; rollback != iter; ++rollback)
                        gold::iter_destroy_at(rollback);
                    throw;
                }
            }

            template <std::ranges::forward_range R, typename T>
                requires std::constructible_from<std::ranges::range_value_t<R>, const T&>
            static constexpr std::ranges::borrowed_iterator_t<R> operator()(R&& r) {
                return operator()(std::ranges::begin(r), std::ranges::end(r));
            }

        };

        /// __algo::uninit_fill_n_fn
        struct uninit_fill_n_fn {

            template <std::forward_iterator Iter, typename T>
                requires std::constructible_from<std::iter_value_t<Iter>, const T&>
            static constexpr Iter operator()(Iter iter, std::iter_difference_t<Iter> n, const T& op) {
                Iter rollback { iter };
                try {
                    for (; n != 0; (void) ++iter, --n)
                        gold::iter_construct_at(iter, op);
                    return iter;
                } catch (...) {
                    for (; rollback != iter; ++rollback)
                        gold::iter_destroy_at(rollback);
                    throw;
                }
            }

        };

    } // namespace __algo

    /// uninit_fill
    inline constexpr __algo::uninit_fill_fn uninit_fill {};

    /// uninit_fill_n
    inline constexpr __algo::uninit_fill_n_fn uninit_fill_n {};

    /// uninit_copy_result
    template <typename In, typename Out>
    using uninit_copy_result = std::ranges::in_out_result<In, Out>;

    /// uninit_copy_n_result
    template <typename In, typename Out>
    using uninit_copy_n_result = std::ranges::in_out_result<In, Out>;

    namespace __algo {

        /// __algo::uninit_copy_fn
        struct uninit_copy_fn {

            template <std::input_iterator   In,  std::sentinel_for<In>  Sent1,
                      std::forward_iterator Out, std::sentinel_for<Out> Sent2>
                requires std::constructible_from<std::iter_value_t<Out>, std::iter_reference_t<In>>
            static constexpr uninit_copy_result<In, Out>
            operator()(In in, Sent1 sent1, Out out, Sent2 sent2) {
                Out current { out };
                try {
                    for (; !(in == sent1 || current == sent2); ++in, ++current)
                        gold::iter_construct_at(current, *in);
                    return { .in = std::move(in), .out = std::move(current) };
                } catch (...) {
                    for (; out != current; ++out)
                        gold::iter_destroy_at(out);
                    throw;
                }
            }

            template <std::ranges::input_range RIn, std::ranges::forward_range ROut>
                requires std::constructible_from<std::ranges::range_value_t<ROut>,
                                                 std::ranges::range_reference_t<RIn>>
            static constexpr uninit_copy_result<std::ranges::borrowed_iterator_t<RIn>,
                                         std::ranges::borrowed_iterator_t<ROut>>
            operator()(RIn&& r_in, ROut&& r_out) {
                return operator()(std::ranges::begin(r_in), std::ranges::end(r_in),
                                  std::ranges::begin(r_out), std::ranges::end(r_out));
            }

        };

        /// __algo::uninit_copy_n_fn
        struct uninit_copy_n_fn {

            template <std::input_iterator In, std::forward_iterator Out, std::sentinel_for<Out> Sent>
                requires std::constructible_from<std::iter_value_t<Out>, std::iter_reference_t<In>>
            static constexpr uninit_copy_n_result<In, Out>
            operator()(In in, std::iter_difference_t<In> n, Out out, Sent sent) {
                Out current { out };
                try {
                    for (; n != 0 && current != sent; ++in, ++current, --n)
                        gold::iter_construct_at(current, *in);
                    return { .in = std::move(in), .out = std::move(current) };
                } catch (...) {
                    for (; out != current; ++out)
                        gold::iter_destroy_at(out);
                    throw;
                }
            }

        };

    } // namespace __algo

    /// uninit_copy
    inline constexpr __algo::uninit_copy_fn uninit_copy {};

    /// uninit_copy_n
    inline constexpr __algo::uninit_copy_n_fn uninit_copy_n {};

    /// uninit_move_result
    template <typename In, typename Out>
    using uninit_move_result = std::ranges::in_out_result<In, Out>;

    /// uninit_move_n_result
    template <typename In, typename Out>
    using uninit_move_n_result = std::ranges::in_out_result<In, Out>;

    namespace __algo {

        /// __algo::uninit_move_fn
        struct uninit_move_fn {

            template <std::input_iterator   In,  std::sentinel_for<In>  Sent1,
                      std::forward_iterator Out, std::sentinel_for<Out> Sent2>
                requires std::constructible_from<std::iter_value_t<Out>, std::iter_rvalue_reference_t<In>>
            static constexpr uninit_move_result<In, Out>
            operator()(In in, Sent1 sent1, Out out, Sent2 sent2) {
                Out current { out };
                try {
                    for (; !(in == sent1 || current == sent2); ++in, ++current)
                        gold::iter_construct_at(current, std::ranges::iter_move(in));
                    return { .in = std::move(in), .out = std::move(current) };
                } catch (...) {
                    for (; out != current; ++out)
                        gold::iter_destroy_at(out);
                    throw;
                }
            }

            template <std::ranges::input_range RIn, std::ranges::forward_range ROut>
                requires std::constructible_from<std::ranges::range_value_t<ROut>,
                                                 std::ranges::range_rvalue_reference_t<RIn>>
            static constexpr uninit_move_result<std::ranges::borrowed_iterator_t<RIn>,
                                         std::ranges::borrowed_iterator_t<ROut>>
            operator()(RIn&& r_in, ROut&& r_out) {
                return operator()(std::ranges::begin(r_in), std::ranges::end(r_in),
                                  std::ranges::begin(r_out), std::ranges::end(r_out));
            }

        };

        /// __algo::uninit_move_n_fn
        struct uninit_move_n_fn {

            template <std::input_iterator In, std::forward_iterator Out, std::sentinel_for<Out> Sent>
                requires std::constructible_from<std::iter_value_t<Out>, std::iter_rvalue_reference_t<In>>
            static constexpr uninit_move_n_result<In, Out>
            operator()(In in, std::iter_difference_t<In> n, Out out, Sent sent) {
                Out current { out };
                try {
                    for (; n != 0 && current != sent; ++in, ++current, --n)
                        gold::iter_construct_at(current, std::ranges::iter_move(in));
                    return { .in = std::move(in), .out = std::move(current) };
                } catch (...) {
                    for (; out != current; ++out)
                        gold::iter_destroy_at(out);
                    throw;
                }
            }

        };

    } // namespace __algo

    /// uninit_move
    inline constexpr __algo::uninit_move_fn uninit_move {};

    /// uninit_move_n
    inline constexpr __algo::uninit_move_n_fn uninit_move_n {};

    /// uninit_relocate_result
    template <typename In, typename Out>
    using uninit_relocate_result = std::ranges::in_out_result<In, Out>;

    /// uninit_relocate_n_result
    template <typename In, typename Out>
    using uninit_relocate_n_result = std::ranges::in_out_result<In, Out>;

    namespace __algo {

        /// __algo::uninit_relocate_fn
        struct uninit_relocate_fn {

            template <std::input_iterator   In,  std::sentinel_for<In>  Sent1,
                      std::forward_iterator Out, std::sentinel_for<Out> Sent2>
                requires std::constructible_from<std::iter_value_t<Out>, std::iter_rvalue_reference_t<In>>
                      && std::destructible<std::iter_value_t<In>>
            static constexpr uninit_relocate_result<In, Out>
            operator()(In in, Sent1 sent1, Out out, Sent2 sent2) {
                Out current { out };
                try {
                    for (; !(in == sent1 || current == sent2); ++in, ++current)
                        gold::iter_relocate_at(current, in);
                    return { .in = std::move(in), .out = std::move(current) };
                } catch (...) {
                    for (; out != current; ++out)
                        gold::iter_destroy_at(out);
                    throw;
                }
            }

            template <std::ranges::input_range RIn, std::ranges::forward_range ROut>
                requires std::constructible_from<std::ranges::range_value_t<ROut>,
                                                 std::ranges::range_rvalue_reference_t<RIn>>
                      && std::destructible<std::ranges::range_value_t<RIn>>
            static constexpr uninit_relocate_result<std::ranges::borrowed_iterator_t<RIn>,
                                         std::ranges::borrowed_iterator_t<ROut>>
            operator()(RIn&& r_in, ROut&& r_out) {
                return operator()(std::ranges::begin(r_in), std::ranges::end(r_in),
                               std::ranges::begin(r_out), std::ranges::end(r_out));
            }

        };

        /// __algo::uninit_relocate_n_fn
        struct uninit_relocate_n_fn {

            template <std::input_iterator In, std::forward_iterator Out, std::sentinel_for<Out> Sent>
                requires std::constructible_from<std::iter_value_t<Out>, std::iter_rvalue_reference_t<In>>
                      && std::destructible<std::iter_value_t<In>>
            static constexpr uninit_relocate_n_result<In, Out>
            operator()(In in, std::iter_difference_t<In> n, Out out, Sent sent) {
                Out current { out };
                try {
                    for (; n != 0 && current != sent; ++in, ++current, --n)
                        gold::iter_relocate_at(current, in);
                    return { .in = std::move(in), .out = std::move(current) };
                } catch (...) {
                    for (; out != current; ++out)
                        gold::iter_destroy_at(out);
                    throw;
                }
            }

        };

    } // namespace __algo

    /// uninit_relocate
    inline constexpr __algo::uninit_relocate_fn uninit_relocate {};

    /// uninit_relocate_n
    inline constexpr __algo::uninit_relocate_n_fn uninit_relocate_n {};

} // namespace gold

#endif // __GOLD_BITS_MEMORY_OPS_ALGO_HPP
