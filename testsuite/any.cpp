#include <string>
#include <gold/assertion>
#include <gold/any>
#include <gold/functional>
#include <gold/bits/memory/voidify.hpp>

struct Point {
    int x, y;
};

struct Student {
    std::string name;
    std::size_t age;
};

constexpr void any_0() {
    // 0
    [[maybe_unused]] gold::any a0;
    // 1
    [[maybe_unused]] constexpr gold::any a1;
    // 2
    [[maybe_unused]] gold::any a2 = 10;
    // 3
    [[maybe_unused]] gold::any a3 = 3.14;
    // 4
    [[maybe_unused]] gold::any a4 = Point{.x = 1, .y = 3};
    // 5
    [[maybe_unused]] gold::any a5 = Point{.x = 2};
    // 6
    [[maybe_unused]] gold::any a6 { std::in_place_type<int>, 10 };
    // 7
    [[maybe_unused]] gold::any a7 { std::in_place_type<Point> };
    // 8
    [[maybe_unused]] gold::any a8 { std::in_place_type<std::string>, "hello" };
    // 9
    [[maybe_unused]] gold::any a9 { std::in_place_type<std::string>, 10, 'A' };
    // 10
    [[maybe_unused]] gold::any a10 { std::in_place_type<Point>, 10 };
    // 11
    [[maybe_unused]] gold::any a11 { std::in_place_type<Point>, 6, 9 };
    // 12
    [[maybe_unused]] gold::any a12 = a2;
    // 13
    [[maybe_unused]] gold::any a13 = std::move(a12);
}

constexpr void any_1() {
    /// copy move and swap

    gold::any a0;
    gold::system_assert(!a0.has_value());
    // assigning 'a0' from a temporary 'any'
    a0 = gold::any(3.14);
    gold::system_assert(a0.has_value() && gold::holds_current_type<double>(a0));

    /// Swapping
    gold::any a1 = "hello";
    gold::system_assert(gold::holds_current_type<const char*>(a1));

    gold::any a2 = nullptr;
    gold::system_assert(gold::holds_current_type<std::nullptr_t>(a2));

    a1.swap(a2);

    gold::system_assert(gold::holds_current_type<const char*>(a2));
    gold::system_assert(gold::holds_current_type<std::nullptr_t>(a1));

    a1.reset();

    a1.swap(a2);

    gold::system_assert(a1.has_value() && !a2.has_value());

    a2 = a1;
    gold::system_assert(a1 == a2);

    a1 = 101;
    a2 = 102;
    gold::system_assert(a1 < a2);

    gold::any a3 = std::array{1, 2};
    gold::any a4 = std::move(a3);
    a4 = std::move(a4);
    a3 = std::move(a4);

}

constexpr void any_2() {

    struct non_equal {
        constexpr bool operator==(const non_equal&) = delete;
    };

    gold::any a = non_equal{};

    // below will throw 'gold::bad_any_access(no_spaceship_operator)'
    // a <=> non_equal{};

    static_assert(std::equality_comparable<gold::any>);
    static_assert(std::equality_comparable_with<gold::any, int>);
    static_assert(!std::equality_comparable_with<gold::any, non_equal>);
    static_assert(std::totally_ordered<gold::any>);
    static_assert(std::totally_ordered_with<gold::any, int>);

    a.reset();

    gold::system_assert(a == gold::any{});

    a = 3.14;

    gold::system_assert(a != gold::any{} && gold::any{} != a);

}

constexpr void any_3() {
    constexpr gold::view_any a1;

    static constexpr int x = 10;
    constexpr void* ptr = gold::voidify(x);

    constexpr gold::view_any a2 = x;
    static_assert(gold::holds_current_type<int>(a2));

    int z = 10;
    gold::view_any a3 (gold::in_place_viewable, &z);
    gold::system_assert(gold::holds_current_type<int*>(a3));
    gold::system_assert(&z == gold::any_cast<int*>(gold::in_place_viewable, a3));

    a3 = gold::view_any(gold::in_place_viewable, auto("Hello"));
    gold::system_assert(gold::holds_current_type<const char*>(a3));
    const char* s = gold::any_cast<const char*>(gold::in_place_viewable, a3);
    gold::system_assert(s[0] == 'H'
                     && s[1] == 'e'
                     && s[2] == 'l'
                     && s[3] == 'l'
                     && s[4] == 'o');

    static_assert(!std::constructible_from<gold::view_any, gold::in_place_viewable_t, int(*)()>);
}

constexpr void any_4() {
    gold::view_any a;
    gold::system_assert(!a.has_value());
    int x;
    a = x;
    gold::system_assert(a.has_value() && gold::holds_current_type<int>(a));
    float f;
    a = f;
    gold::system_assert(a.has_value() && gold::holds_current_type<float>(a));
}

static_assert(!gold::view_any().has_value());

constexpr void any_5() {
    int x;
    gold::view_any a;
    a = x;
    gold::system_assert(a.has_value() && gold::holds_current_type<int>(a));
    gold::any_cast<int>(a) = 10;
    gold::system_assert(x == 10);
}

constexpr void any_6() {
    int x = 0;
    gold::view_any a;
    a = x;
    gold::system_assert(a.has_value() && gold::holds_current_type<int>(a));
    gold::any_cast<int>(a) = 10;
    gold::system_assert(x == 10);
}

constexpr void any_7() {
    const int x = 0;
    gold::view_any a = x;
    gold::system_assert(a == 0);
}

constexpr void any_8() {
    bool b = true;
    gold::view_any x { b };
    gold::view_any y = std::move(x);
    gold::system_assert(!x.has_value() && y.has_value());
    // y.swap(x);
    x.swap(y);
    // x = std::move(y);
    gold::system_assert(!y.has_value() && x.has_value());
}

/// [gold.any]
/// - replacing 'gold::bad_any_cast' with 'gold::bad_any_access' [ / ]
/// - implementing equal operator==     [ / ]
/// - implementing spaceship operator<=>
/// - reimplementing 'gold::__any::basic_any_handler::s_swap_'
///   by adding 'if constexpr (std::swappable)' condition to fix some
///   non-swappable types (internally non-move assignable)
/// - fixing 'operator==' and 'operator<=>' for no-value 'any' (patch 2022.2.1)

// revised rules for 'any-like' comparison ('operator==')
// 0.0 - let both 'lhs' and 'rhs' be the parameter
// 0.1 - if both 'lhs' and 'rhs' have no value, then return 'true'
// 0.2 - if either 'lhs' or 'rhs' has no value, then return 'false'
// 0.3 - if the contained type of either 'lhs' or 'rhs' has no defined 'operator==',
//       then will throw an exception
// 0.4 - if the contained types of 'lhs' and 'rhs' are not the same,
//       then return 'false'
// 0.5 - otherwise, compare then using 'operator=='

// revised rules for 'any-like' three-way comparison ('operator<=>')
// 0.0 - let both 'lhs' and 'rhs' be the parameter
// 0.1 - let the return type be 'std::partial_ordering'
// 0.2 - if both 'lhs' and 'rhs' have no value, then return '::equivalent'
// 0.3 - if either 'lhs' or 'rhs' has no value, then return '::unordered'
// 0.4 - if the contained type of either 'lhs' or 'rhs' has no defined 'operator<=>',
//       then will throw an exception
// 0.5 - if the contained types of 'lhs' and 'rhs' are not the same,
//       then return '::unordered'
// 0.6 - otherwise, compare then using 'operator<=>'

int main() {
    gold::consteval_invoke(any_0);
    gold::consteval_invoke(any_1);
    gold::consteval_invoke(any_2);
    gold::consteval_invoke(any_3);
    gold::consteval_invoke(any_4);
    gold::consteval_invoke(any_5);
    gold::consteval_invoke(any_6);
    gold::consteval_invoke(any_7);
    gold::consteval_invoke(any_8);

//    any_0();
//    any_1();
//    any_2();
}
