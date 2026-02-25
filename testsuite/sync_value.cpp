#include <gold/sync_value>
#include <thread>
#include <iostream>

gold::sync_value scout (std::ref(std::cout));

int main() {
    std::jthread th1 ([] {
        gold::apply_sync([](std::ostream& os){ os << "Thread 1" << '\n'; }, scout);
        // scout.apply_sync([](std::ostream& os){ os << "Thread 1" << '\n'; });
    });
    std::jthread th2 ([] {
        gold::apply_sync([](std::ostream& os){ os << "Thread 2" << '\n'; }, scout);
        // scout.apply_sync([](std::ostream& os){ os << "Thread 2" << '\n'; });
    });
}
