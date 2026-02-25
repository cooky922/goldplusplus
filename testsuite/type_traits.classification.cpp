#include <gold/type_traits>

int main() {
    static_assert(gold::is_scalar_numeric_type_v<int>);
    static_assert(gold::is_scalar_numeric_type_v<float>);
    static_assert(!gold::is_scalar_numeric_type_v<std::nullptr_t>);
    static_assert(!gold::is_scalar_numeric_type_v<char>);
    static_assert(!gold::is_scalar_numeric_type_v<bool>);
}
