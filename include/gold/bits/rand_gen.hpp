// <gold/bits/rand_gen.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RAND_GEN_HPP
#define __GOLD_BITS_RAND_GEN_HPP

#include <utility>
#include <random>
#include <bits/stl_algo.h>

#include <gold/bits/concepts/types.hpp>

namespace gold {

    /// qualified_distribution_type
    template <typename T>
    concept qualified_distribution_type
    = requires (T t, const typename T::param_type& p) {
        typename T::result_type;
        typename T::param_type;
        { t.reset()  } -> std::same_as<void>;
        { t.min()    } -> std::same_as<typename T::result_type>;
        { t.max()    } -> std::same_as<typename T::result_type>;
        { t.param()  } -> std::same_as<typename T::param_type>;
        { t.param(p) } -> std::same_as<void>;
    };

    /// random_seed_t
    enum random_seed_t : decltype(std::declval<std::random_device>()()) {};

    /// random_number_generator
    template <std::uniform_random_bit_generator RandEngineT = std::default_random_engine>
    class random_number_generator {
      public:
        using engine_type = RandEngineT;

      private:
        using default_seeder_type = std::random_device;

        engine_type m_generator_ { default_seeder_type{}() };

      public:
        /// default constructor
        random_number_generator() noexcept = default;

        /// deleted copy constructor
        random_number_generator(const random_number_generator&) = delete;

        /// move constructor
        random_number_generator(random_number_generator&&) noexcept = default;

        /// constructor taking seed
        explicit random_number_generator(random_seed_t seed)
        :  m_generator_(std::to_underlying(seed)) {}

        /// constructor taking seed_seq
        template <typename SSeq>
        explicit random_number_generator(SSeq& seq_)
        : m_generator_(seq_) {}

        /// delete copy assignment
        random_number_generator& operator=(const random_number_generator&) = delete;

        /// move assignment
        random_number_generator& operator=(random_number_generator&&) noexcept = default;

        /// generate [ generic ]
        template <typename T>
            requires qualified_distribution_type<std::remove_cvref_t<T>>
        auto generate(T&& dist) {
            return std::forward<T>(dist)(m_generator_);
        }

        /// generate_l
        template <typename T>
            requires qualified_distribution_type<std::remove_cvref_t<T>>
        auto generate_l(T&& dist) {
            return [&, dist = std::forward<T>(dist)]{
                return dist(m_generator_);
            };
        }

        /// next_int
        template <std::integral T>
        auto next_int(const T& min_, const T& max_) {
            std::uniform_int_distribution<T> dis_ { min_, max_ };
            return generate(dis_);
        }

        /// next_real
        template <std::floating_point T>
        auto next_real(const T& min_, const T& max_) {
            std::uniform_real_distribution<T> dis_ { min_, max_ };
            return generate(dis_);
        }

        /// next_char
        template <char_type C>
        auto next_char(const C& min_, const C& max_) {
            return static_cast<C>(
                next_int(static_cast<std::uint32_t>(min_), static_cast<std::uint32_t>(max_))
            );
        }

        /// next_int_l ( lambda )
        template <std::integral T>
        auto next_int_l(const T& min_, const T& max_) {
            return [&, this] mutable {
                return next_int(min_, max_);
            };
        }

        /// next_real_l ( lambda )
        template <std::floating_point T>
        auto next_real_l(const T& min_, const T& max_) {
            return [&, this] mutable {
                return next_real(min_, max_);
            };
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
        bool next_bool(const float& probability_) {
            std::bernoulli_distribution dis_ (probability_);
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

        void reseed(const typename engine_type::result_type& n = default_seeder_type{}()) {
            m_generator_.seed(n);
        }

        /// engine
        engine_type& engine() { return m_generator_; }

        /// choose
        template <std::ranges::sized_range R>
        auto choose(R&& r) {
            const std::size_t& i_ = next_int(0ull, std::ranges::size(r) - 1);
            auto iter_ = std::ranges::begin(std::forward<R>(r));
            if constexpr (std::random_access_iterator<decltype(iter_)>) {
                iter_ += i_;
            } else if constexpr (std::input_iterator<decltype(iter_)>) {
                for (auto c = i_; c > 0; --c, ++iter_);
            } else {
                std::ranges::advance(iter_, i_);
            }
            return *iter_;
        }

        template <typename T>
        auto choose(std::initializer_list<T> init_list) {
            const std::size_t& i_ = next_int(0ull, init_list.size() - 1);
            auto iter_ = init_list.begin();
            iter_ += i_;
            return *iter_;
        }

        /// shuffle
        template <std::ranges::random_access_range R>
            requires std::permutable<std::ranges::iterator_t<R>>
        void shuffle(R&& r) {
            std::shuffle(std::ranges::begin(r), std::ranges::end(r), m_generator_);
        }
    };

}

#endif // __GOLD_BITS_RAND_GEN_HPP
