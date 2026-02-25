#include <gold/bits/functional/overload.hpp>

void f(int) {}
void f(float) {}

constexpr int foo(int&) { return 1; }
constexpr int foo(const int&) { return 2; }
constexpr int foo(int&&) { return 3; }

consteval void test0() {
//    auto fn = gold::make_overload(static_cast<int(*)(int&)>(&foo),
//                                  static_cast<int(*)(const int&)>(&foo),
//                                  static_cast<int(*)(int&&)>(&foo));

}

int main() {
    auto overloaded_fn = gold::make_overload(static_cast<void(*)(int)>(&f),
                                             static_cast<void(*)(float)>(&f));

    static_assert(sizeof(overloaded_fn) == 16);

    // TODO:
    auto new_f = gold::make_overload(gold::non_type<static_cast<void(*)(int)>(&f)>,
                                     gold::non_type<static_cast<void(*)(float)>(&f)>);

    static_assert(sizeof(new_f) == 1);
}
