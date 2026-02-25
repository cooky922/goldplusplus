#include <iostream>
#include <gold/dynamic_library>
#include <gold/assertion>
#include <gold/demangling>

int main() {

    constexpr const char* arithmetic_path = "C:\\Users\\Admin\\Documents\\DESMOND-FROM-OLD-LAPPY\\Programming\\C++\\libs-sandbox\\DLL Practice\\arithmetic.dll";
    gold::dynlib mod = gold::dynlib::unchecked_load(arithmetic_path);

    // is_odd
    auto is_odd = mod.extract_function<bool(int)>("_ZN10arithmetic6is_oddEi");

    gold::system_assert(is_odd(1));
    gold::system_assert(!is_odd(8));

    // power
    auto power = mod.extract_function<int(int, int)>("_ZN10arithmetic5powerEii");

    gold::system_assert(power(2, 3) == 8);
    gold::system_assert(power(5, 2) == 25);
    gold::system_assert(power(3, 4) == 81);

    auto symbols = mod.extract_symbols();
    std::cout << "there are total of " << symbols.size() << " symbols: " << '\n';
    for (const auto& elem : symbols)
        std::cout << elem.name() << '\n';

    std::cout << '\n' << "demangled forms: " << '\n';
    for (const auto& elem : symbols)
        std::cout << gold::demangle(elem.name()) << '\n';

    // gold::system_assert(mod.entity_invoke<int>("_ZN10arithmetic3lcmEii", 15, 10) == 30);

    std::cout << '\n';

    std::cout << "loaded size:   " << (mod.size_bytes() / 1024) << " kb" << '\n';
    std::cout << "base address:  " << mod.native_handle() << '\n';
    std::cout << "entry point:   " << mod.entrypoint_address() << '\n';
    std::cout << "name:          " << mod.name() << '\n';
    std::cout << "location path: " << mod.location_path() << '\n';

    // list currently loaded modules
    std::cout << '\n';

    std::cout << "currently loaded modules:" << '\n';
    for (gold::dynlib lib : gold::dynlib::loaded_libraries())
        std::cout << lib.location_path() << '\n';
}
