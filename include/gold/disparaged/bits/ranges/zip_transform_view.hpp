// <gold/bits/ranges/zip_transform_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_ZIP_TRANSFORM_VIEW_HPP
#define __GOLD_BITS_RANGES_ZIP_TRANSFORM_VIEW_HPP

#include <gold/bits/ranges/zip_view.hpp>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::zip_transform_iter_cat_base
        template <bool Present, bool Const, typename F, typename... Vs>
        struct zip_transform_iter_cat_base {};

        template <bool Const, typename F, typename... Vs>
        struct zip_transform_iter_cat_base<true, Const, F, Vs...> {
          private:
            using s_invoke_result_ = std::invoke_result_t<
                __ranges::maybe_const_t<Const, F>&,
                ranges::range_ref_t<__ranges::maybe_const_t<Const, Vs>>...
            >;

            template <typename V>
            using s_inner_cat_ = std::iterator_traits<
                ranges::iterator_t<
                    __ranges::maybe_const_t<Const, V>
                >
            >::iterator_category;

            static consteval auto s_iter_cat_() {
                if constexpr (!std::is_lvalue_reference_v<s_invoke_result_>)
                    return std::input_iterator_tag();
                else if ((std::derived_from<s_inner_cat_<Vs>, std::random_access_iterator_tag> && ...))
                    return std::random_access_iterator_tag();
                else if ((std::derived_from<s_inner_cat_<Vs>, std::bidirectional_iterator_tag> && ...))
                    return std::bidirectional_iterator_tag();
                else if ((std::derived_from<s_inner_cat_<Vs>, std::forward_iterator_tag> && ...))
                    return std::forward_iterator_tag();
                else
                    return std::input_iterator_tag();
            }

          public:
            using iterator_category = decltype(s_iter_cat_());
        };

    } // namespace __ranges

    /// ranges::zip_transform_view
    template <std::copy_constructible F, std::ranges::input_range... Vs>
        requires (std::ranges::view<Vs> && ...) && (sizeof...(Vs) > 0)
              && std::is_object_v<F>
              && std::regular_invocable<F&, ranges::range_ref_t<Vs>...>
              && __ranges::can_reference<
                std::invoke_result_t<F&, ranges::range_ref_t<Vs>...>
              >
    class zip_transform_view : public std::ranges::view_interface<zip_transform_view<F, Vs...>> {
      private:
        [[no_unique_address]] __ranges::box<F> m_fun_;
        zip_view<Vs...>  m_zip_;

        using InnerView = zip_view<Vs...>;

        template <bool Const>
        using ziperator = ranges::iterator_t<__ranges::maybe_const_t<Const, InnerView>>;

        template <bool Const>
        using zentinel = ranges::sentinel_t<__ranges::maybe_const_t<Const, InnerView>>;

        template <bool Const>
        class iterator
        : public __ranges::zip_transform_iter_cat_base<
            std::ranges::forward_range<__ranges::maybe_const_t<Const, InnerView>>,
            Const, F, Vs...
          > {
          private:
            using Parent = __ranges::maybe_const_t<Const, zip_transform_view>;
            using Base   = __ranges::maybe_const_t<Const, InnerView>;

            Parent* m_parent_ = nullptr;
            ziperator<Const> m_inner_;

            constexpr iterator(Parent& parent, ziperator<Const> inner)
            : m_parent_(std::addressof(parent)), m_inner_(std::move(inner)) {}

            friend zip_transform_view;

          public:
            // iterator_category defined in __ranges::zip_transform_iter_cat_base
            using iterator_concept = typename ziperator<Const>::iterator_concept;
            using value_type = std::remove_cvref_t<
                std::invoke_result_t<
                    __ranges::maybe_const_t<Const, F>&,
                    ranges::range_ref_t<__ranges::maybe_const_t<Const, Vs>> ...
                >
            >;
            using difference_type = ranges::range_diff_t<Base>;

            constexpr iterator() = default;

            constexpr iterator(iterator<!Const> other)
                requires Const
                      && std::convertible_to<ziperator<false>, ziperator<Const>>
            : m_parent_(std::move(other.m_parent_)),
              m_inner_(std::move(other.m_inner_)) {}

            // TODO: conditional noexcept
            constexpr decltype(auto) operator*() const {
                return tuples::apply([&](const auto&... iters) -> decltype(auto) {
                    return gold::invoke(*m_parent_->m_fun_, *iters...);
                }, m_inner_.m_current_);
            }

            constexpr iterator& operator++() {
                ++m_inner_;
                return *this;
            }

            constexpr void operator++(int) {
                ++*this;
            }

            constexpr iterator operator++(int) requires std::ranges::forward_range<Base> {
                auto temp = *this;
                ++*this;
                return temp;
            }

            constexpr iterator& operator--() requires std::ranges::bidirectional_range<Base> {
                --m_inner_;
                return *this;
            }

            constexpr iterator operator--(int) requires std::ranges::bidirectional_range<Base> {
                auto temp = *this;
                --*this;
                return temp;
            }

            constexpr iterator& operator+=(difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                m_inner_ += n;
                return *this;
            }

            constexpr iterator& operator-=(difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                m_inner_ -= n;
                return *this;
            }

            constexpr decltype(auto) operator[](difference_type n) const
                requires std::ranges::random_access_range<Base>
            {
                return tuples::apply([&]<typename... Iters>(const Iters&... iters) -> decltype(auto) {
                    return gold::invoke(*m_parent_->m_fun_, iters[gold::iter_diff_t<Iters>(n)]...);
                }, m_inner_.m_current_);
            }

            friend constexpr bool operator==(const iterator& lhs, const iterator& rhs)
                requires std::equality_comparable<ziperator<Const>>
            {
                return lhs.m_inner_ == rhs.m_inner_;
            }

            friend constexpr auto operator<=>(const iterator& lhs, const iterator& rhs)
                requires std::ranges::random_access_range<Base>
            {
                return lhs.m_inner <=> rhs.m_inner_;
            }

            friend constexpr iterator operator+(const iterator& iter, difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                return iterator { *iter.m_parent_, iter.m_inner + n };
            }

            friend constexpr iterator operator+(difference_type n, const iterator& iter)
                requires std::ranges::random_access_range<Base>
            {
                return iter + n;
            }

            friend constexpr iterator operator-(const iterator& iter, difference_type n)
                requires std::ranges::random_access_range<Base>
            {
                return iterator { *iter.m_parent_, iter.m_inner - n };
            }

            friend constexpr difference_type operator-(const iterator& lhs, const iterator& rhs)
                requires std::sized_sentinel_for<ziperator<Const>, ziperator<Const>>
            {
                return lhs.m_inner_ - rhs.m_inner_;
            }

        };

        template <bool Const>
        class sentinel {
          private:
            zentinel<Const> m_inner_;

            constexpr explicit sentinel(zentinel<Const> inner)
            : m_inner_(inner) {}

            friend zip_transform_view;

          public:
            constexpr sentinel() = default;

            constexpr sentinel(sentinel<!Const> other)
                requires Const
                      && std::convertible_to<zentinel<false>, zentinel<Const>>
            : m_inner_(std::move(other.m_inner_)) {}

            template <bool OtherConst>
                requires std::sentinel_for<zentinel<Const>, ziperator<OtherConst>>
            friend constexpr bool operator==(const iterator<OtherConst>& iter, const sentinel& sent) {
                return iter.m_inner_ == sent.m_inner_;
            }

            template <bool OtherConst>
                requires std::sized_sentinel_for<zentinel<Const>, ziperator<OtherConst>>
            friend constexpr ranges::range_diff_t<__ranges::maybe_const_t<OtherConst, InnerView>>
            operator-(const iterator<OtherConst>& iter, const sentinel& sent) {
                return iter.m_inner_ - sent.m_inner_;
            }

            template <bool OtherConst>
                requires std::sized_sentinel_for<zentinel<Const>, ziperator<OtherConst>>
            friend constexpr ranges::range_diff_t<__ranges::maybe_const_t<OtherConst, InnerView>>
            operator-(const sentinel& sent, const iterator<OtherConst>& iter) {
                return sent.m_inner_ - iter.m_inner_;
            }

        };

      public:
        constexpr zip_transform_view() = default;

        constexpr explicit zip_transform_view(F fun, Vs... vs)
        : m_fun_(std::move(fun)), m_zip_(std::move(vs)...) {}

        constexpr auto begin() {
            return iterator<false>{*this, m_zip_.begin()};
        }

        constexpr auto begin() const
            requires std::ranges::range<const InnerView>
                  && std::regular_invocable<const F&, ranges::range_ref_t<const Vs>...>
        {
            return iterator<true>{*this, m_zip_.begin()};
        }

        constexpr auto end() {
            if constexpr (std::ranges::common_range<InnerView>)
                return iterator<false>{*this, m_zip_.end()};
            else
                return sentinel<false>{*this, m_zip_.end()};
        }

        constexpr auto end() const
            requires std::ranges::range<const InnerView>
                  && std::regular_invocable<const F&, ranges::range_ref_t<const Vs>...>
        {
            if constexpr (std::ranges::common_range<const InnerView>)
                return iterator<true>{*this, m_zip_.end()};
            else
                return sentinel<true>{*this, m_zip_.end()};
        }

        constexpr auto size() requires std::ranges::sized_range<InnerView> {
            return m_zip_.size();
        }

        constexpr auto size() const requires std::ranges::sized_range<const InnerView> {
            return m_zip_.size();
        }

    };

    template <typename F, typename... Rs>
    zip_transform_view(F, Rs&&...) -> zip_transform_view<F, std::views::all_t<Rs>...>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_zip_transform_view
            template <typename F, typename... Rs>
            concept can_zip_transform_view = requires {
                ranges::zip_transform_view { std::declval<F>(), std::declval<Rs>() ... };
            };

            /// ranges::views::__views::zip_transform_fn
            struct zip_transform_fn {

                template <typename F, typename Fd = std::decay_t<F>>
                    requires std::copy_constructible<Fd>
                          && std::regular_invocable<Fd&>
                          || std::is_object_v<std::decay_t<
                            std::invoke_result_t<Fd&>
                          >>
                [[nodiscard]] constexpr auto operator()(F&&) const noexcept {
                    return std::views::empty<std::decay_t<std::invoke_result_t<Fd&>>>;
                }

                // TODO: add conditional noexcept
                template <typename F, std::ranges::viewable_range... Rs>
                    requires (sizeof...(Rs) > 0)
                          && can_zip_transform_view<F, Rs...>
                [[nodiscard]] constexpr auto operator()(F&& f, Rs&&... rs) const {
                    return zip_transform_view { std::forward<F>(f), std::forward<Rs>(rs) ... };
                }

            };

        } // namespace __views

        /// ranges::views::zip_transform
        inline constexpr __views::zip_transform_fn zip_transform {};

    } // namespace views

} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_ZIP_TRANSFORM_VIEW_HPP
