// <gold/bits/mdspan/mdspan.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_MDSPAN_MDSPAN_HPP
#define __GOLD_BITS_MDSPAN_MDSPAN_HPP

#include <gold/bits/mdspan/fwd.hpp>
#include <gold/bits/mdspan/extents.hpp>
#include <gold/bits/mdspan/layout_right.hpp>
#include <gold/bits/mdspan/default_accessor.hpp>
#include <type_traits>

namespace gold {

    /// mdspan
    template <typename T, typename Extents, typename LayoutPolicy, typename AccessorPolicy>
    class mdspan {
      public:
        using extents_type     = Extents;
        using layout_type      = LayoutPolicy;
        using accessor_type    = AccessorPolicy;
        using mapping_type     = layout_type::template mapping<extents_type>;
        using element_type     = T;
        using value_type       = __remove_cv(T);
        using index_type       = extents_type::index_type;
        using size_type        = extents_type::size_type;
        using rank_type        = extents_type::rank_type;
        using data_handle_type = accessor_type::data_handle_type;
        using reference        = accessor_type::reference;

        template <typename OtherLayoutPolicy>
        using rebind_layout = mdspan<T, Extents, OtherLayoutPolicy, AccessorPolicy>;

        template <typename OtherAccessorPolicy>
        using rebind_accessor = mdspan<T, Extents, LayoutPolicy, OtherAccessorPolicy>;

        static constexpr rank_type rank() noexcept { return extents_type::rank(); }
        static constexpr rank_type rank_dynamic() noexcept { return extents_type::rank_dynamic(); }
        static constexpr std::size_t static_extent(rank_type r) noexcept { return extents_type::static_extent(r); }
        constexpr index_type extent(rank_type r) const noexcept { return extents().extent(r); }

      private:
        [[no_unique_address]] accessor_type    m_acc_;
        [[no_unique_address]] mapping_type     m_map_;
        [[no_unique_address]] data_handle_type m_ptr_;

        template <typename U, typename OtherExtents,
                  typename OtherLayoutPolicy, typename OtherAccessor>
        friend class mdspan;

      public:
        constexpr mdspan() requires (rank_dynamic() > 0) = default;
        constexpr mdspan(const mdspan&) = default;
        constexpr mdspan(mdspan&&) = default;

        template <typename... OtherIndexTypes>
            requires ((std::is_convertible_v<OtherIndexTypes, index_type> && ...) &&
                      (std::is_nothrow_constructible_v<index_type, OtherIndexTypes> && ...) &&
                      (sizeof...(OtherIndexTypes) == rank() || sizeof...(OtherIndexTypes) == rank_dynamic()) &&
                      (std::is_constructible_v<mapping_type, extents_type> && std::is_constructible_v<accessor_type>))
        constexpr explicit mdspan(data_handle_type p, OtherIndexTypes... exts)
        : m_acc_(), m_map_(extents_type(static_cast<index_type>(std::move(exts))...)), m_ptr_(std::move(p)) {}

        template <typename OtherIndexType, std::size_t N>
            requires (std::is_convertible_v<const OtherIndexType&, index_type> &&
                      std::is_nothrow_constructible_v<index_type, const OtherIndexType&> &&
                      (N == rank() || N == rank_dynamic()) &&
                      std::is_constructible_v<mapping_type, extents_type> && std::is_constructible_v<accessor_type>)
        constexpr explicit(N != rank_dynamic()) mdspan(data_handle_type p, std::span<OtherIndexType, N> exts)
        : m_acc_(), m_map_(extents_type(exts)), m_ptr_(std::move(p)) {}

        template <typename OtherIndexType, std::size_t N>
            requires (std::is_convertible_v<const OtherIndexType&, index_type> &&
                      std::is_nothrow_constructible_v<index_type, const OtherIndexType&> &&
                      (N == rank() || N == rank_dynamic()) &&
                      std::is_constructible_v<mapping_type, extents_type> && std::is_constructible_v<accessor_type>)
        constexpr explicit(N != rank_dynamic()) mdspan(data_handle_type p, const std::array<OtherIndexType, N>& exts)
        : m_acc_(), m_map_(extents_type(exts)), m_ptr_(std::move(p)) {}

        constexpr mdspan(data_handle_type p, const extents_type& ext)
            requires (std::is_constructible_v<mapping_type, const extents_type&> &&
                      std::is_constructible_v<accessor_type>)
        : m_acc_(), m_map_(ext), m_ptr_(std::move(p)) {}

        constexpr mdspan(data_handle_type p, const mapping_type& m)
            requires (std::is_constructible_v<accessor_type>)
        : m_acc_(), m_map_(m), m_ptr_(std::move(p)) {}

        constexpr mdspan(data_handle_type p, const mapping_type& m, const accessor_type& a)
        : m_acc_(a), m_map_(m), m_ptr_(std::move(p)) {}

        template <typename U, typename OtherExtents,
                  typename OtherLayoutPolicy, typename OtherAccessor>
            requires (std::is_constructible_v<mapping_type, const typename OtherLayoutPolicy::template mapping<OtherExtents>&> &&
                      std::is_constructible_v<accessor_type, const OtherAccessor&> &&
                      std::is_constructible_v<data_handle_type, const typename OtherAccessor::data_handle_type&> &&
                      std::is_constructible_v<extents_type, OtherExtents>)
        constexpr explicit(!std::is_convertible_v<const typename OtherLayoutPolicy::template mapping<OtherExtents>&, mapping_type> ||
                           !std::is_convertible_v<const OtherAccessor&, accessor_type>)
        mdspan(const mdspan<U, OtherExtents, OtherLayoutPolicy, OtherAccessor>& other)
        : m_acc_(other.m_acc_), m_map_(other.m_map_), m_ptr_(other.m_ptr_) {}

        template <typename... OtherIndexTypes>
            requires ((std::is_convertible_v<OtherIndexTypes, index_type> && ...) &&
                      (std::is_nothrow_constructible_v<index_type, OtherIndexTypes> && ...) &&
                      (sizeof...(OtherIndexTypes) == rank()))
        constexpr reference operator[](OtherIndexTypes... indices) const {
            return m_acc_.access(m_ptr_, m_map_(static_cast<index_type>(std::move(indices))...));
        }

        template <typename OtherIndexType>
        constexpr reference operator[](std::span<OtherIndexType, rank()> indices) const {
            return gold::pack_apply_index<rank()>([&]<std::size_t... Is> -> reference {
                return operator[](static_cast<const OtherIndexType&>(indices[Is])...);
            });
        }

        template <typename OtherIndexType>
        constexpr reference operator[](const std::array<OtherIndexType, rank()>& indices) const {
            return operator[](std::span(indices));
        }

        constexpr size_type size() const noexcept {
            index_type s = 1;
            for (std::size_t r = 0; r < extents_type::rank(); ++r)
                s *= extents().extent(r);
            return s;
        }

        [[nodiscard]] constexpr bool empty() const noexcept {
            return size() == 0;
        }

        friend constexpr void swap(mdspan& lhs, mdspan& rhs) noexcept {
            std::ranges::swap(lhs.m_acc_, rhs.m_acc_);
            std::ranges::swap(lhs.m_map_, rhs.m_map_);
            std::ranges::swap(lhs.m_ptr_, rhs.m_ptr_);
        }

        constexpr const extents_type& extents() const noexcept { return m_map_.extents(); }
        constexpr const data_handle_type& data_handle() const noexcept { return m_ptr_; }
        constexpr const mapping_type& mapping() const noexcept { return m_map_; }
        constexpr const accessor_type& accessor() const noexcept { return m_acc_; }

        static constexpr bool is_always_unique() noexcept { return mapping_type::is_always_unique(); }
        static constexpr bool is_always_exhaustive() noexcept { return mapping_type::is_always_exhaustive(); }
        static constexpr bool is_always_strided() noexcept { return mapping_type::is_always_strided(); }

        constexpr bool is_unique() const noexcept { return m_map_.is_unique(); }
        constexpr bool is_exhaustive() const noexcept { return m_map_.is_exhaustive(); }
        constexpr bool is_strided() const noexcept { return m_map_.is_strided(); }

        constexpr index_type stride(rank_type r) const { return m_map_.stride(r); }
    };

    template <typename CArray>
        requires (std::is_array_v<CArray> && std::rank_v<CArray> == 1)
    mdspan(CArray&) -> mdspan<std::remove_all_extents<CArray>,
                              extents<std::size_t, std::extent_v<CArray, 0>>>;

    template <typename P>
        requires (std::is_pointer_v<__remove_reference(P)>)
    mdspan(P&&) -> mdspan<std::remove_pointer_t<__remove_reference(P)>, extents<std::size_t>>;

    template <typename T, typename... Ints>
        requires ((__is_convertible(Ints, std::size_t) && ...) &&
                  sizeof...(Ints) > 0)
    explicit mdspan(T*, Ints...) -> mdspan<T, extents<std::size_t, __mdspan::maybe_static_ext<Ints>...>>;

    template <typename T, typename OtherIndexType, std::size_t N>
    mdspan(T*, std::span<OtherIndexType, N>) -> mdspan<T, dextents<std::size_t, N>>;

    template <typename T, typename OtherIndexType, std::size_t N>
    mdspan(T*, const std::array<OtherIndexType, N>&) -> mdspan<T, dextents<std::size_t, N>>;

    template <typename T, typename IndexType, std::size_t... Exts>
    mdspan(T*, const extents<IndexType, Exts...>&) -> mdspan<T, extents<IndexType, Exts...>>;

    template <typename T, typename MappingType>
    mdspan(T*, const MappingType&) -> mdspan<T, typename MappingType::extents_type,
                                                typename MappingType::layout_type>;

    template <typename MappingType, typename AccessorType>
    mdspan(const typename AccessorType::data_handle_type&,
           const MappingType&, const AccessorType&)
    -> mdspan<typename AccessorType::element_type,
              typename MappingType::extents_type,
              typename MappingType::layout_type,
              AccessorType>;

} // namespace gold

#endif // __GOLD_BITS_MDSPAN_MDSPAN_HPP
