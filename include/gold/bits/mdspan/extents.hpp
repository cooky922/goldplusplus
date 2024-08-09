// <gold/bits/mdspan/extents.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MDSPAN_EXTENTS_HPP
#define __GOLD_BITS_MDSPAN_EXTENTS_HPP

#include <array>
#include <concepts>
#include <span>
#include <limits>
#include <utility>
#include <gold/bits/tuples/fwd.hpp>
#include <gold/bits/mdspan/fwd.hpp>
#include <gold/bits/sequence/value_seq.hpp>
#include <gold/bits/__util/mini_array.hpp>
#include <gold/bits/__util/uneval.hpp>
#include <gold/bits/type_traits/helper_class.hpp>

namespace gold {

    namespace __mdspan {

        /// __mdspan::generate_dynamic_index_map
        template <bool... IsDynamicExtents>
        consteval auto generate_dynamic_index_map() {
            constexpr std::size_t rank_ = sizeof...(IsDynamicExtents);
            __util::mini_array<bool, rank_> is_dynamic_at { IsDynamicExtents ... };
            __util::mini_array<std::size_t, rank_> result;
            for (std::size_t r = 0, i = 0; r < rank_; ++r) {
                result[r] = i;
                if (is_dynamic_at[r])
                    ++i;
            }
            return result;
        }

        /// __mdspan::generate_dynamic_index_inv_map
        template <bool... IsDynamicExtents>
        consteval auto generate_dynamic_index_inv_map() {
            constexpr std::size_t rank_ = sizeof...(IsDynamicExtents);
            constexpr std::size_t rank_dyn_ = (std::size_t(IsDynamicExtents) + ... + 0);
            __util::mini_array<bool, rank_> is_dynamic_at { IsDynamicExtents ... };
            __util::mini_array<std::size_t, rank_dyn_> result;
            for (std::size_t r = 0, i = 0; r < rank_; ++r)
                if (is_dynamic_at[r])
                    result[i++] = r;
            return result;
        }

        /// __mdspan::extents_impl
        template <typename Dynamic, std::size_t... Extents>
        struct extents_impl {
            static constexpr std::size_t DynTag = std::dynamic_extent;
            static constexpr std::size_t s_rank_ = sizeof...(Extents);
            static constexpr std::size_t s_rank_dyn_ = ((Extents == DynTag) + ... + 0);
            static constexpr bool s_req_size_is_always_zero_ = ((Extents == 0) || ...);
            static constexpr auto s_dyn_idx_map_ = __mdspan::generate_dynamic_index_map<(Extents == DynTag)...>();
            static constexpr auto s_dyn_idx_inv_map_ = __mdspan::generate_dynamic_index_inv_map<(Extents == DynTag)...>();

            using Static = std::size_t;
            using StaticExtents  = __util::mini_array<Static, s_rank_>;
            using DynamicExtents = __util::mini_array<Dynamic, s_rank_dyn_>;

            static constexpr StaticExtents s_static_exts_ { Extents ... };
            [[no_unique_address]] DynamicExtents m_dyn_exts_;

            template <std::size_t... Is>
            static constexpr DynamicExtents sf_zeroes_(gold::index_seq_t<Is...>*) noexcept {
                return DynamicExtents { ((void) Is, 0) ... };
            }

            constexpr extents_impl() noexcept requires (s_rank_dyn_ == 0) = default;

            constexpr extents_impl() noexcept requires (s_rank_dyn_ != 0)
            : m_dyn_exts_(sf_zeroes_(static_cast<gold::index_seq_t<__integer_pack(s_rank_dyn_)...>*>(nullptr))) {}

            template <typename... DynVals>
                requires (sizeof...(DynVals) == s_rank_dyn_)
            constexpr extents_impl(DynVals... vals)
            : m_dyn_exts_{static_cast<Dynamic>(vals)...} {}

            template <typename T, std::size_t N>
                requires (N == s_rank_dyn_)
            constexpr extents_impl(const std::span<T, N>& vals) {
                if constexpr (N > 0) {
                    for (std::size_t i = 0; i < N; ++i)
                        m_dyn_exts_[i] = static_cast<Dynamic>(vals[i]);
                }
            }

            template <typename... DynVals>
                requires (sizeof...(DynVals) != s_rank_dyn_)
                      && (sizeof...(DynVals) == s_rank_)
            constexpr extents_impl(DynVals... vals) {
                Dynamic ops[s_rank_] = {static_cast<Dynamic>(vals)...};
                for (std::size_t i = 0; i < s_rank_; ++i) {
                    Static static_ext = s_static_exts_[i];
                    if (static_ext == DynTag)
                        m_dyn_exts_[s_dyn_idx_map_[i]] = ops[i];
                    else
                        [[gnu::assume(ops[i] == static_cast<Dynamic>(static_ext))]];
                }
            }

            template <typename T, std::size_t N>
                requires (N != s_rank_dyn_)
                      && (N == s_rank_ || N == std::dynamic_extent)
            constexpr extents_impl(const std::span<T, N>& vals) {
                if constexpr (N > 0) {
                    for (std::size_t i = 0; i < N; ++i) {
                        Static static_ext = s_static_exts_[i];
                        if (static_ext == DynTag)
                            m_dyn_exts_[s_dyn_idx_map_[i]] = static_cast<Dynamic>(vals[i]);
                        else
                            [[gnu::assume(static_cast<Dynamic>(vals[i]) == static_cast<Dynamic>(static_ext))]];
                    }
                }
            }

            constexpr Dynamic mf_extent_(std::size_t i) const {
                [[gnu::assume(i < s_rank_)]];
                return s_static_exts_[i] == DynTag ?
                       m_dyn_exts_[s_dyn_idx_map_[i]] :
                       static_cast<Dynamic>(s_static_exts_[i]);
            }
        };

        /// __mdspan::is_representable_as
        template <typename To, typename From>
        constexpr bool is_representable_as(From value) {
            return std::in_range<To>(value);
        }

        /// __mdspan::are_representable_as
        template <std::integral To, typename... From>
        constexpr bool are_representable_as(From... ops) {
            return (__mdspan::is_representable_as<To>(ops) && ... && true);
        }

        template <std::integral To, typename From, std::size_t N>
        constexpr bool are_representable_as(std::span<From, N> op) {
            for (std::size_t i = 0; i < N; ++i)
                if (!__mdspan::is_representable_as<To>(op[i]))
                    return false;
            return true;
        }

        /// __mdspan::is_index_in_extent
        template <std::integral IndexType, std::integral From>
        constexpr bool is_index_in_extent(IndexType ext, From val) {
            if constexpr (std::is_signed_v<From>) {
                if (val < 0)
                    return false;
            }
            using T = std::common_type_t<IndexType, From>;
            return static_cast<T>(val) < static_cast<T>(ext);
        }

        template <std::integral IndexType, typename From>
            requires (!std::integral<From>)
        constexpr bool is_index_in_extent(IndexType ext, From val) {
            if constexpr (std::is_signed_v<From>) {
                if (static_cast<IndexType>(val) < 0)
                    return false;
            }
            return static_cast<IndexType>(val) < ext;
        }

        /// __mdspan::is_multidim_index_in
        template <typename Extents, typename... From>
        constexpr bool is_multidim_index_in(const Extents& ext, From... vals) {
            return gold::pack_apply_index<Extents::rank()>([&]<std::size_t... Is> {
                return (__mdspan::is_index_in_extent(ext.extent(Is), vals) && ...);
            });
        }

        /// __mdspan::integral_constant_like
        template <typename T>
        concept integral_constant_like =
            std::is_integral_v<decltype(T::value)> &&
            !std::is_same_v<bool, std::remove_const_t<decltype(T::value)>> &&
            std::convertible_to<T, decltype(T::value)> &&
            std::equality_comparable_with<T, decltype(T::value)> &&
            std::bool_constant<T() == T::value>::value &&
            std::bool_constant<static_cast<decltype(T::value)>(T()) == T::value>::value;

        /// __mdspan::maybe_static_ext
        template <typename>
        inline constexpr std::size_t maybe_static_ext = std::dynamic_extent;

        template <__mdspan::integral_constant_like T>
        inline constexpr std::size_t maybe_static_ext<T> = static_cast<std::size_t>(T::value);

    } // namespace __mdspan

    /// extents
    template <typename IndexType, std::size_t... Extents>
    class extents {
      public:
        using index_type = IndexType;
        using size_type  = std::make_unsigned_t<index_type>;
        using rank_type  = std::size_t;

        static_assert(std::is_integral_v<index_type> && !std::is_same_v<index_type, bool>);
        static_assert(((__mdspan::is_representable_as<index_type>(Extents) || (Extents == std::dynamic_extent)) && ...));

      private:
        using ExtentsImpl = __mdspan::extents_impl<IndexType, Extents...>;
        [[no_unique_address]] ExtentsImpl m_exts_;

        // structured binding support
        template <std::size_t I>
        friend constexpr index_type get(const extents& exts) noexcept {
            return exts.extent(I);
        }

      public:
        static constexpr rank_type rank() noexcept { return ExtentsImpl::s_rank_; }
        static constexpr rank_type rank_dynamic() noexcept { return ExtentsImpl::s_rank_dyn_; }

        constexpr index_type extent(rank_type r) const noexcept { return m_exts_.mf_extent_(r); }
        static constexpr std::size_t static_extent(rank_type r) noexcept {
            return ExtentsImpl::s_static_exts_[r];
        }

        constexpr extents() noexcept = default;

        template <typename... OtherIndexTypes>
            requires ((std::is_convertible_v<OtherIndexTypes, index_type> && ...) &&
                      (std::is_nothrow_constructible_v<index_type, OtherIndexTypes> && ...) &&
                      (sizeof...(OtherIndexTypes) == rank() || sizeof...(OtherIndexTypes) == rank_dynamic()))
        constexpr explicit extents(OtherIndexTypes... dyn_vals) noexcept
        : m_exts_(static_cast<index_type>(dyn_vals)...) {
            if consteval {
                [[assume(__mdspan::are_representable_as<index_type>(dyn_vals...))]];
            }
        }

        template <typename OtherIndexType, std::size_t N>
            requires (std::is_convertible_v<OtherIndexType, index_type> &&
                      std::is_nothrow_constructible_v<index_type, OtherIndexType> &&
                      (N == rank() || N == rank_dynamic()))
        constexpr explicit(N != rank_dynamic()) extents(const std::span<OtherIndexType, N>& exts) noexcept
        : m_exts_(exts) {
            if consteval {
                [[assume(__mdspan::are_representable_as<index_type>(exts))]];
            }
        }

        template <typename OtherIndexType, std::size_t N>
            requires (std::is_convertible_v<OtherIndexType, index_type> &&
                      std::is_nothrow_constructible_v<index_type, OtherIndexType> &&
                      (N == rank() || N == rank_dynamic()))
        constexpr explicit(N != rank_dynamic()) extents(const std::array<OtherIndexType, N>& exts) noexcept
        : m_exts_(std::span(exts)) {}

      private:
        template <typename OtherExtents>
        static constexpr ExtentsImpl sf_x_vals_from_extents_(const OtherExtents& other) noexcept {
            return gold::pack_apply_index<rank_dynamic()>([&]<std::size_t... Is> {
                return ExtentsImpl { other.extent(sf_dynamic_index_inv_(Is)) ... };
            });
        }

      public:
        template <typename OtherIndexType, std::size_t... OtherExtents>
            requires ((sizeof...(OtherExtents) == sizeof...(Extents)) &&
                      ((OtherExtents == std::dynamic_extent ||
                        Extents == std::dynamic_extent      ||
                        OtherExtents == Extents) && ...))
        explicit((((Extents != std::dynamic_extent) && (OtherExtents == std::dynamic_extent)) || ...) ||
                 (static_cast<std::make_unsigned_t<index_type>>(std::numeric_limits<index_type>::max()) <
                  static_cast<std::make_unsigned_t<OtherIndexType>>(std::numeric_limits<OtherIndexType>::max())))
        constexpr extents(const extents<OtherIndexType, OtherExtents...>& other) noexcept
        : m_exts_(sf_x_vals_from_extents_(other)) {
            if consteval {
                if constexpr (rank() > 0) {
                    for (std::size_t r = 0; r < rank(); ++r) {
                        if constexpr (static_cast<std::make_unsigned_t<index_type>>(std::numeric_limits<index_type>::max()) <
                                      static_cast<std::make_unsigned_t<OtherIndexType>>(std::numeric_limits<OtherIndexType>::max()))
                            [[assume(__mdspan::is_representable_as<index_type>(other.extent(r)))]];
                        [[assume(static_extent(r) == std::dynamic_extent ||
                                 static_cast<index_type>(other.extent(r)) == static_cast<index_type>(static_extent(r)))]];
                    }
                }
            }
        }

        template <typename OtherIndexType, std::size_t... OtherExtents>
        friend constexpr bool operator==(const extents& lhs, const extents<OtherIndexType, OtherExtents...>& rhs) noexcept {
            if constexpr (rank() != sizeof...(OtherExtents)) {
                return false;
            } else {
                for (rank_type r = 0; r < rank(); ++r) {
                    using CommonType = std::common_type_t<index_type, OtherIndexType>;
                    if (static_cast<CommonType>(lhs.extent(r)) != static_cast<CommonType>(rhs.extent(r)))
                        return false;
                }
                return true;
            }
        }

      private:
        constexpr index_type mf_fwd_prod_of_extents_(rank_type r) const noexcept {
            index_type s = 1;
            for (rank_type i = 0; i < r; ++i)
                s *= extent(i);
            return s;
        }

        constexpr index_type mf_rev_prod_of_extents_(rank_type r) const noexcept {
            index_type s = 1;
            for (rank_type i = rank() - 1; i > r; --i)
                s *= extent(i);
            return s;
        }

        static constexpr index_type sf_dynamic_index_(rank_type r) noexcept {
            return ExtentsImpl::s_dyn_idx_map_[r];
        }

        static constexpr index_type sf_dynamic_index_inv_(rank_type r) noexcept {
            return ExtentsImpl::s_dyn_idx_inv_map_[r];
        }

        constexpr index_type mf_size_() const noexcept {
            if constexpr (ExtentsImpl::s_req_size_is_always_zero_)
                return 0;
            else
                return mf_fwd_prod_of_extents_(rank());
        }

        friend class layout_left;
        friend class layout_right;
        friend class layout_stride;
        template <std::size_t>
        friend class layout_left_padded;
        template <std::size_t>
        friend class layout_right_padded;
    };

    template <typename... IndexTypes>
        requires ((std::is_convertible_v<IndexTypes, std::size_t> && ...))
    extents(IndexTypes...) -> extents<std::size_t, __mdspan::maybe_static_ext<IndexTypes>...>;

    // structured binding-support for gold::extents
    template <typename IndexType, std::size_t... Extents>
    struct tuples::tuple_size<extents<IndexType, Extents...>> {
        static constexpr std::size_t value = sizeof...(Extents);
    };

    template <std::size_t I, typename IndexType, std::size_t... Extents>
    struct tuples::tuple_element<I, extents<IndexType, Extents...>> {
        using type = IndexType;
    };

    namespace __mdspan {

        /// __mdspan::dextents_impl
        template <typename IndexType, std::size_t Rank>
        consteval auto dextents_impl() {
            return []<std::size_t... Is>(gold::index_seq_t<Is...>*) {
                return __util::uneval<gold::extents<IndexType, (Is, std::dynamic_extent) ...>>;
            }(static_cast<gold::index_seq_t<__integer_pack(Rank)...>*>(nullptr));
        }

        /// __mdspan::is_extents
        template <typename>
        inline constexpr bool is_extents = false;

        template <typename IndexType, std::size_t... Args>
        inline constexpr bool is_extents<gold::extents<IndexType, Args...>> = true;

        /// __mdspan::is_mapping_of
        template <typename Layout, typename Mapping>
        concept is_mapping_of = std::is_same_v<Mapping, typename Layout::template mapping<typename Mapping::extents_type>>;

        /// __mdspan::layout_mapping_alike
        template <typename M>
        concept layout_mapping_alike = requires {
            requires __mdspan::is_extents<typename M::extents_type>;
            { M::is_always_strided() } -> std::same_as<bool>;
            { M::is_always_exhaustive() } -> std::same_as<bool>;
            { M::is_always_unique() } -> std::same_as<bool>;
            std::bool_constant<M::is_always_strided()>::value;
            std::bool_constant<M::is_always_exhaustive()>::value;
            std::bool_constant<M::is_always_unique()>::value;
        };

        /// __mdspan::is_layout_left_padded
        template <typename Layout>
        inline constexpr bool is_layout_left_padded = false;

        template <std::size_t PaddingStride>
        inline constexpr bool is_layout_left_padded<gold::layout_left_padded<PaddingStride>> = true;

        /// __mdspan::is_layout_left_padded_mapping_of
        template <typename Mapping>
        inline constexpr bool is_layout_left_padded_mapping_of =
            __mdspan::is_layout_left_padded<typename Mapping::layout_type>;

        /// __mdspan::is_layout_right_padded
        template <typename Layout>
        inline constexpr bool is_layout_right_padded = false;

        template <std::size_t PaddingStride>
        inline constexpr bool is_layout_right_padded<gold::layout_right_padded<PaddingStride>> = true;

        /// __mdspan::is_layout_right_padded_mapping_of
        template <typename Mapping>
        inline constexpr bool is_layout_right_padded_mapping_of =
            __mdspan::is_layout_right_padded<typename Mapping::layout_type>;

        /// __mdspan::index_cast
        template <typename To, typename From>
        constexpr auto index_cast(From&& i) noexcept {
            if constexpr (std::is_integral_v<From> && !std::is_same_v<From, bool>)
                return i;
            else
                return static_cast<To>(i);
        }

        /// __mdspan::least_multiple_at_least [TODO: optimize]
        template <typename T, typename U = T>
        constexpr T least_multiple_at_least(T n, U min_val) noexcept {
            T result = n;
            while (std::cmp_less(result, min_val))
                result += n;
            return result;
        }

    } // namespace __mdspan

    /// dextents
    template <typename IndexType, std::size_t Rank>
    using dextents = decltype(__mdspan::dextents_impl<IndexType, Rank>()());

} // namespace gold

#endif // __GOLD_BITS_MDSPAN_EXTENTS_HPP
