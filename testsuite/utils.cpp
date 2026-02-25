#include <gold/bits/__util/are_related_ptrs.hpp>
#include <gold/bits/__util/cast_from_vptr.hpp>
#include <gold/bits/__util/is_ptr_in_range.hpp>
#include <gold/bits/__util/is_valid_consteval.hpp>
#include <gold/bits/__util/is_valid_ptr.hpp>
#include <gold/bits/__util/uneval.hpp>
#include <gold/bits/assume.hpp>
#include <gold/bits/memory/ops.hpp>

consteval void is_ptr_in_range_test0() {
    const char str[] = "Gold";
    const char* ptr = str;

    gold::assume(gold::__util::is_ptr_in_range(str, str + 4, ptr));
    gold::assume(!gold::__util::is_ptr_in_range(str + 1, str + 4, ptr));
    gold::assume(gold::__util::is_ptr_in_range(str + 1, str + 4, str + 3));
    gold::assume(!gold::__util::is_ptr_in_range(str + 1, str + 4, str + 4));
}

consteval void is_valid_ptr_test0() {
    // pointer must not be default initialized (has indeterminate value) in order
    // to be used in constant expression

    int* ptr = nullptr;
    gold::assume(!gold::__util::is_valid_ptr(ptr));
    int val = 43;
    gold::assume(gold::__util::is_valid_ptr(&val));
    ptr = &val;
    gold::assume(gold::__util::is_valid_ptr(ptr));
    ptr = nullptr;
    gold::assume(!gold::__util::is_valid_ptr(ptr));

    ptr = new int (5);
    gold::assume(gold::__util::is_valid_ptr(ptr));
    delete ptr;
    gold::assume(!gold::__util::is_valid_ptr(ptr));

    union {
        int   x;
        float y;
    };
    gold::assume(!gold::__util::is_valid_ptr(&x));
    gold::assume(!gold::__util::is_valid_ptr(&y));
    gold::construct_at(&x);
    x = 101;
    gold::assume(gold::__util::is_valid_ptr(&x));
    gold::assume(!gold::__util::is_valid_ptr(&y));
    gold::construct_at(&y);
    y = 3.14f;
    gold::assume(!gold::__util::is_valid_ptr(&x));
    gold::assume(gold::__util::is_valid_ptr(&y));
}

consteval void uneval_test0() {
    static_assert(__is_same(decltype(gold::__util::uneval<int>()), int));
    static_assert(__is_same(decltype(gold::__util::uneval<int&>()), int&));
    static_assert(__is_same(decltype(gold::__util::uneval<int&&>()), int&&));
    static_assert(!__is_same(decltype(gold::__util::uneval<const int>()), const int));
    static_assert(__is_same(decltype(gold::__util::uneval<const int&>()), const int&));
    static_assert(__is_same(decltype(gold::__util::uneval<const int&&>()), const int&&));
    static_assert(__is_same(decltype(gold::__util::uneval<void>()), void));
    static_assert(__is_same(decltype(gold::__util::uneval<int*>()), int*));
    static_assert(__is_same(decltype(gold::__util::uneval<const int*>()), const int*));
    static_assert(__is_same(decltype(gold::__util::uneval<const int* const&&>()), const int* const&&));

    static_assert(__is_same(decltype(gold::__util::uneval<void(&)()>()), void(&)()));
    static_assert(__is_same(decltype(gold::__util::uneval<void(*)()>()), void(*)()));
    static_assert(__is_same(decltype(gold::__util::uneval<void(*)(...)>()), void(*)(...)));
    static_assert(__is_same(decltype(gold::__util::uneval<void()>()), void(&)()));
    static_assert(__is_same(decltype(gold::__util::uneval<void(...)>()), void(&)(...)));

    static_assert(__is_same(decltype(gold::__util::uneval<int[]>()), int(&&)[]));
    static_assert(__is_same(decltype(gold::__util::uneval<int[1]>()), int(&&)[1]));
    static_assert(__is_same(decltype(gold::__util::uneval<int(&)[]>()), int(&)[]));
    static_assert(__is_same(decltype(gold::__util::uneval<int(&)[1]>()), int(&)[1]));
    static_assert(__is_same(decltype(gold::__util::uneval<int(&&)[]>()), int(&&)[]));
    static_assert(__is_same(decltype(gold::__util::uneval<int(&&)[1]>()), int(&&)[1]));

    struct Class {
        int mem;

        void fn() const {}
    };

    static_assert(__is_same(decltype(gold::__util::uneval<int Class::*>()), int Class::*));

    struct A {
        virtual ~A() = default;
        virtual void f() = 0;
    };

    static_assert(__is_same(decltype(gold::__util::uneval<A>()), A));

    struct Incomplete;

    static_assert(__is_same(decltype(gold::__util::uneval<Incomplete>()), Incomplete));

    /////////////////////////
    static_assert(__is_same(decltype(gold::__util::declval<int>()), int&&));
    static_assert(__is_same(decltype(gold::__util::declval<int&>()), int&));
    static_assert(__is_same(decltype(gold::__util::declval<int&&>()), int&&));
    static_assert(__is_same(decltype(gold::__util::declval<A>()), A&&));
    static_assert(__is_same(decltype(gold::__util::declval<A&>()), A&));
    static_assert(__is_same(decltype(gold::__util::declval<void>()), void));

}

int main() {
    is_ptr_in_range_test0();
    is_valid_ptr_test0();
    uneval_test0();
}
