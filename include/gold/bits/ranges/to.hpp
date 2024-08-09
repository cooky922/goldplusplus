// <gold/bits/ranges/to.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_TO_HPP
#define __GOLD_BITS_RANGES_TO_HPP

#if __has_include(<bits/views.h>)
#include <bits/views.h>
#else
#include <ranges>
#endif
#include <gold/bits/__util/uneval.hpp>
#include <gold/bits/algo/copy.hpp>
#include <gold/bits/concepts/not_void.hpp>
#include <gold/bits/ranges/from_range.hpp>
#include <gold/bits/ranges/range_adaptor.hpp>
#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/ranges/move_each_view.hpp>
#include <gold/pack>

namespace gold::ranges {

    namespace __ranges {

        /// __ranges::__ranges::maybe_move
        template <typename S, typename R = S>
        [[gnu::always_inline]] constexpr decltype(auto) maybe_move(R&& r) {
            if constexpr (std::is_lvalue_reference_v<S> ||
                          std::is_trivially_copyable_v<std::ranges::range_reference_t<S>> ||
                          !requires { views::move_each(r); })
                return static_cast<S>(r);
            else
                return views::move_each(r);
        }

        /// ranges::__ranges::maybe_move_t
        template <typename S>
        using maybe_move_t = decltype(__ranges::maybe_move<S>(__util::uneval<S&>()));

        /// ranges::__ranges::reachable_sentinel
        template <typename S>
        inline constexpr bool reachable_sentinel = !__is_same(__remove_cvref(S), std::unreachable_sentinel_t);

        /// ranges::__ranges::reservable_container
        template <typename C>
        concept reservable_container = std::ranges::sized_range<C> && requires (C& c) {
            c.reserve(std::ranges::size(c));
            { c.capacity() } -> std::same_as<std::ranges::range_size_t<C>>;
            { c.max_size() } -> std::same_as<std::ranges::range_size_t<C>>;
        };

        /// ranges::__ranges::appendable_container
        template <typename C, typename Ref = std::ranges::range_reference_t<C>>
        inline constexpr bool appendable_container = requires (C& c, Ref&& r) {
            requires (requires { c.emplace_back(std::forward<Ref>(r)); } ||
                      requires { c.push_back(std::forward<Ref>(r));    } ||
                      requires { c.emplace(c.end(), std::forward<Ref>(r)); } ||
                      requires { c.insert(c.end(), std::forward<Ref>(r)); });
        };

        /// ranges::__ranges::container_append
        template <typename C>
        constexpr auto container_appender(C& c) {
            return [&c]<typename Ref>(Ref&& r) {
                if constexpr (requires { c.emplace_back(std::declval<Ref>()); })
                    c.emplace_back(std::forward<Ref>(r));
                else if constexpr (requires { c.push_back(std::declval<Ref>()); })
                    c.push_back(std::forward<Ref>(r));
                else if constexpr (requires { c.emplace(c.end(), std::declval<Ref>()); })
                    c.emplace(c.end(), std::forward<Ref>(r));
                else
                    c.insert(c.end(), std::forward<Ref>(r));
            };
        }

        /// ranges::__ranges::try_non_recursive_conversion
        template <typename C, typename R>
        concept try_non_recursive_conversion =
            !std::ranges::input_range<C> ||
             std::convertible_to<std::ranges::range_reference_t<R>, std::ranges::range_value_t<C>>;

        /// ranges::__ranges::constructible_from_iter_pair
        template <typename C, typename R, typename... Args>
        concept constructible_from_iter_pair =
            std::ranges::common_range<R> &&
            requires { typename std::iterator_traits<std::ranges::iterator_t<R>>::iterator_category; } &&
            std::derived_from<typename std::iterator_traits<std::ranges::iterator_t<R>>::iterator_category, std::input_iterator_tag> &&
            std::constructible_from<C, std::ranges::iterator_t<R>, std::ranges::sentinel_t<R>, Args...>;

        /// ranges::__ranges::constructible_from_uncommon_iter_sent
        template <typename C, typename R, typename... Args>
        concept constructible_from_uncommon_iter_sent =
            __ranges::constructible_from_iter_pair<C, std::ranges::common_view<R>, Args...>;

        /// ranges::__ranges::to_fn
        template <typename C>
        struct to_fn {
            // instead of static_assert, make it void
            template <typename R, typename... Args>
            [[gnu::always_inline]] static constexpr auto operator()(R&& r, Args&&... args) {
                using D = __ranges::maybe_move_t<R&&>;

                // if the range is infinite, the construction must be invalid
                if constexpr (!__ranges::reachable_sentinel<std::ranges::sentinel_t<R>>)
                    return;

                // non-recursive case, a conversion target is either:
                // + a range with a convertible value type
                // + a non-range type which might support construction from input arguments
                else if constexpr (__ranges::try_non_recursive_conversion<C, D>) {
                    // case 1: construct directly from the given range
                    if constexpr (std::constructible_from<C, D, Args...>)
                        return C(D(r), std::forward<Args>(args)...);

                    // case 2: constructing using 'gold::from_range_t' tagged ctor
                    else if constexpr (std::constructible_from<C, gold::from_range_t, D, Args...>)
                        return C(gold::from_range, D(r), std::forward<Args>(args)...);

                    // case 3: constructing using iterator pairs (if the range is common)
                    else if constexpr (__ranges::constructible_from_iter_pair<C, D, Args...>) {
                        decltype(auto) d = D(r);
                        return C(std::ranges::begin(d), std::ranges::end(d), std::forward<Args>(args)...);
                    }

                    // case 4: constructing using non-common iterator and sentinel
                    else if constexpr (__ranges::constructible_from_uncommon_iter_sent<C, D, Args...>) {
                        auto d = std::views::common(D(r));
                        return C(std::ranges::begin(d), std::ranges::end(d), std::forward<Args>(args)...);
                    }

                    // case 5: default constructing (or constructing from extra arguments)
                    //         and insert
                    else if constexpr (std::constructible_from<C, Args...> &&
                                       __ranges::appendable_container<C, std::ranges::range_reference_t<D>>) {
                        C result (std::forward<Args>(args)...);
                        // reserve some space if possible
                        if constexpr (std::ranges::sized_range<R> && __ranges::reservable_container<C>)
                            result.reserve(static_cast<std::ranges::range_size_t<C>>(std::ranges::size(r)));

                        auto appender = __ranges::container_appender(result);
                        for (auto&& op : D(r))
                            appender (std::forward<decltype(op)>(op));

                        return result;
                    }
                    else return;
                }
                // try the recursive case
                else if constexpr (std::ranges::input_range<std::ranges::range_reference_t<D>>) {
                    return operator()(
                        std::views::transform(D(r), []<typename T>(T&& elem) {
                            return __ranges::to_fn<std::ranges::range_value_t<C>>()(std::forward<T>(elem));
                        }),
                        std::forward<Args>(args)...
                    );
                }
                else return;
            }
        };

        /// ranges::__ranges::can_convert_from_impl
        template <typename C, typename R, typename... Args>
        inline constexpr bool can_convert_from_impl =
            requires {
                { __ranges::to_fn<C>()(std::declval<R>(), std::declval<Args>()...) } -> __concepts::not_void;
            };

        /// ranges::__ranges::can_convert_from
        template <typename C, typename R, typename... Args>
        concept can_convert_from =
            !std::ranges::view<C> &&
            !std::is_const_v<C> &&
             __ranges::can_convert_from_impl<C, R, Args...>;

        /// ranges::__ranges::uneval_input_iterator_t
        template <typename R>
        struct uneval_input_iterator_t {
            using iterator_category = std::input_iterator_tag;
            using value_type        = std::ranges::range_value_t<R>;
            using difference_type   = std::ptrdiff_t;
            using reference         = std::ranges::range_reference_t<R>;
            using pointer           = std::add_pointer_t<reference>;

            reference operator*() const;
            pointer operator->() const;
            uneval_input_iterator_t& operator++();
            uneval_input_iterator_t operator++(int);
            bool operator==(const uneval_input_iterator_t&) const;
        };

        /// ranges::__ranges::deduced_container
        template <template <typename...> typename C, typename R, typename... Args>
        struct deduced_container {
            using type = decltype([]{
                using Iter = __ranges::uneval_input_iterator_t<R>;
                if constexpr (requires { C(__util::uneval<R>(), __util::uneval<Args>()...); })
                    return __util::uneval<decltype(C(__util::uneval<R>(), __util::uneval<Args>()...))>;
                else if constexpr (requires { C(gold::from_range, __util::uneval<R>(), __util::uneval<Args>()...); })
                    return __util::uneval<decltype(C(gold::from_range, __util::uneval<R>(), __util::uneval<Args>()...))>;
                else if constexpr (requires { C(__util::uneval<Iter>(), __util::uneval<Iter>(), __util::uneval<Args>()...); })
                    return __util::uneval<decltype(C(__util::uneval<Iter>(), __util::uneval<Iter>(), __util::uneval<Args>()...))>;
                else
                    return __util::uneval<void>;
            }()());
        };

        /// ranges::__ranges::deduced_container_t
        template <template <typename...> typename C, typename R, typename... Args>
        using deduced_container_t = deduced_container<C, R, Args...>::type;

        /// ranges::__ranges::deducible_container
        template <template <typename...> typename C, typename R, typename... Args>
        inline constexpr bool deducible_container = !std::is_void_v<__ranges::deduced_container_t<C, R, Args...>>;

        /// ranges::__ranges::partial_to_fn_impl
        template <typename C, typename... Args>
        struct partial_to_fn_impl {
            std::tuple<Args...> bound_args;

            constexpr partial_to_fn_impl(Args&&... args)
            : bound_args(std::forward<Args>(args)...) {}

            template <std::ranges::input_range R>
                requires __ranges::can_convert_from<C, R, Args...>
            constexpr C operator()(R&& r) const {
                return std::apply([&r](Args&&... args) {
                    return __ranges::to_fn<C>()(std::forward<R>(r), std::forward<Args>(args)...);
                }, bound_args);
            }

            inline static constexpr bool has_simple_call_op = true;
        };

        template <typename C>
        struct partial_to_fn_impl<C> {
            constexpr partial_to_fn_impl() noexcept = default;

            template <std::ranges::input_range R>
                requires __ranges::can_convert_from<C, R>
            static constexpr C operator()(R&& r) {
                return __ranges::to_fn<C>()(std::forward<R>(r));
            }
        };

        /// ranges::__ranges::partial_to_fn
        template <typename C, typename... Args>
        using partial_to_fn = ranges::range_adaptor_closure<__ranges::partial_to_fn_impl<C, Args...>>;

        /// ranges::__ranges::partial_template_to_fn_impl
        template <template <typename...> typename C, typename... Args>
        struct partial_template_to_fn_impl {
            std::tuple<Args...> bound_args;

            constexpr partial_template_to_fn_impl(Args&&... args)
            : bound_args(std::forward<Args>(args)...) {}

            template <typename R>
            using Container = __ranges::deduced_container_t<C, R, Args...>;

            template <std::ranges::input_range R>
                requires __ranges::deducible_container<C, R, Args...> &&
                         __ranges::can_convert_from<Container<R>, R, Args...>
            constexpr Container<R> operator()(R&& r) const {
                return std::apply([&r](Args&&... args) {
                    return __ranges::to_fn<Container<R>>()(std::forward<R>(r), std::forward<Args>(args)...);
                }, bound_args);
            }

            inline static constexpr bool has_simple_call_op = true;
        };

        template <template <typename...> typename C>
        struct partial_template_to_fn_impl<C> {

            constexpr partial_template_to_fn_impl() noexcept = default;

            template <typename R>
            using Container = __ranges::deduced_container_t<C, R>;

            template <std::ranges::input_range R>
                requires __ranges::deducible_container<C, R> &&
                         __ranges::can_convert_from<Container<R>, R>
            static constexpr Container<R> operator()(R&& r) {
                return __ranges::to_fn<Container<R>>()(std::forward<R>(r));
            }

            inline static constexpr bool has_simple_call_op = true;
        };

        /// ranges::__ranges::partial_template_to_fn
        template <template <typename...> typename C, typename... Args>
        using partial_template_to_fn = ranges::range_adaptor_closure<__ranges::partial_template_to_fn_impl<C, Args...>>;

    } // namespace __ranges

    /// ranges::to
    template <typename C, std::ranges::input_range R, typename... Args>
        requires __ranges::can_convert_from<C, R, Args...>
    constexpr C to(R&& r, Args&&... args) {
        return __ranges::to_fn<C>()(std::forward<R>(r), std::forward<Args>(args)...);
    }

    template <typename C, std::ranges::input_range R, typename... Args>
    void to(R&&, Args&&...) = delete;

    template <template <typename...> typename C, std::ranges::input_range R, typename... Args>
        requires __ranges::deducible_container<C, R, Args...> &&
                 __ranges::can_convert_from<__ranges::deduced_container_t<C, R, Args...>, R, Args...>
    constexpr __ranges::deduced_container_t<C, R, Args...> to(R&& r, Args&&... args) {
        using D = __ranges::deduced_container_t<C, R, Args...>;
        return __ranges::to_fn<D>()(std::forward<R>(r), std::forward<Args>(args)...);
    }

    template <template <typename...> typename C, std::ranges::input_range R, typename... Args>
    void to(R&&, Args&&...) = delete;

    template <typename C, typename... Args>
        requires (std::move_constructible<Args> && ...)
    constexpr auto to(Args&&... args) {
        return __ranges::partial_to_fn<C, Args...>(std::forward<Args>(args)...);
    }

    template <template <typename...> typename C, typename... Args>
        requires (std::move_constructible<Args> && ...)
    constexpr auto to(Args&&... args) {
        return __ranges::partial_template_to_fn<C, Args...>(std::forward<Args>(args)...);
    }

    // created a disambiguation overload for deferring the construction
    // especially when the first argument is input_range
    template <typename C, typename... Args>
        requires (std::move_constructible<Args> && ...)
    constexpr auto to(std::in_place_t, Args&&... args) {
        return __ranges::partial_to_fn<C, Args...>(std::forward<Args>(args)...);
    }

    template <template <typename...> typename C, typename... Args>
        requires (std::move_constructible<Args> && ...)
    constexpr auto to(std::in_place_t, Args&&... args) {
        return __ranges::partial_template_to_fn<C, Args...>(std::forward<Args>(args)...);
    }

} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_TO_HPP
