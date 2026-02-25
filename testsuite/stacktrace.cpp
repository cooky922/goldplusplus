#include <iostream>
#include <gold/stacktrace>

void bar() {
    std::cout << gold::stacktrace::current() << '\n';
}

void foo() {
    bar();
}

int main() {
    foo();
}
