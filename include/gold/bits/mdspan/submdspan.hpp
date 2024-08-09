// <gold/bits/mdspan/mdspan.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MDSPAN_SUBMDSPAN_HPP
#define __GOLD_BITS_MDSPAN_SUBMDSPAN_HPP

#include <gold/bits/mdspan/extents.hpp>
#include <gold/bits/mdspan/layout_left.hpp>
#include <gold/bits/mdspan/layout_right.hpp>
#include <gold/bits/mdspan/layout_stride.hpp>
#include <gold/bits/mdspan/mdspan.hpp>
#include <gold/pack>
#include <gold/bits/__util/mini_array.hpp>
#include <gold/bits/type_traits/specialization_of.hpp>
#include <gold/bits/sequence/make_integer_seq.hpp>
#include <gold/bits/sequence/make_value_seq_from.hpp>

namespace gold {

    namespace __mdspan {

        /// __mdspan::integral_constant_like [ defined in <gold/bits/mdspan/extents.hpp> ]

        /// __mdspan::de_ice
        template <typename T>
        constexpr auto de_ice(T val) {
            if constexpr (__mdspan::integral_constant_like<T>)
                return T::value;
            else
                return val;
        }

    } // namespace __mdspan

    /// ranged_slice
    // representing a right-open interval [begin, end) slice
    template <typename BeginType, typename EndType>
    struct ranged_slice {

        [[no_unique_address]] BeginType begin;
        [[no_unique_address]] EndType   end;

        static_assert(std::is_integral_v<BeginType> || __mdspan::integral_constant_like<BeginType>);
        static_assert(std::is_integral_v<EndType> || __mdspan::integral_constant_like<EndType>);
    };

    /// strided_slice
    template <typename Offset, typename Extent, typename Stride>
    struct strided_slice {
        using offset_type = Offset;
        using extent_type = Extent;
        using stride_type = Stride;

        [[no_unique_address]] offset_type offset {};
        [[no_unique_address]] extent_type extent {};
        [[no_unique_address]] stride_type stride {};

        static_assert(std::is_integral_v<Offset> || __mdspan::integral_constant_like<Offset>);
        static_assert(std::is_integral_v<Extent> || __mdspan::integral_constant_like<Extent>);
        static_assert(std::is_integral_v<Stride> || __mdspan::integral_constant_like<Stride>);
    };

    /// submdspan_mapping_result
    template <typename LayoutMapping>
    struct submdspan_mapping_result {
        [[no_unique_address]] LayoutMapping mapping = LayoutMapping();
        std::size_t offset;
    };

    /// full_extent_t
    struct full_extent_t {
        explicit full_extent_t() = default;
    };

    /// full_extent
    inline constexpr full_extent_t full_extent {};

    namespace __mdspan {

        /// __mdspan::is_ranged_slice
        template <typename T>
        inline constexpr bool is_ranged_slice = gold::is_specialization_of_v<T, gold::ranged_slice>();

        /// __mdspan::is_strided_slice
        template <typename T>
        inline constexpr bool is_strided_slice = gold::is_specialization_of_v<T, gold::strided_slice>();

        /// __mdspan::first_
        template <typename IndexType, std::size_t K, typename... SliceSpecifiers>
            requires std::is_integral_v<IndexType>
        constexpr IndexType first_(SliceSpecifiers... slices) {
            using SliceK = __type_pack_element<K, SliceSpecifiers...>;
            auto& slice_k = gold::pack_get<K>(slices...);
            if constexpr (std::convertible_to<SliceK, IndexType>)
                return __mdspan::index_cast<IndexType>(slice_k);
            else if constexpr (__mdspan::is_ranged_slice<SliceK>)
                return __mdspan::index_cast<IndexType>(__mdspan::de_ice(slice_k.begin));
            else if constexpr (__mdspan::is_strided_slice<SliceK>)
                return __mdspan::index_cast<IndexType>(__mdspan::de_ice(slice_k.offset));
            else
                return 0;
        }

        /// __mdspan::last_
        template <std::size_t K, typename Extents, typename... SliceSpecifiers>
            requires __mdspan::is_extents<Extents>
        constexpr auto last_(const Extents& src, SliceSpecifiers... slices) {
            using IndexType = typename Extents::index_type;
            using SliceK = __type_pack_element<K, SliceSpecifiers...>;
            auto& slice_k = gold::pack_get<K>(slices...);
            if constexpr (std::convertible_to<SliceK, IndexType>)
                return __mdspan::index_cast<IndexType>(__mdspan::de_ice(slice_k) + 1);
            else if constexpr (__mdspan::is_ranged_slice<SliceK>)
                return __mdspan::index_cast<IndexType>(__mdspan::de_ice(slice_k.end));
            else if constexpr (__mdspan::is_strided_slice<SliceK>)
                return __mdspan::index_cast<IndexType>(__mdspan::de_ice(slice_k.offset) +
                                                       __mdspan::de_ice(slice_k.extent));
            else
                return __mdspan::index_cast<IndexType>(src.extent(K));
        }

        /// __mdspan::generate_map_rank_table
        template <typename IndexType, typename... SliceSpecifiers>
        consteval auto generate_map_rank_table() {
            static constexpr std::size_t rank = sizeof...(SliceSpecifiers);
            std::array<std::size_t, rank> result;
            gold::pack_for_each<SliceSpecifiers...>([&, k = 0, j = 0]<typename SliceK> mutable {
                if constexpr (std::convertible_to<SliceK, IndexType>)
                    result[k] = std::dynamic_extent;
                else
                    result[k] = j++;
                ++k;
            });
            return result;
        }

        /// __mdspan::src_indices
        template <typename IndexType, std::size_t N, typename... SliceSpecifiers>
            requires std::is_integral_v<IndexType>
        constexpr std::array<IndexType, sizeof...(SliceSpecifiers)> src_indices(const std::array<IndexType, N>& indices,
                                                                                SliceSpecifiers... slices) {
            static constexpr auto map_rank = __mdspan::generate_map_rank_table<IndexType, SliceSpecifiers...>();
            std::array<IndexType, sizeof...(SliceSpecifiers)> result;
            gold::pack_for_each_index<sizeof...(SliceSpecifiers)>([&]<std::size_t K>{
                if constexpr (map_rank[K] == std::dynamic_extent)
                    result[K] = __mdspan::first_<IndexType, K>(slices...);
                else
                    result[K] = __mdspan::first_<IndexType, K>(slices...) + indices[map_rank[K]];
            });
            return result;
        }

        /// __mdspan::slice_specifier
        template <typename Slice, typename IndexType = std::size_t>
        concept slice_specifier =
               std::is_convertible_v<Slice, IndexType>
            || std::is_convertible_v<Slice, gold::full_extent_t>
            || __mdspan::is_ranged_slice<Slice>
            || __mdspan::is_strided_slice<Slice>;

        /// __mdspan::make_subextents_type
        template <typename Extents, typename... SliceSpecifiers>
        consteval auto make_subextents_type() {
            using index_type = typename Extents::index_type;
            static constexpr std::size_t subrank = ((!std::convertible_to<SliceSpecifiers, index_type>) + ... + 0);
            static constexpr auto map_rank = __mdspan::generate_map_rank_table<index_type, SliceSpecifiers...>();
            static constexpr auto subextents_seq = gold::make_sequence_from_array<
                [] {
                    __util::mini_array<std::size_t, subrank> result;
                    gold::pack_for_each_index<Extents::rank()>([&]<std::size_t K> {
                        using SliceK = __type_pack_element<K, SliceSpecifiers...>;
                        if constexpr (map_rank[K] != std::dynamic_extent) {
                            if constexpr (std::is_convertible_v<SliceK, gold::full_extent_t>)
                                result[map_rank[K]] = Extents::static_extent(K);
                            else if constexpr (requires {
                                                    requires __mdspan::is_ranged_slice<SliceK>;
                                                    requires __mdspan::integral_constant_like<decltype(std::declval<SliceK>().begin)>;
                                                    requires __mdspan::integral_constant_like<decltype(std::declval<SliceK>().end)>;
                                               })
                                result[map_rank[K]] = __mdspan::de_ice(SliceK().end) -
                                                      __mdspan::de_ice(SliceK().begin);
                            else if constexpr (requires {
                                                    requires __mdspan::is_strided_slice<SliceK>;
                                                    requires __mdspan::integral_constant_like<typename SliceK::extent_type>;
                                                    requires typename SliceK::extent_type() == 0;
                                               })
                                result[map_rank[K]] = 0;
                            else if constexpr (requires {
                                                    requires __mdspan::is_strided_slice<SliceK>;
                                                    requires __mdspan::integral_constant_like<typename SliceK::extent_type>;
                                                    requires __mdspan::integral_constant_like<typename SliceK::stride_type>;
                                               })
                                result[map_rank[K]] = 1 + (__mdspan::de_ice(SliceK::extent_type()) - 1) /
                                                           __mdspan::de_ice(SliceK::stride_type());
                            else
                                result[map_rank[K]] = std::dynamic_extent;
                        }
                    });
                    return result;
                } ()
            >();
            return []<std::size_t... Is>(gold::index_seq_t<Is...>) {
                return gold::extents<index_type, Is...> {};
            }(subextents_seq);
        }

    } // namespace __mdspan

    /// submdspan_extents
    template <typename Extents, typename... SliceSpecifiers>
        requires __mdspan::is_extents<Extents> &&
                (Extents::rank() == sizeof...(SliceSpecifiers)) &&
                (__mdspan::slice_specifier<SliceSpecifiers, typename Extents::index_type> && ...)
    constexpr auto submdspan_extents(const Extents& src, SliceSpecifiers... slices) {
        using IndexType  = typename Extents::index_type;
        using SubExtents = decltype(__mdspan::make_subextents_type<Extents, SliceSpecifiers...>());
        static constexpr auto map_rank = __mdspan::generate_map_rank_table<IndexType, SliceSpecifiers...>();
        const auto extents_in_array = [&] {
            std::array<IndexType, SubExtents::rank()> result;
            gold::pack_for_each_index<Extents::rank()>([&]<std::size_t K> {
                using SliceK = __type_pack_element<K, SliceSpecifiers...>;
                if constexpr (map_rank[K] != std::dynamic_extent) {
                    auto& slice_k = gold::pack_get<K>(slices...);
                    if constexpr (__mdspan::is_strided_slice<SliceK>)
                        result[map_rank[K]] = slice_k.extent == 0
                                            ? 0
                                            : 1 + (__mdspan::de_ice(slice_k.extent) - 1) /
                                                   __mdspan::de_ice(slice_k.stride);
                    else
                        result[map_rank[K]] = __mdspan::last_<K>(src, slices...) -
                                              __mdspan::first_<IndexType, K>(slices...);
                }
            });
            return result;
        }();
        return SubExtents { extents_in_array };
    }

    namespace __mdspan {

        /// __mdspan::layout_left_submdspan_mapping_condition
        template <typename IndexType, typename Extents, typename SubExtents, typename... SliceSpecifiers>
        consteval bool layout_left_submdspan_mapping_condition() {
            if constexpr (SubExtents::rank() == 0 || (SubExtents::rank() - 1) >= sizeof...(SliceSpecifiers))
                return false;
            else {
                using last_slice_spec = __type_pack_element<(SubExtents::rank() - 1), SliceSpecifiers...>;
                return gold::pack_apply_index<SubExtents::rank() - 1>([]<std::size_t... Ks> {
                    return (std::is_convertible_v<gold::pack_element_at<Ks, SliceSpecifiers...>, gold::full_extent_t> && ...);
                }) && (__mdspan::is_ranged_slice<last_slice_spec> ||
                       std::is_convertible_v<last_slice_spec, gold::full_extent_t>);
            }
        }

        /// __mdspan::layout_right_submdspan_mapping_condition
        template <typename IndexType, typename Extents, typename SubExtents, typename... SliceSpecifiers>
        consteval bool layout_right_submdspan_mapping_condition() {
            if constexpr (Extents::rank()  < SubExtents::rank() ||
                          (Extents::rank() - SubExtents::rank()) >= sizeof...(SliceSpecifiers))
                return false;
            else {
                using first_slice_spec = __type_pack_element<(Extents::rank() - SubExtents::rank()), SliceSpecifiers...>;
                return gold::pack_apply_index<SubExtents::rank() - 1>([]<std::size_t... Ks> {
                    constexpr auto apply_offset = [](std::size_t K) {
                        return K + Extents::rank() - SubExtents::rank() + 1;
                    };
                    return (std::is_convertible_v<gold::pack_element_at<apply_offset(Ks), SliceSpecifiers...>, gold::full_extent_t> && ...);
                }) && (__mdspan::is_ranged_slice<first_slice_spec> ||
                       std::is_convertible_v<first_slice_spec, gold::full_extent_t>);
            }
        }

        /// __mdspan::submdspan_mapping_impl
        template <typename Extents, typename LayoutMapping, typename... SliceSpecifiers>
        constexpr auto submdspan_mapping_impl(const LayoutMapping& mapping, SliceSpecifiers... slices) {
            using index_type = typename Extents::index_type;
            const auto sub_ext = gold::submdspan_extents(mapping.extents(), slices...);
            using SubExtents = __remove_cv(decltype(sub_ext));
            static constexpr auto map_rank = __mdspan::generate_map_rank_table<index_type, SliceSpecifiers...>();
            const auto sub_strides = [&] {
                std::array<index_type, SubExtents::rank()> result;
                gold::pack_for_each_index<Extents::rank()>([&]<std::size_t K> {
                    using SliceK = __type_pack_element<K, SliceSpecifiers...>;
                    if constexpr (map_rank[K] != std::dynamic_extent) {
                        auto& slice_k = gold::pack_get<K>(slices...);
                        if constexpr (__mdspan::is_strided_slice<SliceK>) {
                            if (slice_k.stride < slice_k.extent)
                                result[map_rank[K]] = mapping.stride(K) * __mdspan::de_ice(slice_k.stride);
                            else
                                result[map_rank[K]] = mapping.stride(K);
                        } else {
                            result[map_rank[K]] = mapping.stride(K);
                        }
                    }
                });
                return result;
            }();
            return [&]<std::size_t... P>(gold::index_seq_t<P...>) {
                const std::size_t offset = mapping(__mdspan::first_<index_type, P>(slices...)...);
                if constexpr (Extents::rank() == 0)
                    return gold::submdspan_mapping_result { mapping, 0 };
                else if constexpr (std::is_same_v<gold::layout_left, typename LayoutMapping::layout_type> &&
                         __mdspan::layout_left_submdspan_mapping_condition<index_type, Extents, SubExtents, SliceSpecifiers...>())
                    return gold::submdspan_mapping_result { gold::layout_left::mapping(sub_ext), offset };
                else if constexpr (std::is_same_v<gold::layout_right, typename LayoutMapping::layout_type> &&
                         __mdspan::layout_right_submdspan_mapping_condition<index_type, Extents, SubExtents, SliceSpecifiers...>())
                    return gold::submdspan_mapping_result { gold::layout_right::mapping(sub_ext), offset };
                else
                    return gold::submdspan_mapping_result { gold::layout_stride::mapping(sub_ext, sub_strides), offset };
            }(gold::make_index_seq<LayoutMapping::extents_type::rank()>);
        }

    } // namespace __mdspan

    /// layout_left::mapping::mf_submdspan_mapping_impl_
    // TODO: add constraints
    template <typename Extents>
    template <typename... SliceSpecifiers>
    constexpr auto layout_left::mapping<Extents>::mf_submdspan_mapping_impl_(SliceSpecifiers... slices) const {
        return __mdspan::submdspan_mapping_impl<Extents>(*this, slices...);
    }

    /// layout_right::mapping::mf_submdspan_mapping_impl_
    // TODO: add constraints
    template <typename Extents>
    template <typename... SliceSpecifiers>
    constexpr auto layout_right::mapping<Extents>::mf_submdspan_mapping_impl_(SliceSpecifiers... slices) const {
        return __mdspan::submdspan_mapping_impl<Extents>(*this, slices...);
    }

    /// layout_stride::mapping::mf_submdspan_mapping_impl_
    // TODO: add constraints
    template <typename Extents>
    template <typename... SliceSpecifiers>
    constexpr auto layout_stride::mapping<Extents>::mf_submdspan_mapping_impl_(SliceSpecifiers... slices) const {
        return __mdspan::submdspan_mapping_impl<Extents>(*this, slices...);
    }

    /// submdspan
    // TODO: add constraints
    template <typename ElementType, typename Extents, typename LayoutPolicy, typename AccessorPolicy,
              typename... SliceSpecifiers>
    constexpr auto submdspan(const mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy>& src,
                             SliceSpecifiers... slices) {
        auto sub_map_offset = submdspan_mapping(src.mapping(), slices...);
        return mdspan (src.accessor().offset(src.data_handle(), sub_map_offset.offset),
                       sub_map_offset.mapping,
                       typename AccessorPolicy::offset_policy(src.accessor()));
    }

} // namespace gold

#endif // __GOLD_BITS_MDSPAN_SUBMDSPAN_HPP
