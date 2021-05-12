// type_traits.hpp under gold++.type_support

#ifndef __GOLDPLUSPLUS_TYPE_SUPPORT_TYPE_TRAITS_HPP__
#define __GOLDPLUSPLUS_TYPE_SUPPORT_TYPE_TRAITS_HPP__

#define NS_BEGIN namespace gold {
#define NS_END }

NS_BEGIN
inline namespace type_support {
namespace type_traits {
// gold::type_support::type_traits::remove_all_adj and gold::type_support::type_traits::remove_all_adj_t
template<typename T, typename U =
    std::remove_cvref_t<
    std::remove_pointer_t<
    std::remove_extent_t<
    T >>>>
struct remove_all_adj : remove_all_adj<U> {};
template<typename T> struct remove_all_adj<T, T> { using type = T; };
template<typename T> using remove_all_adj_t = typename remove_all_adj<T>::type;

// gold::type_support::type_traits::is_container and gold::type_support::type_traits::is_container_v
// gold::type_support::type_traits::is_container_adaptor and gold::type_support::type_traits::is_container_adaptor_v

template <typename T> struct is_container : std::false_type {};
template <typename T> struct is_container_adaptor : std::false_type {};
template <typename T> constexpr bool is_container_v = is_container<T>::value;
template <typename T> constexpr bool is_container_adaptor_v = is_container_adaptor<T>::value;


// Template Specializations
template <typename T, typename Alloc>
struct is_container<std::vector<T, Alloc>> : std::true_type {};

template <typename T, size_t N>
struct is_container<std::array<T, N>> : std::true_type {};

template <typename T, typename Alloc>
struct is_container<std::deque<T, Alloc>> : std::true_type {};

template <typename T, typename Alloc>
struct is_container<std::forward_list<T, Alloc>> : std::true_type {};

template <typename T, typename Alloc>
struct is_container<std::list<T, Alloc>> : std::true_type {};

template <typename Key, typename Compare, typename Alloc>
struct is_container<std::set<Key, Compare, Alloc>> : std::true_type {};

template <typename Key, typename T, typename Compare, typename Alloc>
struct is_container<std::map<Key, T, Compare, Alloc>> : std::true_type {};

template <typename Key, typename Compare, typename Alloc>
struct is_container<std::multiset<Key, Compare, Alloc>> : std::true_type {};

template <typename Key, typename T, typename Compare, typename Alloc>
struct is_container<std::multimap<Key, T, Compare, Alloc>> : std::true_type {};

template <typename Key, typename Hash, typename Pred, typename Alloc>
struct is_container<std::unordered_set<Key, Hash, Pred, Alloc>> : std::true_type {};

template <typename Key, typename T, typename Hash, typename Pred, typename Alloc>
struct is_container<std::unordered_map<Key, T, Hash, Pred, Alloc>> : std::true_type {};

template <typename Key, typename Hash, typename Pred, typename Alloc>
struct is_container<std::unordered_multiset<Key, Hash, Pred, Alloc>> : std::true_type {};

template <typename Key, typename T, typename Hash, typename Pred, typename Alloc>
struct is_container<std::unordered_multimap<Key, T, Hash, Pred, Alloc>> : std::true_type {};

template <typename T, size_t Extent>
struct is_container<std::span<T, Extent>> : std::true_type {};

template <typename T, typename Cont>
struct is_container_adaptor<std::stack<T, Cont>> : std::true_type {};

template <typename T, typename Cont>
struct is_container_adaptor<std::queue<T, Cont>> : std::true_type {};

template <typename T, typename Cont, typename Compare>
struct is_container_adaptor<std::priority_queue<T, Cont, Compare>> : std::true_type {};

// gold::type_support::type_traits::is_string and gold::type_support::type_traits::is_string_v
// note: unfinished
template <typename T> struct is_string : std::false_type {};
template <typename T> struct is_string<T&> : is_string<T> {};
template <typename T> struct is_string<T&&> : is_string<T> {};
template <typename T> struct is_string<const T> : is_string<T> {};
template <typename T> struct is_string<volatile T> : is_string<T> {};
template <typename T> struct is_string<const volatile T> : is_string<T> {};
template <typename T> struct is_string<const T&> : is_string<T> {};
template <typename T> struct is_string<const T&&> : is_string<T> {};
template <typename T> struct is_string<volatile T&> : is_string<T> {};
template <typename T> struct is_string<volatile T&&> : is_string<T> {};
template <typename T> struct is_string<const volatile T&> : is_string<T> {};
template <typename T> struct is_string<const volatile T&&> : is_string<T> {};

template <> struct is_string<char*> : std::true_type {};
template <> struct is_string<char[]> : std::true_type {};
template <> struct is_string<char(&)[]> : std::true_type {};
template <> struct is_string<char*&> : std::true_type {};
template <> struct is_string<unsigned char*> : std::true_type {};
template <> struct is_string<unsigned char[]> : std::true_type {};
template <> struct is_string<char8_t*> : std::true_type {};
template <> struct is_string<char8_t[]> : std::true_type {};
template <> struct is_string<char16_t*> : std::true_type {};
template <> struct is_string<char16_t[]> : std::true_type {};
template <> struct is_string<char32_t*> : std::true_type {};
template <> struct is_string<char32_t[]> : std::true_type {};
template <> struct is_string<wchar_t*> : std::true_type {};
template <> struct is_string<wchar_t[]> : std::true_type {};

template <> struct is_string<std::string> : std::true_type {};
template <> struct is_string<std::wstring> : std::true_type {};
template <> struct is_string<std::u8string> : std::true_type {};
template <> struct is_string<std::u16string> : std::true_type {};
template <> struct is_string<std::string_view> : std::true_type {};
template <> struct is_string<std::wstring_view> : std::true_type {};
template <> struct is_string<std::u8string_view> : std::true_type {};
template <> struct is_string<std::u16string_view> : std::true_type {};
template <> struct is_string<std::u32string_view> : std::true_type {};
}}
NS_END
#undef NS_BEGIN
#undef NS_END
#endif //  __GOLDPLUSPLUS_TYPE_SUPPORT_TYPE_TRAITS_HPP__
