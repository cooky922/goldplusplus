#include <gold/bits/type_traits/constant.hpp>

int main() {
    static_assert(gold::constant<1> == gold::constant<1>);
    static_assert(gold::constant<2> != gold::constant<3>);

    static_assert((gold::constant<2> * (-gold::constant<3>)) == -6);
}
