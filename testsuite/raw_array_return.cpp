#include <cstddef>

template <std::size_t N>
constexpr auto& return_array(int (& arr)[N]) { return arr; }

int main() {
    auto&& arr = (int[]){1, 2, 3, 4, 5};
    return_array(arr);
}
