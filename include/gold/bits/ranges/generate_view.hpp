// <gold/bits/ranges/generate_view.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RANGES_GENERATE_VIEW_HPP
#define __GOLD_BITS_RANGES_GENERATE_VIEW_HPP

#include <gold/bits/casts.hpp>
#include <gold/bits/functional/invoke.hpp>
#include <gold/bits/ranges/view_util.hpp>
#include <gold/bits/type_traits/conditional.hpp>

namespace gold::ranges {

    namespace __ranges {

        /// ranges::__ranges::nullary_invocable
        template <typename T>
        concept nullary_invocable = std::move_constructible<T>
            && std::regular_invocable<T&>
            && !std::is_void_v<std::invoke_result_t<T>>
            && std::movable<std::invoke_result_t<T>>
            && std::default_initializable<std::invoke_result_t<T>>;

    } // namespace __ranges

    /// ranges::generate_view
    template <__ranges::nullary_invocable F, std::semiregular Bound = std::unreachable_sentinel_t>
        requires (__ranges::integer_like<Bound> || std::same_as<Bound, std::unreachable_sentinel_t>)
    class generate_view : public std::ranges::view_interface<generate_view<F>> {
      private:
        using Val = std::invoke_result_t<F>;
        inline static constexpr bool Bounded = !std::same_as<Bound, std::unreachable_sentinel_t>;

        [[no_unique_address]] __ranges::box<F> m_fun_;
        [[no_unique_address]] Bound m_bound_ = Bound();
        [[no_unique_address]] Val m_val_ = Val();

        friend class iterator;

        struct sentinel {};

        class iterator {
          private:
            friend generate_view;
            friend sentinel;

            generate_view* m_parent_;

            constexpr explicit iterator(generate_view* parent) noexcept
            : m_parent_(parent) {}

            constexpr Bound mf_bound_() const noexcept { return m_parent_->m_bound_; }

          public:
            using iterator_concept = std::input_iterator_tag;
            using difference_type  = gold::conditional_t<Bounded, Bound, std::ptrdiff_t>;
            using value_type       = Val;

            constexpr iterator& operator++() {
                if constexpr (Bounded)
                    --m_parent_->m_bound_;
                m_parent_->m_val_ = gold::invoke(*m_parent_->m_fun_);
                return *this;
            }

            constexpr void operator++(int) { ++*this; }

            constexpr Val& operator*() const { return m_parent_->m_val_; }

            friend constexpr bool operator==(const iterator& iter, sentinel) noexcept requires Bounded {
                return iter.mf_bound_() == 0;
            }
        };

      public:
        constexpr generate_view() requires std::default_initializable<F> = default;

        constexpr explicit generate_view(F fun, Bound bound = Bound())
        : m_fun_(std::move(fun)), m_bound_(bound) {}

        // undefined behavior if begin() is called more than once!
        constexpr auto begin() {
            m_val_ = gold::invoke(*m_fun_);
            return iterator{this};
        }

        static constexpr sentinel end() noexcept requires Bounded {
            return {};
        }

        static constexpr std::unreachable_sentinel_t end() noexcept {
            return std::unreachable_sentinel;
        }

        constexpr auto size() const requires Bounded {
            return gold::to_unsigned(m_bound_);
        }

    };

    template <typename F, typename Bound>
    generate_view(F, Bound) -> generate_view<F, Bound>;

    namespace views {

        namespace __views {

            /// ranges::views::__views::can_generate_view
            template <typename F, typename U = void>
            concept can_generate_view = [] {
                if constexpr (std::is_void_v<U>)
                    return requires { generate_view { std::declval<F>() }; };
                else
                    return requires { generate_view { std::declval<F>(), std::declval<U>() }; };
            }();

            /// ranges::views::__views::generate_fn
            struct generate_fn {

                template <typename F>
                    requires can_generate_view<F>
                [[nodiscard]] constexpr auto operator()(F&& f) const noexcept {
                    return generate_view { std::forward<F>(f) };
                }

                template <typename F, typename U>
                    requires can_generate_view<F, U>
                [[nodiscard]] constexpr auto operator()(F&& f, U&& u) const noexcept {
                    return generate_view { std::forward<F>(f), std::forward<U>(u) };
                }

            };

        } // namespace __views

        /// ranges::views::generate
        inline constexpr __views::generate_fn generate {};

    } // namespace views

} // namespace gold::ranges

#endif // __GOLD_BITS_RANGES_GENERATE_VIEW_HPP
