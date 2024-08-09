// <gold/bits/ranges/concat_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_CONCAT_VIEW_HPP
#define __GOLD_BITS_RANGES_CONCAT_VIEW_HPP

#include <tuple>
#include <variant>
#include <bits/stl_iterator_base_types.h>
#include <bits/ranges_util.h>
#include <gold/bits/algo/with_index.hpp>
#include <gold/bits/ranges/range_primitives.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/bits/sequence/type_seq.hpp>
#include <gold/bits/tuples/apply.hpp>
#include <gold/bits/tuples/apply_each.hpp>
#include <gold/bits/tuples/get.hpp>


namespace gold::ranges {

    // TODO: optimize concat_view
    //                to use simpler algorithms
    //                if there is homogeneous uniformity

    namespace __ranges {

        /// ranges::__ranges::concat_ref_t
        template <typename... Rs>
        using concat_ref_t = std::common_reference_t<std::ranges::range_reference_t<Rs>...>;

        /// ranges::__ranges::concat_val_t
        template <typename... Rs>
        using concat_val_t = std::common_type_t<std::ranges::range_value_t<Rs>...>;

        /// ranges::__ranges::concat_cref_t
        template <typename... Rs>
        using concat_cref_t = std::common_reference_t<std::ranges::range_const_reference_t<Rs>...>;

        /// ranges::__ranges::concat_rref_t
        template <typename... Rs>
        using concat_rref_t = std::common_reference_t<std::ranges::range_rvalue_reference_t<Rs>...>;

        /// ranges::__ranges::concat_diff_t
        template <typename... Rs>
        using concat_diff_t = std::common_type_t<std::ranges::range_difference_t<Rs>...>;

        /// ranges::__ranges::concat_indirectly_readable_impl
        template <typename Ref, typename RRef, typename Iter>
        concept concat_indirectly_readable_impl = requires (const Iter iter) {
            { *iter } -> std::convertible_to<Ref>;
            { std::ranges::iter_move(iter) } -> std::convertible_to<RRef>;
        };

        /// ranges::__ranges::concat_indirectly_readable
        template <typename... Rs>
        concept concat_indirectly_readable =
               std::common_reference_with<concat_ref_t<Rs...>&&, concat_val_t<Rs...>&>
            && std::common_reference_with<concat_ref_t<Rs...>&&, concat_rref_t<Rs...>&&>
            && std::common_reference_with<concat_rref_t<Rs...>&&, const concat_val_t<Rs...>&>
            && (concat_indirectly_readable_impl<concat_ref_t<Rs...>, concat_rref_t<Rs...>, std::ranges::iterator_t<Rs>> && ...);

        /// ranges::__ranges::concatable
        template <typename... Rs>
        concept concatable = requires {
            typename concat_val_t<Rs...>;
            typename concat_ref_t<Rs...>;
            typename concat_rref_t<Rs...>;
        } && concat_indirectly_readable<Rs...>;

        /// ranges::__ranges::all_sized_range
        template <typename... Rs>
        struct all_sized_range {
            static constexpr bool value = (std::ranges::sized_range<Rs> && ...);
        };

        /// ranges::__ranges::concat_is_random_access
        template <typename... Rs>
        concept concat_is_random_access =
            (std::ranges::random_access_range<Rs> && ...) &&
            gold::type_seq_t<Rs...>::template pop_back<>::template unpack<all_sized_range>::value;

        /// ranges::__ranges::constant_time_reversible
        template <typename R>
        concept constant_time_reversible =
               (std::ranges::bidirectional_range<R> && std::ranges::common_range<R>)
            || (std::ranges::sized_range<R> && std::ranges::random_access_range<R>);

        /// ranges::__ranges::are_all_constant_time_reversible
        template <typename... Rs>
        struct are_all_constant_time_reversible {
            inline static constexpr bool value = (constant_time_reversible<Rs> && ...);
        };

        /// ranges::__ranges::concat_is_bidirectional
        template <typename... Rs>
        concept concat_is_bidirectional =
            std::ranges::bidirectional_range<typename gold::type_seq_t<Rs...>::template back<>>
            && gold::type_seq_t<Rs...>::template unpack<are_all_constant_time_reversible>::value;

        /// ranges::__ranges::concat_iter_cat_base
        template <bool Present, bool Const, typename... Rs>
        struct concat_iter_cat_base {};

        template <bool Const, typename... Rs>
        struct concat_iter_cat_base<true, Const, Rs...> {
          private:
            template <typename R>
            using IterCat = std::iterator_traits<
                std::ranges::iterator_t<__ranges::maybe_const_t<Const, R>>
            >::iterator_category;

            using ConcatRef = __ranges::concat_ref_t<__ranges::maybe_const_t<Const, Rs>...>;

            static consteval auto s_iter_cat_() {
                if constexpr (!std::is_lvalue_reference_v<ConcatRef>)
                    return std::input_iterator_tag();
                else if constexpr ((std::derived_from<IterCat<Rs>, std::random_access_iterator_tag> && ...)
                                && __ranges::concat_is_random_access<__ranges::maybe_const_t<Const, Rs>...>)
                    return std::random_access_iterator_tag();
                else if constexpr ((std::derived_from<IterCat<Rs>, std::bidirectional_iterator_tag> && ...)
                                && __ranges::concat_is_bidirectional<__ranges::maybe_const_t<Const, Rs>...>)
                    return std::bidirectional_iterator_tag();
                else if constexpr ((std::derived_from<IterCat<Rs>, std::forward_iterator_tag> && ...))
                    return std::forward_iterator_tag();
                else
                    return std::input_iterator_tag();
            }

          public:
            using iterator_category = decltype(s_iter_cat_());
        };

    } // namespace __ranges

    /// ranges::concat_view
    template <std::ranges::input_range... Vs>
        requires (std::ranges::view<Vs> && ...)
              && (sizeof...(Vs) > 0)
              && __ranges::concatable<Vs...>
    class concat_view : public std::ranges::view_interface<concat_view<Vs...>> {
      private:
        std::tuple<Vs...> m_views_ = std::tuple<Vs...>();

        template <bool Const>
        class iterator
        : public __ranges::concat_iter_cat_base<
            (std::ranges::forward_range<__ranges::maybe_const_t<Const, Vs>> && ...),
            Const, Vs...
        > {
          private:
            static consteval auto s_iter_concept_() {
                if constexpr (__ranges::concat_is_random_access<__ranges::maybe_const_t<Const, Vs>...>)
                    return std::random_access_iterator_tag();
                else if constexpr (__ranges::concat_is_bidirectional<__ranges::maybe_const_t<Const, Vs>...>)
                    return std::bidirectional_iterator_tag();
                else if constexpr ((std::ranges::forward_range<__ranges::maybe_const_t<Const, Vs>> && ...))
                    return std::forward_iterator_tag();
                else
                    return std::input_iterator_tag();
            }

            using rvalue_reference = __ranges::concat_rref_t<__ranges::maybe_const_t<Const, Vs>...>;

          public:
            using value_type = __ranges::concat_val_t<__ranges::maybe_const_t<Const, Vs>...>;
            using reference  = __ranges::concat_ref_t<__ranges::maybe_const_t<Const, Vs>...>;
            using difference_type = __ranges::concat_diff_t<__ranges::maybe_const_t<Const, Vs>...>;
            using iterator_concept = decltype(s_iter_concept_());
            // iterator_category defined in __ranges::concat_iter_cat_base

          private:
            friend iterator<!Const>;
            friend concat_view;

            using BaseIter = std::variant<
                std::ranges::iterator_t<__ranges::maybe_const_t<Const, Vs>>...
            >;
            using Parent   = __ranges::maybe_const_t<Const, concat_view>;

            Parent*  m_parent_ = nullptr;
            BaseIter m_iter_   = BaseIter();

            constexpr decltype(auto) mf_get_parent_views_() const {
                return m_parent_->m_views_;
            }

            template <std::size_t N>
            constexpr void mf_satisfy_() {
                if constexpr (N != (sizeof...(Vs) - 1)) {
                    if (std::get<N>(m_iter_) == std::ranges::end(tuples::get<N>(m_parent_->m_views_))) {
                        m_iter_.template emplace<N + 1>(std::ranges::begin(tuples::get<N + 1>(m_parent_->m_views_)));
                        mf_satisfy_<N + 1>();
                    }
                }
            }

            template <std::size_t N>
            constexpr void mf_prev_() {
                if constexpr (N == 0) {
                    --std::get<0>(m_iter_);
                } else {
                    if (std::get<N>(m_iter_) == std::ranges::begin(tuples::get<N>(m_parent_->m_views_))) {
                        using PrevView = __ranges::maybe_const_t<Const, typename gold::type_seq_t<Vs...>::template at<N - 1>>;
                        if constexpr (std::ranges::common_range<PrevView>) {
                            m_iter_.template emplace<N - 1>(std::ranges::end(tuples::get<N - 1>(m_parent_->m_views_)));
                        } else {
                            m_iter_.template emplace<N - 1>(
                                std::ranges::next(std::ranges::begin(tuples::get<N - 1>(m_parent_->m_views_)),
                                                  std::ranges::size(tuples::get<N - 1>(m_parent_->m_views_)))
                            );
                        }
                        mf_prev_<N - 1>();
                    } else {
                        --std::get<N>(m_iter_);
                    }
                }
            }

            template <std::size_t N>
            constexpr void mf_advance_fwd_(difference_type offset, difference_type steps) {
                if constexpr (N == sizeof...(Vs) - 1) {
                    std::get<N>(m_iter_) += steps;
                } else {
                    auto n_size = std::ranges::size(tuples::get<N>(m_parent_->m_views_));
                    if (offset + steps < static_cast<difference_type>(n_size)) {
                        std::get<N>(m_iter_) += steps;
                    } else {
                        m_iter_.template emplace<N + 1>(std::ranges::begin(tuples::get<N + 1>(m_parent_->m_views_)));
                        mf_advance_fwd_<N + 1>(0, offset + steps - n_size);
                    }
                }
            }

            template <std::size_t N>
            constexpr void mf_advance_bwd_(difference_type offset, difference_type steps) {
                if constexpr (N == 0) {
                    std::get<N>(m_iter_) -= steps;
                } else {
                    if (offset >= steps) {
                        std::get<N>(m_iter_) -= steps;
                    } else {
                        m_iter_.template emplace<N - 1>(std::ranges::begin(tuples::get<N - 1>(m_parent_->m_views_))
                                                      + std::ranges::size(tuples::get<N - 1>(m_parent_->m_views_)));
                        mf_advance_bwd_<N - 1>(
                            static_cast<difference_type>(std::ranges::size(tuples::get<N - 1>(m_parent_->m_views_))),
                            steps - offset
                        );
                    }
                }
            }

            template <typename... Args>
            constexpr explicit iterator(Parent* parent, Args&&... args)
                requires std::constructible_from<BaseIter, Args...>
            : m_parent_(parent), m_iter_(std::forward<Args>(args)...) {}

          public:
            constexpr iterator() = default;

            constexpr iterator(iterator<!Const> other)
                requires Const
                      && (std::convertible_to<std::ranges::iterator_t<Vs>,
                                              std::ranges::iterator_t<__ranges::maybe_const_t<Const, Vs>>> && ...)
            : m_parent_(other.m_parent_), m_iter_(std::move(other.m_iter_)) {}

            constexpr decltype(auto) operator*() const {
                return std::visit([](auto&& iter) -> reference {
                    return *iter;
                }, m_iter_);
            }

            constexpr iterator& operator++() {
                __algo::visit_with_index<sizeof...(Vs)>(m_iter_.index(), [&]<std::size_t I> {
                    ++std::get<I>(m_iter_);
                    mf_satisfy_<I>();
                });
                return *this;
            }

            constexpr void operator++(int) { ++*this; }

            constexpr iterator operator++(int)
                requires (std::ranges::forward_range<__ranges::maybe_const_t<Const, Vs>> && ...)
            {
                auto temp = *this;
                ++*this;
                return temp;
            }

            constexpr iterator& operator--()
                requires __ranges::concat_is_bidirectional<__ranges::maybe_const_t<Const, Vs>...>
            {
                __algo::visit_with_index<sizeof...(Vs)>(m_iter_.index(), [&]<std::size_t I> {
                    mf_prev_<I>();
                });
                return *this;
            }

            constexpr iterator operator--(int)
                requires __ranges::concat_is_bidirectional<__ranges::maybe_const_t<Const, Vs>...>
            {
                auto temp = *this;
                --*this;
                return temp;
            }

            constexpr iterator& operator+=(difference_type n)
                requires __ranges::concat_is_random_access<__ranges::maybe_const_t<Const, Vs>...>
            {
                __algo::visit_with_index<sizeof...(Vs)>(m_iter_.index(), [&]<std::size_t I> {
                    if (n > 0)
                        mf_advance_fwd_<I>(std::get<I>(m_iter_) - std::ranges::begin(tuples::get<I>(m_parent_->m_views_)), n);
                    else if (n < 0)
                        mf_advance_bwd_<I>(std::get<I>(m_iter_) - std::ranges::begin(tuples::get<I>(m_parent_->m_views_)), -n);
                });
                return *this;
            }

            constexpr iterator& operator-=(difference_type n)
                requires __ranges::concat_is_random_access<__ranges::maybe_const_t<Const, Vs>...>
            {
                return *this += -n;
            }

            constexpr decltype(auto) operator[](difference_type n) const
                requires __ranges::concat_is_random_access<__ranges::maybe_const_t<Const, Vs>...>
            {
                return *(*this + n);
            }

            friend constexpr bool operator==(const iterator& lhs, const iterator& rhs)
                requires (std::equality_comparable<std::ranges::iterator_t<__ranges::maybe_const_t<Const, Vs>>> && ...)
            {
                return lhs.m_iter_ == rhs.m_iter_;
            }

            friend constexpr bool operator==(const iterator& iter, std::default_sentinel_t) {
                constexpr auto last_index = sizeof...(Vs) - 1;
                return iter.m_iter_.index() == last_index
                    && std::get<last_index>(iter.m_iter_) == std::ranges::end(tuples::get<last_index>(iter.m_parent->m_views_));
            }

            friend constexpr bool operator<(const iterator& lhs, const iterator& rhs)
                requires (std::ranges::random_access_range<__ranges::maybe_const_t<Const, Vs>> && ...)
            {
                return lhs.m_iter_ < rhs.m_iter_;
            }

            friend constexpr bool operator<=(const iterator& lhs, const iterator& rhs)
                requires (std::ranges::random_access_range<__ranges::maybe_const_t<Const, Vs>> && ...)
            {
                return lhs.m_iter_ <= rhs.m_iter_;
            }

            friend constexpr bool operator>(const iterator& lhs, const iterator& rhs)
                requires (std::ranges::random_access_range<__ranges::maybe_const_t<Const, Vs>> && ...)
            {
                return lhs.m_iter_ > rhs.m_iter_;
            }

            friend constexpr bool operator>=(const iterator& lhs, const iterator& rhs)
                requires (std::ranges::random_access_range<__ranges::maybe_const_t<Const, Vs>> && ...)
            {
                return lhs.m_iter_ >= rhs.m_iter_;
            }


            friend constexpr auto operator<=>(const iterator& lhs, const iterator& rhs)
                requires (std::ranges::random_access_range<__ranges::maybe_const_t<Const, Vs>> && ...)
                      && (std::three_way_comparable<__ranges::maybe_const_t<Const, Vs>> && ...)
            {
                return lhs.m_iter_ <=> rhs.m_iter_;
            }

            friend constexpr iterator operator+(const iterator& iter, difference_type n)
                requires __ranges::concat_is_random_access<__ranges::maybe_const_t<Const, Vs>...>
            {
                return iterator { iter } += n;
            }

            friend constexpr iterator operator+(difference_type n, const iterator& iter)
                requires __ranges::concat_is_random_access<__ranges::maybe_const_t<Const, Vs>...>
            {
                return iter + n;
            }

            friend constexpr iterator operator-(const iterator& iter, difference_type n)
                requires __ranges::concat_is_random_access<__ranges::maybe_const_t<Const, Vs>...>
            {
                return iterator { iter } -= n;
            }

            friend constexpr difference_type operator-(const iterator& iter, const iterator& other_iter)
                requires __ranges::concat_is_random_access<__ranges::maybe_const_t<Const, Vs>...>
            {
                const auto ix = iter.m_iter_.index();
                const auto iy = other_iter.m_iter_.index();
                if (ix > iy) {
                    const auto dy = __algo::visit_with_index<sizeof...(Vs)>(iy, [&]<std::size_t I> -> difference_type {
                        return std::ranges::end(tuples::get<I>(iter.mf_get_parent_views_())) - std::get<I>(other_iter.m_iter_);
                    });
                    const auto dx = __algo::visit_with_index<sizeof...(Vs)>(ix, [&]<std::size_t I> -> difference_type {
                        return std::get<I>(iter.m_iter_) - std::ranges::begin(tuples::get<I>(iter.mf_get_parent_views_()));
                    });
                    const auto s = [&] {
                        difference_type sum = 0;
                        for (difference_type i = iy + 1; i < ix; ++i)
                            __algo::visit_with_index<sizeof...(Vs)>(i, [&]<std::size_t I> {
                                sum += std::ranges::size(tuples::get<I>(iter.mf_get_parent_views_()));
                            });
                        return sum;
                    }();
                    return dy + s + dx;
                } else if (ix < iy) {
                    return -(other_iter - iter);
                } else {
                    return __algo::visit_with_index<sizeof...(Vs)>(ix, [&]<std::size_t I> -> difference_type {
                        return std::get<I>(iter.m_iter_) - std::get<I>(other_iter.m_iter_);
                    });
                }
            }

            friend constexpr difference_type operator-(const iterator& iter, std::default_sentinel_t)
                requires __ranges::concat_is_random_access<__ranges::maybe_const_t<Const, Vs>...>
            {
                const auto ix = iter.m_iter_.index();
                const auto dx = __algo::visit_with_index<sizeof...(Vs)>(ix, [&]<std::size_t I> -> difference_type {
                    return std::ranges::end(tuples::get<I>(iter.mf_get_parent_views_())) - std::get<I>(iter.m_iter_);
                });
                const auto s = [&] {
                    difference_type sum = 0;
                    for (difference_type i = ix + 1; i < sizeof...(Vs); ++i)
                        __algo::visit_with_index<sizeof...(Vs)>(i, [&]<std::size_t I> {
                            sum += std::ranges::size(tuples::get<I>(iter.mf_get_parent_views_()));
                        });
                    return sum;
                }();
                return -(dx + s);
            }

            friend constexpr difference_type operator-(std::default_sentinel_t, const iterator& iter)
                requires __ranges::concat_is_random_access<__ranges::maybe_const_t<Const, Vs>...>
            {
                return -(iter - std::default_sentinel);
            }

            static consteval bool sf_iter_move_noexcept_() {
                return ((
                    std::is_nothrow_invocable_v<
                        decltype(std::ranges::iter_move),
                        const std::ranges::iterator_t<__ranges::maybe_const_t<Const, Vs>>&
                    > &&
                    std::is_nothrow_convertible_v<
                        std::ranges::range_rvalue_reference_t<__ranges::maybe_const_t<Const, Vs>>,
                        rvalue_reference
                    >) && ...
                );
            }

            friend constexpr decltype(auto) iter_move(const iterator& iter)
                noexcept(sf_iter_move_noexcept_())
            {
                return std::visit([](const auto& iter) -> rvalue_reference {
                    return std::ranges::iter_move(iter);
                }, iter.m_iter_);
            }

            static consteval bool sf_iter_swap_noexcept_() {
                return (std::indirectly_swappable<std::ranges::iterator_t<__ranges::maybe_const_t<Const, Vs>>>
                        && ... &&
                        std::swappable_with<__ranges::concat_ref_t<__ranges::maybe_const_t<Const, Vs>...>,
                                            __ranges::concat_ref_t<__ranges::maybe_const_t<Const, Vs>...>>);
            }

            // TODO: add constraints
            friend constexpr void iter_swap(const iterator& lhs, const iterator& rhs)
                noexcept(sf_iter_swap_noexcept_())
            {
                std::visit(std::ranges::iter_swap, lhs.m_iter_, rhs.m_iter_);
            }

        };

      public:
        constexpr concat_view()
            requires (std::default_initializable<Vs> && ...)
        = default;

        constexpr explicit concat_view(Vs... vs)
        : m_views_(std::move(vs)...) {}

        constexpr iterator<false> begin()
            requires (!(__ranges::simple_view<Vs> && ...))
        {
            iterator<false> iter { this, std::in_place_index<0>, std::ranges::begin(tuples::get<0>(m_views_)) };
            iter.template mf_satisfy_<0>();
            return iter;
        }

        constexpr iterator<true> begin() const
            requires (std::ranges::range<const Vs> && ...)
                  && (__ranges::concatable<const Vs...>)
        {
            iterator<true> iter { this, std::in_place_index<0>, std::ranges::begin(tuples::get<0>(m_views_)) };
            iter.template mf_satisfy_<0>();
            return iter;
        }

        constexpr auto end() requires (!(__ranges::simple_view<Vs> && ...)) {
            using LastView = gold::type_seq_t<Vs...>::template back<>;
            if constexpr (std::ranges::common_range<LastView>) {
                constexpr auto N = sizeof...(Vs);
                return iterator<false>{this, std::in_place_index<N - 1>,
                                       std::ranges::end(tuples::get<N - 1>(m_views_))};
            } else {
                return std::default_sentinel;
            }
        }

        constexpr auto end() const requires (std::ranges::range<const Vs> && ...) {
            using LastView = const gold::type_seq_t<Vs...>::template back<>;
            if constexpr (std::ranges::common_range<LastView>) {
                constexpr auto N = sizeof...(Vs);
                return iterator<true>{this, std::in_place_index<N - 1>,
                                       std::ranges::end(tuples::get<N - 1>(m_views_))};
            } else {
                return std::default_sentinel;
            }
        }

        constexpr auto size() requires (std::ranges::sized_range<Vs> && ...) {
            return tuples::apply([](auto... sizes) {
                using CT = std::make_unsigned_t<std::common_type_t<decltype(sizes)...>>;
                return (CT{0} + ... + CT{sizes});
            }, tuples::apply_each(std::ranges::size, m_views_));
        }

        constexpr auto size() const requires (std::ranges::sized_range<const Vs> && ...) {
            return tuples::apply([](auto... sizes) {
                using CT = std::make_unsigned_t<std::common_type_t<decltype(sizes)...>>;
                return (CT{0} + ... + CT{sizes});
            }, tuples::apply_each(std::ranges::size, m_views_));
        }

    };

    template <typename... Rs>
    concat_view(Rs&&...) -> concat_view<std::views::all_t<Rs>...>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_concat_view
            template <typename... Rs>
            concept can_concat_view = requires {
                ranges::concat_view { std::declval<Rs>() ... };
            };

            /// ranges::views::__views::concat_fn
            struct concat_fn {

                // TODO: add conditional noexcept
                template <std::ranges::viewable_range... Rs>
                    requires (sizeof...(Rs) > 1) && can_concat_view<Rs...>
                [[nodiscard]] static constexpr auto operator()(Rs&&... rs) {
                    return concat_view { std::forward<Rs>(rs) ... };
                }

                // TODO: add conditional noexcept

                // TODO: add conditional noexcept
                template <std::ranges::viewable_range R>
                [[nodiscard]] static constexpr auto operator()(R&& r) {
                    return std::views::all(std::forward<R>(r));
                }

            };

        } // namespace __views

        /// ranges::views::concat
        inline constexpr __views::concat_fn concat {};

    } // namespace views

} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_CONCAT_VIEW_HPP
