#include <gold/bits/type_traits/constant.hpp>
#include <bits/move.h>

template <auto F>
struct fn_as_nttp_t {
    template <typename... Args>
    static constexpr decltype(auto) operator()(Args&&... args)
        noexcept(noexcept(F(std::forward<Args>(args)...)))
    { return F(std::forward<Args>(args)...); }

    constexpr operator decltype(F)() const noexcept
    { return F; }
};

template <auto F>
inline constexpr fn_as_nttp_t<F> fn_as_nttp {};

void foo() {
    __builtin_printf("hello foo\n");
}

int main() {
    auto p = fn_as_nttp<&foo>;
    p();

#if 0
    // identity function
    (phi<0>)(arg);

    // doubling
    (phi<0> + phi<0>)(arg); // the same as 'arg + arg'

    // calling
    (phi<0>())(arg); // the same as 'arg()'
#endif
}
