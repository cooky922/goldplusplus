#include <gold/bits/memory/ops_algo.hpp>
#include <string>
#include <span>
#include <gold/assertion>

struct S {};

struct S0 {
    int n;
    constexpr S0() {}
};

consteval void test0() {
    using alloc_type = std::allocator_traits<std::string::allocator_type>::rebind_alloc<std::string>;
    alloc_type alloc;
    std::string* ptr = alloc.allocate(5);
    gold::uninit_fill_n(ptr, 5, "Hello");
    gold::system_assert(ptr[0] == ptr[1] && ptr[0] == "Hello");
    alloc.deallocate(ptr, 5);
}

consteval void test1() {
    int x = 10;
    // gold::destroy_at(&x); // this is also error "destroying 'x' outside its lifetime called after scope"
    // x = 91; // this is error even if it is trivial type because it's already destroyed

    std::string s = "Hello";
    gold::destroy_at(&s);
    gold::construct_at(&s, 5, '*');
    gold::system_assert(s == "*****");
}

int main() {
    test0();
    test1();
}
