#include <gold/inplace_vector>
#include <gold/bits/assume.hpp>

consteval void test0() {
    gold::inplace_vector<int, 10> small_vec;
    small_vec.push_back(5);
    small_vec.push_back(6);
    small_vec.push_back(1);

    gold::assume(small_vec.capacity() == 10);
    gold::assume(small_vec.size() == 3);
    gold::assume(small_vec[0] == 5);
    gold::assume(small_vec[1] == 6);
    gold::assume(small_vec[2] == 1);

    small_vec.clear();

    gold::assume(small_vec.size() == 0);
}

struct A {
    A() { __builtin_printf("hello A at %p\n", this); }
    ~A() { __builtin_printf("goodbye A at %p\n", this); }
};

int main() {
    test0();

    // supports non-trivial type but it is no longer constexpr
    gold::inplace_vector<A, 5> vec;
    vec.emplace_back();
    vec.emplace_back();
}

