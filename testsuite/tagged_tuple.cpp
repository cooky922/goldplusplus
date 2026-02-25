#include <gold/tagged_tuple>

int main() {
    using Point = gold::tagged_tuple<
        gold::tagged_member<"x", int>,
        gold::tagged_member<"y", int>
    >;
    constexpr Point p { 1, 2 };
    static_assert(gold::tagged_get<"x">(p) == 1);
    static_assert(gold::tagged_get<"y">(p) == 2);

    auto [x, y] = p;
}
