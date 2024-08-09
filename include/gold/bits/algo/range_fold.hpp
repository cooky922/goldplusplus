// <gold/bits/algo/range_fold.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_ALGO_RANGE_FOLD_HPP
#define __GOLD_BITS_ALGO_RANGE_FOLD_HPP

#include <optional>
#include <gold/bits/functional/invoke.hpp>
#include <gold/bits/iterator/iter_primitives.hpp>
#include <gold/bits/ranges/range_primitives.hpp>

namespace gold::ranges {

    /// ranges::in_value_result
    template <typename I, typename T>
    struct in_value_result {
        [[no_unique_address]] I in;
        [[no_unique_address]] T value;

        template <typename I2, typename T2>
            requires std::convertible_to<const I&, I2> && std::convertible_to<const T&, T2>
        constexpr operator in_value_result<I2, T2>() const& {
            return {in, value};
        }

        template <typename I2, typename T2>
            requires std::convertible_to<I, I2> && std::convertible_to<T, T2>
        constexpr operator in_value_result<I2, T2>() && {
            return {std::move(in), std::move(value)};
        }
    };

    namespace __ranges {

        /// ranges::__ranges::flipped
        template <typename F>
        class flipped {
          private:
            F f;

          public:
            template <typename T, typename U>
                requires std::invocable<F&, U, T>
            auto operator()(T&&, U&&) -> std::invoke_result_t<F&, U, T>;
        };

        /// ranges::__ranges::indirectly_binary_left_foldable_impl
        template <typename F, typename T, typename I, typename U>
        concept indirectly_binary_left_foldable_impl =
            std::movable<T> &&
            std::movable<U> &&
            std::convertible_to<T, U> &&
            std::invocable<F&, U, std::iter_reference_t<I>> &&
            std::assignable_from<U&, std::invoke_result_t<F&, U, std::iter_reference_t<I>>>;

        /// ranges::__ranges::indirectly_binary_left_foldable
        template <typename F, typename T, typename I, typename Ref = std::iter_reference_t<I>>
        concept indirectly_binary_left_foldable =
            std::copy_constructible<F> &&
            std::indirectly_readable<I> &&
            std::invocable<F&, T, Ref> &&
            std::convertible_to<
                std::invoke_result_t<F&, T, Ref>,
                std::decay_t<std::invoke_result_t<F&, T, Ref>>
            > &&
            indirectly_binary_left_foldable_impl<
                F, T, I,
                std::decay_t<std::invoke_result_t<F&, T, Ref>>
            >;

        /// ranges::__ranges::indirectly_binary_right_foldable
        template <typename F, typename T, typename I>
        concept indirectly_binary_right_foldable = indirectly_binary_left_foldable<flipped<F>, T, I>;

    } // namespace __ranges

    /// ranges::fold_left_with_iter_result
    template <typename I, typename T>
    using fold_left_with_iter_result = in_value_result<I, T>;

    /// ranges::fold_left_first_with_iter_result
    template <typename I, typename T>
    using fold_left_first_with_iter_result = in_value_result<I, T>;

    namespace __ranges {

        /// ranges::__ranges::fold_left_with_iter_fn
        struct fold_left_with_iter_fn {
            template <std::input_iterator I, std::sentinel_for<I> S,
                      typename T,
                      __ranges::indirectly_binary_left_foldable<T, I> F,
                      typename U = std::decay_t<std::invoke_result_t<F&, T, std::iter_reference_t<I>>>>
            static constexpr fold_left_with_iter_result<I, U>
            operator()(I first, S last, T init, F f) {
                if (first == last)
                    return {.in = std::move(first), .value = static_cast<U>(std::move(init))};
                U accum = gold::invoke(f, std::move(init), *first);
                for (++first; first != last; ++first)
                    accum = gold::invoke(f, std::move(accum), *first);
                return {.in = std::move(first), .value = std::move(accum)};
            }

            template <std::ranges::input_range R, typename T,
                      __ranges::indirectly_binary_left_foldable<T, std::ranges::iterator_t<R>> F,
                      typename U = std::decay_t<
                        std::invoke_result_t<F&, T, std::ranges::range_reference_t<R>>>>
            static constexpr fold_left_with_iter_result<std::ranges::borrowed_iterator_t<R>, U>
            operator()(R&& r, T init, F f) {
                return operator()(std::ranges::begin(r), std::ranges::end(r),
                                  std::move(init), f);
            }
        };

    } // namespace __ranges

    /// ranges::fold_left_with_iter
    inline constexpr __ranges::fold_left_with_iter_fn fold_left_with_iter {};

    namespace __ranges {

        /// ranges::__ranges::fold_left_fn
        struct fold_left_fn {
            template <std::input_iterator I, std::sentinel_for<I> S,
                      typename T,
                      __ranges::indirectly_binary_left_foldable<T, I> F>
            static constexpr auto operator()(I first, S last, T init, F f) {
                return ranges::fold_left_with_iter(std::move(first), last, std::move(init), f).value;
            }

            template <std::ranges::input_range R, typename T,
                      __ranges::indirectly_binary_left_foldable<T, std::ranges::iterator_t<R>> F>
            static constexpr auto operator()(R&& r, T init, F f) {
                return operator()(std::ranges::begin(r), std::ranges::end(r),
                                  std::move(init), f);
            }
        };

    } // namespace __ranges

    /// ranges::fold_left
    inline constexpr __ranges::fold_left_fn fold_left {};

    namespace __ranges {

        /// ranges::__ranges::fold_left_first_with_iter
        struct fold_left_first_with_iter_fn {
            template <std::input_iterator I, std::sentinel_for<I> S,
                      __ranges::indirectly_binary_left_foldable<std::iter_value_t<I>, I> F,
                      typename U = decltype(ranges::fold_left(
                        std::move(std::declval<I>()),
                        std::declval<S>(),
                        std::iter_value_t<I>(*std::declval<I>()),
                        std::declval<F>()
                      ))>
                requires std::constructible_from<std::iter_value_t<I>, std::iter_reference_t<I>>
            static constexpr auto operator()(I first, S last, F f)
            -> fold_left_first_with_iter_result<I, std::optional<U>> {
                if (first == last)
                    return {.in = std::move(first), .value = std::optional<U>()};

                std::optional<U> init {std::in_place, *first};
                for (++first; first != last; ++first)
                    *init = gold::invoke(f, std::move(*init), *first);

                return {.in = std::move(first), .value = std::move(init)};
            }

            template <std::ranges::input_range R,
                      __ranges::indirectly_binary_left_foldable<
                        std::ranges::range_value_t<R>, std::ranges::iterator_t<R>
                      > F,
                      typename U = decltype(ranges::fold_left(
                        std::move(std::declval<std::ranges::iterator_t<R>>()),
                        std::declval<std::ranges::sentinel_t<R>>(),
                        std::ranges::range_value_t<R>(*std::declval<std::ranges::iterator_t<R>>()),
                        std::declval<F>()
                      ))>
                requires std::constructible_from<std::ranges::range_value_t<R>, std::ranges::range_reference_t<R>>
            static constexpr auto operator()(R&& r, F f)
            -> fold_left_first_with_iter_result<std::ranges::borrowed_iterator_t<R>, std::optional<U>> {
                return operator()(std::ranges::begin(r), std::ranges::end(r), f);
            }
        };

    } // namespace __ranges

    /// ranges::fold_left_first_with_iter
    inline constexpr __ranges::fold_left_first_with_iter_fn fold_left_first_with_iter {};

    namespace __ranges {

        /// ranges::__ranges::fold_left_first_fn
        struct fold_left_first_fn {
            template <std::input_iterator I, std::sentinel_for<I> S,
                      __ranges::indirectly_binary_left_foldable<std::iter_value_t<I>, I> F>
            static constexpr auto operator()(I first, S last, F f) {
                return ranges::fold_left_first_with_iter(std::move(first), last, f).value;
            }

            template <std::ranges::input_range R,
                      __ranges::indirectly_binary_left_foldable<
                        std::ranges::range_value_t<R>,
                        std::ranges::iterator_t<R>> F>
            static constexpr auto operator()(R&& r, F f) {
                return operator()(std::ranges::begin(r), std::ranges::end(r), f);
            }
        };

    } // namespace __ranges

    /// ranges::fold_left_first
    inline constexpr __ranges::fold_left_first_fn fold_left_first {};

    namespace __ranges {

        /// ranges::__ranges::fold_right_fn
        struct fold_right_fn {
            template <std::bidirectional_iterator I, std::sentinel_for<I> S,
                      typename T,
                      __ranges::indirectly_binary_right_foldable<T, I> F>
            static constexpr auto operator()(I first, S last, T init, F f) {
                using U = std::decay_t<std::invoke_result_t<F&, std::iter_reference_t<I>, T>>;
                if (first == last)
                    return static_cast<U>(std::move(init));
                I tail = std::ranges::next(first, last);
                U accum = gold::invoke(f, *--tail, std::move(init));
                while (first != tail)
                    accum = gold::invoke(f, *--tail, std::move(accum));
                return accum;
            }

            template <std::ranges::bidirectional_range R, typename T,
                      __ranges::indirectly_binary_right_foldable<T, std::ranges::iterator_t<R>> F>
            static constexpr auto operator()(R&& r, T init, F f) {
                return operator()(std::ranges::begin(r), std::ranges::end(r),
                                  std::move(init), f);
            }
        };

    } // namespace __ranges

    /// ranges::fold_right
    inline constexpr __ranges::fold_right_fn fold_right {};

    namespace __ranges {

        /// ranges::__ranges::fold_right_last_fn
        struct fold_right_last_fn {
            template <std::bidirectional_iterator I, std::sentinel_for<I> S,
                      __ranges::indirectly_binary_right_foldable<std::iter_value_t<I>, I> F>
                requires std::constructible_from<std::iter_value_t<I>, std::iter_reference_t<I>>
            static constexpr auto operator()(I first, S last, F f) {
                using U = decltype(ranges::fold_right(first, last, std::iter_value_t<I>(*first), f));
                if (first == last)
                    return std::optional<U>();
                I tail = std::ranges::prev(std::ranges::next(first, std::move(last)));
                return std::optional<U>(
                    std::in_place,
                    ranges::fold_right(std::move(first), tail, std::iter_value_t<I>(*tail), std::move(f))
                );
            }

            template <std::ranges::bidirectional_range R,
                      __ranges::indirectly_binary_right_foldable<
                        std::ranges::range_value_t<R>,
                        std::ranges::iterator_t<R>> F>
                requires std::constructible_from<std::ranges::range_value_t<R>, std::ranges::range_reference_t<R>>
            static constexpr auto operator()(R&& r, F f) {
                return operator()(std::ranges::begin(r), std::ranges::end(r), f);
            }
        };

    } // namespace __ranges

    /// ranges::fold_right_last
    inline constexpr __ranges::fold_right_last_fn fold_right_last {};

} // namespace gold::ranges

#endif // __GOLD_BITS_ALGO_RANGE_FOLD_HPP
