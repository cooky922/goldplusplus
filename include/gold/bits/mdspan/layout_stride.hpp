// <gold/bits/mdspan/layout_stride.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MDSPAN_LAYOUT_STRIDE_HPP
#define __GOLD_BITS_MDSPAN_LAYOUT_STRIDE_HPP

#include <gold/bits/mdspan/extents.hpp>

namespace gold {

    /// layout_stride::mapping
    template <typename Extents>
    class layout_stride::mapping {
      public:
        using extents_type = Extents;
        using index_type   = extents_type::index_type;
        using size_type    = extents_type::size_type;
        using rank_type    = extents_type::rank_type;
        using layout_type  = layout_stride;

        static_assert(__mdspan::is_extents<Extents>);

      private:
        static constexpr rank_type s_rank_ = extents_type::rank();
        using s_stride_storage_t_          = std::array<index_type, s_rank_>;

        [[no_unique_address]] extents_type        m_extents_ {};
        [[no_unique_address]] s_stride_storage_t_ m_strides_ {};

        template <typename OtherMapping>
        static constexpr s_stride_storage_t_ sf_fill_strides_(const OtherMapping& op) {
            return gold::pack_apply_index<s_rank_>([&]<std::size_t... Idx> {
                return s_stride_storage_t_ { static_cast<index_type>(op.stride(Idx))... };
            });
        }

        static constexpr const s_stride_storage_t_& sf_fill_strides_(const s_stride_storage_t_& op) {
            return op;
        }

        template <typename Int>
        static constexpr s_stride_storage_t_ sf_fill_strides_(std::span<Int, s_rank_> op) {
            return gold::pack_apply_index<s_rank_>([&]<std::size_t... Idx> {
                return s_stride_storage_t_ { static_cast<index_type>(op[Idx])... };
            });
        }

        template <typename Mapping>
        static constexpr typename Mapping::index_type sf_offset_(const Mapping& m) {
            return gold::pack_apply_index<s_rank_>([&]<std::size_t... Idx> {
                return m((Idx, 0)...);
            });
        }

        // submdspan support
        template <typename... SliceSpecifiers>
        constexpr auto mf_submdspan_mapping_impl_(SliceSpecifiers...) const;

        template <typename... SliceSpecifiers>
        friend constexpr auto submdspan_mapping(const mapping& src, SliceSpecifiers... slices) {
            return src.mf_submdspan_mapping_impl_(slices...);
        }

      public:
        constexpr mapping() noexcept
        : m_extents_(extents_type()), m_strides_(sf_fill_strides_(layout_right::template mapping<extents_type>())) {}

        constexpr mapping(const mapping&) noexcept = default;

        template <typename OtherIndexType>
            requires __is_convertible(const OtherIndexType&, index_type) &&
                     __is_nothrow_constructible(index_type, const OtherIndexType&)
        constexpr mapping(const extents_type& ext, std::span<OtherIndexType, s_rank_> s) noexcept
        : m_extents_(ext), m_strides_(sf_fill_strides_(s)) {}

        template <typename OtherIndexType>
            requires __is_convertible(const OtherIndexType&, index_type) &&
                     __is_nothrow_constructible(index_type, const OtherIndexType&)
        constexpr mapping(const extents_type& ext, const std::array<OtherIndexType, s_rank_>& s) noexcept
        : m_extents_(ext), m_strides_(sf_fill_strides_(std::span(s))) {}

        template <typename StridedLayoutMapping>
            requires __mdspan::layout_mapping_alike<StridedLayoutMapping> &&
                     std::is_constructible_v<extents_type, typename StridedLayoutMapping::extents_type> &&
                     (StridedLayoutMapping::is_always_unique() &&
                      StridedLayoutMapping::is_always_strided())
        constexpr explicit(!(std::is_convertible_v<typename StridedLayoutMapping::extents_type, extents_type> &&
                             (__mdspan::is_mapping_of<layout_left, StridedLayoutMapping>  ||
                              __mdspan::is_mapping_of<layout_right, StridedLayoutMapping> ||
                              __mdspan::is_mapping_of<layout_stride, StridedLayoutMapping>)))
        mapping(const StridedLayoutMapping& other) noexcept
        : m_extents_(other.extents()), m_strides_(sf_fill_strides_(other)) {}

        constexpr mapping& operator=(const mapping&) noexcept = default;

        constexpr const extents_type& extents() const noexcept { return m_extents_; }
        constexpr std::array<index_type, s_rank_> strides() const noexcept { return m_strides_; }

        constexpr index_type required_span_size() const noexcept {
            index_type s = 1;
            for (rank_type r = 0; r < s_rank_; ++r) {
                if (m_extents_.extent(r) == 0)
                    return 0;
                s += static_cast<index_type>(m_extents_.extent(r) - 1) * m_strides_[r];
            }
            return s;
        }

        template <typename... Indices>
            requires ((sizeof...(Indices) == s_rank_) && (__is_convertible(Indices, index_type) && ...) &&
                      (__is_nothrow_constructible(index_type, Indices) && ...))
        constexpr index_type operator()(Indices... is) const noexcept {
            if consteval {
                [[assume(__mdspan::is_multidim_index_in(m_extents_, is...))]];
            }
            return gold::pack_apply_index<sizeof...(Indices)>([&]<std::size_t... Pos> {
                return ((static_cast<index_type>(is) * m_strides_[Pos]) + ... + 0);
            });
        }

        static constexpr bool is_always_unique() noexcept { return true; }
        static constexpr bool is_always_exhaustive() noexcept { return false; }
        static constexpr bool is_always_strided() noexcept { return true; }

        static constexpr bool is_unique() noexcept { return true; }
        constexpr bool is_exhaustive() const noexcept { return required_span_size() == extents().mf_size_(); }
        static constexpr bool is_strided() noexcept { return true; }

        constexpr index_type stride(rank_type r) const noexcept requires (extents_type::rank() > 0) {
            return m_strides_[r];
        }

        template <typename OtherMapping>
            requires __mdspan::layout_mapping_alike<OtherMapping> &&
                     (s_rank_ == OtherMapping::extents_type::rank() &&
                      OtherMapping::is_always_strided())
        friend constexpr bool operator==(const mapping& lhs, const OtherMapping& rhs) noexcept {
            if (lhs.extents() != rhs.extents() || sf_offset_(rhs) != 0)
                return false;
            for (rank_type r = 0; r < s_rank_; ++r)
                if (lhs.stride(r) != rhs.stride(r))
                    return false;
            return true;
        }
    };

} // namespace gold

#endif // __GOLD_BITS_MDSPAN_LAYOUT_STRIDE_HPP
