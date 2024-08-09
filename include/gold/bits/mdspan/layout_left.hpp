// <gold/bits/mdspan/layout_left.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MDSPAN_LAYOUT_LEFT_HPP
#define __GOLD_BITS_MDSPAN_LAYOUT_LEFT_HPP

#include <gold/bits/mdspan/extents.hpp>

namespace gold {

    /// layout_left::mapping
    template <typename Extents>
    class layout_left::mapping {
      public:
        using extents_type = Extents;
        using index_type   = extents_type::index_type;
        using size_type    = extents_type::size_type;
        using rank_type    = extents_type::rank_type;
        using layout_type  = layout_left;

        static_assert(__mdspan::is_extents<Extents>);

      private:
        [[no_unique_address]] extents_type m_extents_ {};

        static constexpr bool sf_required_span_size_is_representable_(const extents_type& ext) {
            if constexpr (extents_type::rank() == 0)
                return true;

            index_type prod = ext.extent(0);
            for (rank_type r = 1; r < extents_type::rank(); ++r) {
                bool overflowed = __builtin_mul_overflow(prod, ext.extent(r), &prod);
                if (overflowed)
                    return false;
            }

            return true;
        }

        static_assert(extents_type::rank_dynamic() > 0 ||
                      sf_required_span_size_is_representable_(extents_type()));

        // submdspan support
        template <typename... SliceSpecifiers>
        constexpr auto mf_submdspan_mapping_impl_(SliceSpecifiers...) const;

        template <typename... SliceSpecifiers>
        friend constexpr auto submdspan_mapping(const mapping& src, SliceSpecifiers... slices) {
            return src.mf_submdspan_mapping_impl_(slices...);
        }

      public:
        constexpr mapping() noexcept = default;
        constexpr mapping(const mapping&) noexcept = default;
        constexpr mapping(const extents_type& ext) noexcept
        : m_extents_(ext) {
            if consteval {
                [[assume(sf_required_span_size_is_representable_(ext))]];
            }
        }

        template <typename OtherExtents>
            requires __is_constructible(extents_type, OtherExtents)
        constexpr explicit(!__is_convertible(OtherExtents, extents_type))
        mapping(const mapping<OtherExtents>& other) noexcept
        : m_extents_(other.extents()) {
            if consteval {
                [[assume(__mdspan::is_representable_as<index_type>(other.required_span_size()))]];
            }
        }

        template <typename OtherExtents>
            requires __is_constructible(extents_type, OtherExtents) &&
                     (extents_type::rank() <= 1)
        constexpr explicit(!__is_convertible(OtherExtents, extents_type))
        mapping(const layout_right::mapping<OtherExtents>& other) noexcept
        : m_extents_(other.extents()) {
            if consteval {
                [[assume(__mdspan::is_representable_as<index_type>(other.required_span_size()))]];
            }
        }

        template <typename OtherExtents>
            requires __is_constructible(extents_type, OtherExtents)
        constexpr explicit(extents_type::rank() > 0)
        mapping(const layout_stride::mapping<OtherExtents>& other) noexcept
        : m_extents_(other.extents()) {
            if consteval {
                [[assume(__mdspan::is_representable_as<index_type>(other.required_span_size()))]];
            }
        }

        constexpr mapping& operator=(const mapping&) noexcept = default;

        constexpr const extents_type& extents() const noexcept { return m_extents_; }
        constexpr std::array<index_type, extents_type::rank()> strides() const noexcept {
            std::array<index_type, extents_type::rank()> result;
            index_type s = 1;
            for (rank_type i = 0; i < extents_type::rank(); ++i) {
                s *= extents().extent(i);
                result[i] = s;
            }
            return result;
        }

        constexpr index_type required_span_size() const noexcept {
            return extents().mf_size_();
        }

        template <typename... Indices>
            requires ((sizeof...(Indices) == extents_type::rank()) && (__is_convertible(Indices, index_type) && ...) &&
                      (__is_nothrow_constructible(index_type, Indices) && ...))
        constexpr index_type operator()(Indices... is) const noexcept {
            if consteval {
                [[assume(__mdspan::is_multidim_index_in(m_extents_, is...))]];
            }
            __util::mini_array<index_type, extents_type::rank()> is_a { static_cast<index_type>(is) ... };
            return gold::pack_apply_index<sizeof...(Indices)>([&]<std::size_t... Pos> {
                index_type res = 0;
                ((res = is_a[extents_type::rank() - 1 - Pos] + m_extents_.extent(extents_type::rank() - 1 - Pos) * res), ...);
                return res;
            });
        }

        static constexpr bool is_always_unique() noexcept { return true; }
        static constexpr bool is_always_exhaustive() noexcept { return true; }
        static constexpr bool is_always_strided() noexcept { return true; }

        static constexpr bool is_unique() noexcept { return true; }
        static constexpr bool is_exhaustive() noexcept { return true; }
        static constexpr bool is_strided() noexcept { return true; }

        constexpr index_type stride(rank_type r) const noexcept requires (extents_type::rank() > 0) {
            return extents().mf_fwd_prod_of_extents_(r);
        }

        template <typename OtherExtents>
            requires (OtherExtents::rank() == extents_type::rank())
        friend constexpr bool operator==(const mapping& lhs, const mapping<OtherExtents>& rhs) noexcept {
            return lhs.extents() == rhs.extents();
        }
    };

} // namespace gold

#endif // __GOLD_BITS_MDSPAN_LAYOUT_LEFT_HPP
