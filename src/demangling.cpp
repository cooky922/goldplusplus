#include <gold/demangling>
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

namespace gold {

    /// demangle
    std::string demangle(std::string_view name) noexcept {

        using smart_ptr_type = std::unique_ptr<char, void(*)(void*)>;

        int status_value;
        smart_ptr_type ptr (
            __cxxabiv1::__cxa_demangle(name.data(), nullptr, nullptr, &status_value),
            &std::free
        );

        if (status_value != 0)
            return std::string(name);
        else
            return ptr.get();
    }


} // namespace gold
