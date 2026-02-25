#include <iterator>
#include <gold/ranges>
#include <gold/struct_string>
#include <gold/type_name>

#include <gold/generator>

struct Empty {};

template <auto>
inline constexpr Empty object;

int main() {

    const char* s = "Hello";
    auto p = gold::views::delimit(s, '\0');

    static_assert(!gold::ranges::nested_range<std::string>);
    static_assert(gold::ranges::nested_range<std::ranges::single_view<std::string>>);
    static_assert(!gold::ranges::statically_sized_range<std::string>);
    static_assert(gold::ranges::statically_sized_range<std::array<char, 5>>);
    static_assert(gold::ranges::const_iterable_range<std::string>);
    static_assert(!gold::ranges::const_iterable_range<gold::generator<int>>);

//    static_assert(requires {
//        { std::views::split(' ') | std::views::chunk(2) } -> gold::ranges::range_adaptor_closure_object;
//    });
//
//    constexpr auto s = gold::struct_string("Hello");
//
//    __builtin_printf("%s\n", gold::type_name<decltype(s | std::views::reverse)>().data());

    // TODO:
    // https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2760r1.html
    // + gold::views::chunk_on
    // + gold::views::remove
    // + gold::views::remove_if
    // + gold::views::replace
    // + gold::views::replace_if
    // + gold::views::cache_last [/]
    // + gold::views::delimit [/]
    // + gold::views::slice
    // + gold::views::drop_last
    // + gold::views::take_last

//    object<gold::views::generate>;
//    object<gold::views::repeat>;
//    object<gold::views::maybe>;
//    object<gold::views::move_each>;
//    object<gold::views::const_each>;
//    object<gold::views::enumerate>;
//    object<gold::views::slide>;
//    object<gold::views::stride>;
//    object<gold::views::chunk>;
//    object<gold::views::chunk_by>;
//    object<gold::views::zip>;
//    object<gold::views::zip_transform>;
//    object<gold::views::elements<0>>;
//    object<gold::views::cycle>;
}
