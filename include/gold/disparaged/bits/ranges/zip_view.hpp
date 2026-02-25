// <gold/bits/ranges/zip_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_ZIP_VIEW_HPP
#define __GOLD_BITS_RANGES_ZIP_VIEW_HPP

#include <bits/stl_function.h>
#include <bits/views_empty.h>
#include <gold/bits/algo/min_max.hpp>
#include <gold/bits/tuples/apply.hpp>
#include <gold/bits/tuples/apply_each.hpp>
#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/pack>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::zip_is_common
        template <typename... Rs>
        concept zip_is_common =
            (sizeof...(Rs) == 1 && (std::ranges::common_range<Rs> && ...))
         || (!(std::ranges::bidirectional_range<Rs> && ...) && (std::ranges::common_range<Rs> && ...))
         || ((std::ranges::random_access_range<Rs> && ...) && (std::ranges::sized_range<Rs> && ...));

        /// ranges::__ranges::tuple_or_pair_impl
        template <bool IsPair>
        struct tuple_or_pair_impl {
            template <typename T, typename U>
            using type = std::pair<T, U>;
        };

        template <>
        struct tuple_or_pair_impl<false> {
            template <typename... Ts>
            using type = std::tuple<Ts...>;
        };

        /// ranges::__ranges::tuple_or_pair
        template <typename... Ts>
        using tuple_or_pair = tuple_or_pair_impl<sizeof...(Ts) == 2>::template type<Ts...>;

        /// ranges::__ranges::to_tuple_or_pair
        template <typename... Ts>
        constexpr auto to_tuple_or_pair(std::tuple<Ts...>&& tup) {
            if constexpr (sizeof...(Ts) == 2)
                return std::pair<Ts...>(
                    std::forward<gold::pack_element_at<0, Ts...>>(std::get<0>(tup)),
                    std::forward<gold::pack_element_at<1, Ts...>>(std::get<1>(tup))
                );
            else
                return tup;
        }

         /// ranges::__ranges::all_random_access
         template <bool Const, typename... Vs>
         concept all_random_access =
            (std::ranges::random_access_range<__ranges::maybe_const_t<Const, Vs>> && ...);

         /// ranges::__ranges::all_bidirectional
         template <bool Const, typename... Vs>
         concept all_bidirectional =
            (std::ranges::bidirectional_range<__ranges::maybe_const_t<Const, Vs>> && ...);

         /// ranges::__ranges::all_forward
         template <bool Const, typename... Vs>
         concept all_forward =
            (std::ranges::forward_range<__ranges::maybe_const_t<Const, Vs>> && ...);

        /// ranges::__ranges::abs
        constexpr auto abs(auto n) noexcept { return n < 0 ? -n : n; }

        /// ranges::__ranges::zip_iter_cat_base
        template <bool Present>
        struct zip_iter_cat_base {};

        template <>
        struct zip_iter_cat_base<true> {
            using iterator_category = std::input_iterator_tag;
        };

    } // namespace __ranges

    namespace views::__views {

        /// ranges::views::__views::elements_fn [fwd]
        template <std::size_t>
        struct elements_fn;

    } // namespace views::__views

    /// ranges::zip_view
    template <std::ranges::input_range... Vs>
        requires (std::ranges::view<Vs> && ...) && (sizeof...(Vs) > 0)
    class zip_view : public std::ranges::view_interface<zip_view<Vs...>> {
      private:
        template <std::size_t N>
        friend struct views::__views::elements_fn;

        __ranges::tuple_or_pair<Vs...> m_views_;

        template <bool Const>
        class iterator
        : public __ranges::zip_iter_cat_base<__ranges::all_forward<Const, Vs...>> {
          private:
            template <typename V>
            using maybe_iterator_const_t = ranges::iterator_t<__ranges::maybe_const_t<Const, V>>;

            using member_type = __ranges::tuple_or_pair<maybe_iterator_const_t<Vs>...>;

            member_type m_current_;

            constexpr explicit iterator(member_type other)
            : m_current_(std::move(other)) {}

            static consteval auto s_iter_concept_() {
                if constexpr (__ranges::all_random_access<Const, Vs...>)
                    return std::random_access_iterator_tag();
                else if constexpr (__ranges::all_bidirectional<Const, Vs...>)
                    return std::bidirectional_iterator_tag();
                else if constexpr (__ranges::all_forward<Const, Vs...>)
                    return std::forward_iterator_tag();
                else
                    return std::input_iterator_tag();
            }

            inline static constexpr bool s_all_forward_ = __ranges::all_forward<Const, Vs...>;
            inline static constexpr bool s_all_bidirectional_ = __ranges::all_bidirectional<Const, Vs...>;
            inline static constexpr bool s_all_random_access_ = __ranges::all_random_access<Const, Vs...>;

            friend zip_view;

            template <std::copy_constructible F, std::ranges::input_range... Os>
                requires (std::ranges::view<Os> && ...) && (sizeof...(Os) > 0)
                      && std::is_object_v<F>
                      && std::regular_invocable<
                        F&, ranges::range_ref_t<Os>...
                      >
                      && __ranges::can_reference<
                        std::invoke_result_t<F&, ranges::range_ref_t<Os>...>
                      >
            friend class zip_transform_view;

            static consteval bool s_noexcept_iter_move_() {
                return (noexcept(std::ranges::iter_move(std::declval<const maybe_iterator_const_t<Vs>&>())) && ...)
                    && (std::is_nothrow_move_constructible_v<ranges::range_rref_t<__ranges::maybe_const_t<Const, Vs>>> && ...);
            }

            static consteval bool s_noexcept_iter_swap_() {
                return (noexcept(std::ranges::iter_swap(std::declval<const maybe_iterator_const_t<Vs>&>(),
                                                        std::declval<const maybe_iterator_const_t<Vs>&>())) && ...);
            }

          public:
            // iterator_category may be defined in __ranges::zip_iter_cat_base
            using iterator_concept = decltype(s_iter_concept_());
            using value_type = __ranges::tuple_or_pair<ranges::range_val_t<__ranges::maybe_const_t<Const, Vs>>...>;
            using difference_type = std::common_type_t<ranges::range_diff_t<__ranges::maybe_const_t<Const, Vs>>...>;

            constexpr iterator() = default;

            constexpr iterator(iterator<!Const> other)
                requires Const
                     && (std::convertible_to<ranges::iterator_t<Vs>, maybe_iterator_const_t<Vs>> && ...)
            : m_current_(std::move(other.m_current_)) {}

            constexpr auto operator*() const {
                return __ranges::to_tuple_or_pair(tuples::apply_each([](auto& iter) -> decltype(auto) { return *iter; }, m_current_));
            }

            constexpr iterator& operator++() {
                tuples::apply_each([](auto& iter) { ++iter; }, m_current_);
                return *this;
            }

            constexpr void operator++(int) { ++*this; }

            constexpr iterator operator++(int) requires s_all_forward_ {
                auto temp = *this;
                ++*this;
                return temp;
            }

            constexpr iterator& operator--() requires s_all_bidirectional_ {
                tuples::apply_each([](auto& iter) { --iter; }, m_current_);
                return *this;
            }

            constexpr iterator& operator+=(difference_type n) requires s_all_random_access_ {
                tuples::apply_each([&]<typename Iter>(Iter& iter) {
                    iter += gold::iter_diff_t<Iter>(n);
                }, m_current_);
                return *this;
            }

            constexpr iterator& operator-=(difference_type n) requires s_all_random_access_ {
                tuples::apply_each([&]<typename Iter>(Iter& iter) {
                    iter -= gold::iter_diff_t<Iter>(n);
                }, m_current_);
                return *this;
            }

            constexpr auto operator[](difference_type n) requires s_all_random_access_ {
                return __ranges::to_tuple_or_pair(tuples::apply_each([&]<typename Iter>(Iter& iter) {
                    return iter[gold::iter_diff_t<Iter>(n)];
                }, m_current_));
            }

            friend constexpr bool operator==(const iterator& lhs, const iterator& rhs)
                requires (std::equality_comparable<maybe_iterator_const_t<Vs>> && ...)
            {
                if constexpr (s_all_bidirectional_)
                    return lhs.m_current_ == rhs.m_current_;
                else
                    return tuples::apply([](auto... bools) {
                        return (bools || ...);
                    }, tuples::apply_each(std::ranges::equal_to{}, lhs.m_current_, rhs.m_current_));
            }

            friend constexpr auto operator<=>(const iterator& lhs, const iterator& rhs) requires s_all_random_access_ {
                return lhs.m_current_ <=> rhs.m_current_;
            }

            friend constexpr iterator operator+(const iterator& iter, difference_type n)
                requires s_all_random_access_
            {
                auto r = iter;
                r += n;
                return r;
            }


            friend constexpr iterator operator+(difference_type n, const iterator& iter)
                requires s_all_random_access_
            {
                return iter + n;
            }

            friend constexpr iterator operator-(const iterator& iter, difference_type n)
                requires s_all_random_access_
            {
                auto r = iter;
                r -= n;
                return r;
            }

            friend constexpr difference_type operator-(const iterator& lhs, const iterator& rhs)
                requires (std::sized_sentinel_for<maybe_iterator_const_t<Vs>, maybe_iterator_const_t<Vs>> && ...)
            {
                return tuples::apply([](auto... ds) {
                    return __algo::min_element(
                        { difference_type(ds)... },
                        [](auto a, auto b) { return __ranges::abs(a) < __ranges::abs(b); }
                    );
                }, tuples::apply_each(std::minus<>{}, lhs.m_current_, rhs.m_current_));
            }

            friend constexpr auto iter_move(const iterator& iter) noexcept(s_noexcept_iter_move_()) {
                return __ranges::to_tuple_or_pair(tuples::apply_each(std::ranges::iter_move, iter.m_current_));
            }

            friend constexpr void iter_swap(const iterator& lhs, const iterator& rhs)
                noexcept(s_noexcept_iter_swap_())
                requires (std::indirectly_swappable<maybe_iterator_const_t<Vs>> && ...)
            {
                tuples::apply_each(std::ranges::swap, lhs.m_current_, rhs.m_current_);
            }


        };

        template <bool Const>
        class sentinel {
          private:
            template <typename V>
            using maybe_sentinel_const_t = ranges::sentinel_t<__ranges::maybe_const_t<Const, V>>;

            using member_type = __ranges::tuple_or_pair<maybe_sentinel_const_t<Vs>...>;

            member_type m_sent_;

            friend zip_view;

            constexpr explicit sentinel(member_type sent)
            : m_sent_(sent) {}

          public:
            constexpr sentinel() = default;

            constexpr sentinel(sentinel<!Const> other)
                requires Const
                      && (std::convertible_to<ranges::sentinel_t<Vs>, maybe_sentinel_const_t<Vs>> && ...)
            : m_sent_(std::move(other.m_sent_)) {}

            template <bool OtherConst>
                requires (std::sentinel_for<
                            maybe_sentinel_const_t<Vs>,
                            ranges::iterator_t<__ranges::maybe_const_t<OtherConst, Vs>>> && ...)
            friend constexpr bool operator==(const iterator<OtherConst>& iter, const sentinel& sent) {
                return tuples::apply([](auto... bools) {
                    return (bools || ...);
                }, tuples::apply_each(std::ranges::equal_to{}, iter.m_current_, sent.m_sent_));
            }

            template <bool OtherConst>
                requires (std::sized_sentinel_for<
                            maybe_sentinel_const_t<Vs>,
                            ranges::iterator_t<__ranges::maybe_const_t<OtherConst, Vs>>> && ...)
            friend constexpr auto operator-(const iterator<OtherConst>& iter, const sentinel& sent)
            -> std::common_type_t<ranges::range_diff_t<__ranges::maybe_const_t<OtherConst, Vs>>...>
            {
                return tuples::apply([](auto... ds) {
                    return __algo::min_element(
                        { difference_type(ds)... },
                        [](auto a, auto b) { return __ranges::abs(a) < __ranges::abs(b); }
                    );
                }, tuples::apply_each(std::minus<>{}, iter.m_current_, sent.m_sent_));
            }

            template <bool OtherConst>
                requires (std::sized_sentinel_for<
                            maybe_sentinel_const_t<Vs>,
                            ranges::iterator_t<__ranges::maybe_const_t<OtherConst, Vs>>> && ...)
            friend constexpr auto operator-(const sentinel& sent, const iterator<OtherConst>& iter)
            -> std::common_type_t<ranges::range_diff_t<__ranges::maybe_const_t<OtherConst, Vs>>...>
            {
                return -(iter - sent);
            }

        };

      public:
        constexpr zip_view() = default;

        constexpr explicit zip_view(Vs... vs)
        : m_views_(std::move(vs)...) {}

        constexpr auto begin() requires (!(__ranges::simple_view<Vs> && ...)) {
            return iterator<false>(__ranges::to_tuple_or_pair(tuples::apply_each(std::ranges::begin, m_views_)));
        }

        constexpr auto begin() const requires (std::ranges::range<const Vs> && ...) {
            return iterator<true>(__ranges::to_tuple_or_pair(tuples::apply_each(std::ranges::begin, m_views_)));
        }

        constexpr auto end() requires (!(__ranges::simple_view<Vs> && ...)) {
            if constexpr (!__ranges::zip_is_common<Vs...>)
                return sentinel<false>(__ranges::to_tuple_or_pair(tuples::apply_each(std::ranges::end, m_views_)));
            else if constexpr ((std::ranges::random_access_range<Vs> && ...))
                return begin() + gold::iter_diff_t<iterator<false>>(size());
            else
                return iterator<false>(__ranges::to_tuple_or_pair(tuples::apply_each(std::ranges::end, m_views_)));
        }

        constexpr auto end() const requires (std::ranges::range<const Vs> && ...) {
            if constexpr (!__ranges::zip_is_common<const Vs...>)
                return sentinel<true>(__ranges::to_tuple_or_pair(tuples::apply_each(std::ranges::end, m_views_)));
            else if constexpr ((std::ranges::random_access_range<Vs> && ...))
                return begin() + gold::iter_diff_t<iterator<true>>(size());
            else
                return iterator<true>(__ranges::to_tuple_or_pair(tuples::apply_each(std::ranges::end, m_views_)));
        }

        constexpr auto size() requires (std::ranges::sized_range<Vs> && ...) {
            return tuples::apply([](auto... sizes) {
                using CT = std::make_unsigned_t<std::common_type_t<decltype(sizes)...>>;
                return __algo::min_element({CT(sizes)...});
            }, tuples::apply_each(std::ranges::size, m_views_));
        }
        constexpr auto size() const requires (std::ranges::sized_range<const Vs> && ...) {
            return tuples::apply([](auto... sizes) {
                using CT = std::make_unsigned_t<std::common_type_t<decltype(sizes)...>>;
                return __algo::min_element({CT(sizes)...});
            }, tuples::apply_each(std::ranges::size, m_views_));
        }

    };

    template <typename... Rs>
    zip_view(Rs&&...) -> zip_view<std::views::all_t<Rs>...>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_zip_view
            template <typename... Rs>
            concept can_zip_view = requires {
                ranges::zip_view { std::declval<Rs>() ... };
            };

            /// ranges::views::__views::zip_fn
            struct zip_fn {

                [[nodiscard]] constexpr auto operator()() const noexcept {
                    return std::views::empty<std::tuple<>>;
                }

                // TODO: add conditional noexcept
                template <std::ranges::viewable_range... Rs>
                    requires (sizeof...(Rs) > 0) && can_zip_view<Rs...>
                [[nodiscard]] constexpr auto operator()(Rs&&... rs) const {
                    return zip_view { std::forward<Rs>(rs) ... };
                }

            };

        } // namespace __views

        /// ranges::views::zip
        inline constexpr __views::zip_fn zip {};

    } // namespace views

} // namespace gold::ranges

namespace std::ranges {

    template <typename... Vs>
    inline constexpr bool enable_borrowed_range<gold::ranges::zip_view<Vs...>> =
        (enable_borrowed_range<Vs> && ...);

} // namespace std::ranges

#endif // __GOLD_BITS_RANGES_ZIP_VIEW_HPP
