/// function traits
#include <gold/invocable_traits>
#include <gold/assertion>

void fn0() {}
void fn1(int) {}

int main() {

    static_assert(requires {
        requires std::same_as<gold::invoke_return_t<void()>, void>;
        requires std::same_as<gold::invoke_return_t<int(bool, char)>, int>;
        requires std::same_as<gold::invoke_parameter_t<int(int), 0>, int>;
        requires std::same_as<gold::invoke_parameter_t<void(bool, char), 1>, char>;
        requires gold::invoke_arity_v<void()> == 0;
        requires gold::invoke_arity_v<std::nullptr_t(bool, bool, bool, bool, char)> == 5;
        requires gold::invoke_arity_v<std::size_t(*)()> == 0;
        requires gold::invoke_arity_v<decltype(fn0)> == 0;
    });
}

