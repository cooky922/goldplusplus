#include <gold/assertion>
#include <gold/functions>
#include <functional>

consteval void test0() {
    auto closure = [i = 0] mutable { return i++; };
    gold::function_ref<int()> return_int = closure;
    gold::system_assert(return_int != nullptr);
    gold::system_assert(return_int() == 0);
    gold::system_assert(return_int() == 1);
    gold::system_assert(return_int() == 2);

    gold::function_ref<int() const> fn;
    gold::system_assert(fn == nullptr);

    fn = std::type_identity_t<int(*)()>(nullptr);
    gold::system_assert(fn == nullptr, "error");
}

consteval void test1() {
    struct Hello {
        std::string_view msg;
        constexpr void set_message(std::string_view op) { msg = op; }
        constexpr std::string_view get_message() const { return msg; }
    };
    gold::function_ref<auto(Hello&, std::string_view) -> void> set_message = gold::constant_arg<&Hello::set_message>;
    gold::function_ref<auto(const Hello) -> std::string_view> get_message  = gold::constant_arg<&Hello::get_message>;

    Hello hello;
    set_message(hello, "Hello");
    gold::system_assert(get_message(hello) == "Hello");
    gold::system_assert(get_message(Hello{"hi"}) == "hi");
}

consteval void test2() {
    struct Point {
        int x, y;

        static constexpr Point get_origin() { return {0, 0}; }
        static constexpr int get_x(Point p) { return p.x; }
        static constexpr int get_y(Point p) { return p.y; }

        constexpr int return_times() const noexcept { return x * y; }

    } p { 23, 34 };

    gold::function_ref<int(Point)> get_component = gold::constant_arg<&Point::x>;
    gold::system_assert(get_component(p) == 23);

    get_component = gold::constant_arg<&Point::get_x>;
    gold::system_assert(get_component(p) == 23);

    get_component = gold::constant_arg<&Point::y>;
    gold::system_assert(get_component(p) == 34);

    get_component = gold::constant_arg<&Point::get_y>;
    gold::system_assert(get_component(p) == 34);

    get_component = gold::constant_arg<&Point::return_times>;
    gold::system_assert(get_component(p) == 23 * 34);

}

consteval void test3() {

    struct A {
        int x;
        constexpr int return_one() { return 1; }
        constexpr int return_x() const { return x; }
    };

    gold::function_ref<int(A&&)> fn = gold::constant_arg<&A::return_one>;
    gold::system_assert(fn(A{}) == 1);

    // error:
    // fn = std::mem_fn(&A::return_x);
    fn = gold::constant_arg<&A::return_x>;
    gold::system_assert(fn(A{10}) == 10);
    gold::system_assert(fn(A{69}) == 69);
}

constexpr void change(int& lhs, int&& rhs) {
    lhs = rhs;
}

consteval void test4() {
    int val = 10;
    gold::system_assert(val == 10);
    gold::function_ref<void(int&, int&&)> fn = &change;
    fn(val, 69);
    gold::system_assert(val == 69);
}

consteval void test5() {
    auto closure = [mut = 0] mutable { return mut++; };
    gold::function_ref<int()> fn = closure;
    gold::function_ref<int()> fn_2 = fn;

    gold::system_assert(fn == fn_2);

    fn = []{ return 101; };

    gold::system_assert(fn != fn_2);

    gold::system_assert(fn_2() == 0);
    gold::system_assert(fn_2() == 1);
    gold::system_assert(fn_2() == 2);
    gold::system_assert(fn() == 101);

}

struct Point {
    int x, y;
    constexpr int get_x() const { return x; }
    constexpr int get_y() const { return y; }
};

consteval void test6() {
    Point p { 4, 5 };
    gold::function_ref<int() const> fn { gold::constant_arg<&Point::x>, p };

    gold::system_assert(fn() == 4);

    fn = gold::function_ref<int() const>{ gold::constant_arg<&Point::y>, p };

    gold::system_assert(fn() == 5);
}

// https://github.com/zhihaoy/nontype_functional/blob/main/include/std23/function_ref.h

int main() {
    test0();
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
}
