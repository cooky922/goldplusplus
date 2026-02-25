#include <gold/struct_array>
#include <gold/struct_string>

struct NonCopyable {
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
};

template <gold::struct_array>
consteval void test() {}

template <gold::struct_string>
consteval void test_string() {}

/// TODO:
// rename struct_array to array_literal    [ can be mutable or immutable ]
// rename struct_string to string_literal  [ only immutable ]

struct A {};

// bahala na kung daghan ang declaration
struct A;
struct A;
struct A;
struct A;
struct A;
struct A;
struct A;

static_assert(gold::is_complete_type_v<A>);

// [gold.tuples]
// - introduce gold::tuples::enable_array_get
//      | the default definition is set to 'false'
//      | when the specialization is set to 'true'
//      | 'gold::tuples::get' will instead look for
//      | the subscript operator of the object
//      | instead of 'get' (free or member)

// before:
// struct ArrayLike { ... }
//
// template <>
// struct gold::tuples::tuple_size<ArrayLike> { ... }
//
// template <std::size_t I>
// struct gold::tuples::tuple_element<I, ArrayLike> { using type = /* always-fixed type */; }

//template <>
//inline constexpr bool gold::tuples::enable_array_like<Array> = true;
//
//gold::tuples::array_like<A>
//
//gold::tuples::tuple_element<I, gold::array_like<>>
//
//gold::tuples::enable_array_like<Array> {
//    inline static constexpr std::size_t extent = 5;
//    using type = int;
//
//    // if extent does not exist as member type-alias,
//    // it is defined as 'Array::extent'
//
//    // or 'Array::size()' if valid
//
//    // if element_type does not exist, then
//    // it is defined as 'std::remove_reference_t<decltype(std::declval<Array>()[std::declval<std::size_t>()])>'
//}

int main() {
    gold::struct_array{1, 2};
    gold::struct_array<int, 2> arr;
    gold::struct_array<const char, 2>{"A"};
    arr = {1};

    test<{1}>();
    test<{1, 2}>();
    test<{3.14f, 1.57f, 4.04f}>();
    test<gold::struct_array("1")>();
    test<gold::struct_array({1, 2, 3})>();

    test_string<"One Wish!">();
    test_string<{'A', 'B', 'C'}>();

//    static_assert(gold::tuples::array_like<gold::struct_array<int, 2>>);
//    static_assert(gold::tuples::array_like<gold::struct_string<5>>);
//    static_assert(gold::tuples::tuple_like<gold::struct_array<int, 2>>);
//    static_assert(gold::tuples::tuple_like<gold::struct_string<5>>);

    // still doesn't work below :<
    // auto [letter_A, letter_D] = gold::struct_string("AD");
}
