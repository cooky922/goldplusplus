// <gold/bits/rand_gen.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RAND_GEN_HPP
#define __GOLD_BITS_RAND_GEN_HPP

#include <utility>
#include <random>
#include <bits/stl_algo.h>

#include <gold/bits/concepts/types.hpp>

namespace gold {

    namespace __random {

        /// qualified_dist_type_impl
        template <typename T>
        concept qualified_dist_type_impl =
            requires (T t, const typename T::param_type& p) {
                typename T::result_type;
                typename T::param_type;
                { t.reset()  } -> std::same_as<void>;
                { t.min()    } -> std::same_as<typename T::result_type>;
                { t.max()    } -> std::same_as<typename T::result_type>;
                { t.param()  } -> std::same_as<typename T::param_type>;
                { t.param(p) } -> std::same_as<void>;
            };

    } // namespace __random

    /// qualified_distribution_type
    template <typename T>
    concept qualified_distribution_type = __random::qualified_dist_type_impl<std::remove_cvref_t<T>>;

    /// random_seed_t
    enum random_seed_t : decltype(std::declval<std::random_device>()()) {};

    /// basic_random_generator
    template <std::uniform_random_bit_generator RandEngineT>
    class basic_random_generator {
      public:
        using engine_type         = RandEngineT;
        using engine_result_type  = typename engine_type::result_type;
        using default_seeder_type = std::random_device;

      private:
        engine_type m_generator_ { default_seeder_type{}() };

      public:
        //// ctors
        /// default ctor
        basic_random_generator() noexcept = default;

        /// copy ctor [deleted]
        basic_random_generator(const basic_random_generator&) = delete;

        /// move ctor
        basic_random_generator(basic_random_generator&&) noexcept = default;

        /// ctor taking a seed
        explicit basic_random_generator(random_seed_t seed)
        : m_generator_(std::to_underlying(seed)) {}

        /// ctor taking seed_seq
        template <typename Seq>
        explicit basic_random_generator(Seq& seq_op)
        : m_generator_(seq_op) {}

        /// copy assign [deleted]
        basic_random_generator& operator=(const basic_random_generator&) = delete;

        /// move assign
        basic_random_generator& operator=(basic_random_generator&&) noexcept = default;

        /// generate [generic]
        template <qualified_distribution_type T>
        auto generate(T&& dist) {
            return std::forward<T>(dist)(m_generator_);
        }

        /// generate_l
        template <qualified_distribution_type T>
        auto generate_l(T&& dist) {
            return [&, dist = std::forward<T>(dist)]{
                return dist(m_generator_);
            };
        }

        /// closure_generate
        template <qualified_distribution_type T>
        auto closure_generate(T&& dist) {
            return generate_l(std::forward<T>(dist));
        }

        /// next_int
        template <std::integral T>
        auto next_int(T min_, T max_) {
            std::uniform_int_distribution<T> dis_ { min_, max_ };
            return generate(dis_);
        }

        /// next_real
        template <std::floating_point T>
        auto next_real(T min_, T max_) {
            std::uniform_real_distribution<T> dis_ { min_, max_ };
            return generate(dis_);
        }

        /// next_char
        template <gold::char_type C>
        auto next_char(C min_, C max_) {
            return static_cast<C>(
                next_int(static_cast<std::uint32_t>(min_), static_cast<std::uint32_t>(max_))
            );
        }

        /// next_int_l ( lambda )
        template <std::integral T>
        auto next_int_l(T min_, T max_) {
            return [&, this] mutable {
                return next_int(min_, max_);
            };
        }

        /// closure_next_int
        template <std::integral T>
        auto closure_next_int(T min_, T max_) {
            return next_int_l(min_, max_);
        }

        /// next_real_l ( lambda )
        template <std::floating_point T>
        auto next_real_l(T min_, T max_) {
            return [&, this] mutable {
                return next_real(min_, max_);
            };
        }

        /// closure_next_real
        template <std::floating_point T>
        auto closure_next_real(T min_, T max_) {
            return next_real_l(min_, max_);
        }

        /// next_default_int - default: [0, 100]
        int next_default_int() {
            return next_int(0, 100);
        }

        /// next_default_real - default: [0.0, 1.0]
        double next_default_real() {
            return next_real(0.0, 1.0);
        }

        /// next_bool
        bool next_bool(double probability_) {
            std::bernoulli_distribution dis_ (static_cast<float>(probability_));
            return generate(dis_);
        }

        bool next_bool() {
            return next_bool(0.5f);
        }

        /// reseed
        template <typename SSeq>
        void reseed(SSeq& seq_) {
            m_generator_.seed(seq_);
        }

        void reseed(const engine_result_type& n = default_seeder_type{}()) {
            m_generator_.seed(n);
        }

        /// engine
        engine_type& engine() noexcept { return m_generator_; }

        /// choose
        template <std::ranges::sized_range R>
        decltype(auto) choose(R&& r) {
            std::size_t i = next_int(0ull, std::ranges::size(r) - 1);
            auto iter     = std::ranges::begin(r);
            std::ranges::advance(iter, i);
            return *iter;
        }

        template <typename T>
        auto choose(std::initializer_list<T> il) {
            std::size_t i = next_int(0ull, il.size() - 1);
            auto iter     = il.begin();
            iter += i;
            return *iter;
        }

        /// shuffle
        template <std::ranges::random_access_range R>
            requires std::permutable<std::ranges::iterator_t<R>>
        void shuffle(R&& r) {
            std::shuffle(std::ranges::begin(r), std::ranges::end(r), m_generator_);
        }

    };

    /// default_random_generator
    using default_random_generator = basic_random_generator<std::default_random_engine>;

    /// random
    inline default_random_generator random {};

} // namespace gold

#endif // __GOLD_BITS_RAND_GEN_HPP
