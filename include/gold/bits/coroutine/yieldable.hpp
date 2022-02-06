// <gold/bits/coroutine/yieldable.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_COROUTINE_YIELDABLE_HPP
#define __GOLD_BITS_COROUTINE_YIELDABLE_HPP

#include <concepts>
#include <bits/ranges_util.h>

namespace gold {

    /// yieldable [[fwd decl]]
    template <typename> class yieldable;

    namespace __detail {

        template <typename T>
        struct is_yieldable_ : std::false_type {};

        template <typename T>
        struct is_yieldable_<yieldable<T>> : std::true_type {};

        template <typename T>
        struct is_yieldable_<const T> : is_yieldable_<T> {};

        template <typename T>
        struct is_yieldable_<T&> : is_yieldable_<T> {};

        template <typename T>
        struct is_yieldable_<T&&> : is_yieldable_<T> {};

        template <typename T>
        inline constexpr bool is_yieldable_v_ = is_yieldable_<T>::value;

    } // namespace __detail

    template <std::movable T> requires (!__detail::is_yieldable_v_<T>)
    class generator;

    namespace __detail {

        template <typename>
        struct is_generator_ : std::false_type {};

        template <typename T>
        struct is_generator_<generator<T>> : std::true_type {};

        template <typename T>
        inline constexpr bool is_generator_v_ = is_generator_<T>::value;

    } // namespace __detail

    /// yieldable
    template <typename T>
    class yieldable {
      public:
        using range_type    = generator<T>;
        using value_type    = typename range_type::value_type;
        using iterator      = typename range_type::iterator;
        using sentinel_type = typename range_type::sentinel_type;

      private:
        range_type base_;

        template <std::ranges::input_range R>
        static range_type generate_for_(R&& range_) {
            for (const auto& elem_ : std::forward<R>(range_))
                co_yield elem_;
        } // generate_for_

      public:
        template <typename R>
            requires __detail::is_generator_v_<R>
        constexpr yieldable(R base) noexcept
        : base_(std::move(base)) {}

        template <std::ranges::input_range R>
            requires (!__detail::is_generator_v_<R>)
        constexpr yieldable(R&& base) noexcept
        : base_(std::move(generate_for_(std::forward<R>(base)))) {}

        constexpr auto get_generator() noexcept { return std::move(base_); }
        constexpr auto begin() noexcept(noexcept(base_.begin())) { return base_.begin(); }
        constexpr auto end() noexcept(noexcept(base_.end())) { return base_.end(); }
        constexpr auto subrange() { return std::ranges::subrange(this->begin(), this->end()); }

    };

    /// deduction guides for yieldable
    template <std::ranges::input_range R>
    yieldable(R) -> yieldable<std::ranges::range_value_t<R>>;


    /// to_yieldable_fn - a type usable for yieldable with pipe syntax
    struct to_yieldable_fn {

        template <std::ranges::input_range R>
        constexpr auto operator()(R&& r) const {
            return yieldable<std::ranges::range_value_t<R>>{std::forward<R>(r)};
        }

        template <std::ranges::input_range R>
        friend constexpr auto operator|(R&& r, to_yieldable_fn) {
            return yieldable<std::ranges::range_value_t<R>>{std::forward<R>(r)};
        }
    };

    /// to_yieldable
    inline constexpr to_yieldable_fn to_yieldable {};

} // namespace gold

namespace std::ranges {

    template <typename T>
    inline constexpr bool enable_view<gold::yieldable<T>> = true;

    template <typename T>
    inline constexpr bool enable_borrowed_range<gold::yieldable<T>> = true;

    template <typename T>
    inline constexpr bool enable_view<gold::generator<T>> = true;

} // namespace std::ranges

#endif // __GOLD_BITS_COROUTINE_YIELDABLE_HPP
