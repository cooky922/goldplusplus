#include <gold/bits/ranges/concat_view.hpp>
#include <iostream>
#include <gold/assertion>

int main() {

    [] consteval {
        const auto s0 = {1, 2, 3};
        const auto s1 = {4, 5};
        const auto s2 = {6, 7, 8, 9, 10};

        auto concatted = gold::views::concat(s0, s1, s2);
        gold::system_assert(concatted.size() == 10);

        auto iter = concatted.begin();
        auto sent = concatted.end();

        gold::system_assert(*iter++ == 1);
        gold::system_assert(*iter++ == 2);
        gold::system_assert(*iter++ == 3);
        gold::system_assert(*iter++ == 4);
        gold::system_assert(*iter++ == 5);
        gold::system_assert(*iter++ == 6);
        gold::system_assert(*iter++ == 7);
        gold::system_assert(*iter++ == 8);
        gold::system_assert(*iter++ == 9);
        gold::system_assert(*iter++ == 10);

        iter = concatted.begin();

        gold::system_assert(std::default_sentinel - iter == 10);

        gold::system_assert(*iter == 1);
        iter += 5;
        gold::system_assert(*iter == 6);
        --iter;
        gold::system_assert(*iter == 5);

        gold::system_assert(concatted.end() - concatted.begin() == 10);
    }();

}
