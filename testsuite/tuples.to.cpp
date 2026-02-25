#include <gold/tuples>
#include <array>

struct Triple {
    int x;
    int y;
    int z;
};

int main() {
    std::tuple ints_of_three {0, 1, 2};
    std::array ints_array = gold::tuples::to<std::array<int, 3>>(ints_of_three);
    // auto triple = gold::tuples::to<Triple>(ints_of_three);
}
