#include <ranges>
#include <utility>
#include <gold/dynamic_library>

#include <windows.h>
#include <psapi.h>
#include <dbghelp.h>
#include <imagehlp.h>

/// requires additional linker flags:
/// -ldbghelp -limagehlp

namespace {

    ::HMODULE __gold_win32_load_library [[maybe_unused]] (std::string_view sv) {
        return ::LoadLibrary(sv.data());
    }

    void __gold_win32_free_library [[maybe_unused]] (::HMODULE hmod) {
        ::FreeLibrary(hmod);
    }

    ::MODULEINFO __gold_win32_get_module_info [[maybe_unused]] (::HMODULE hmod) {
        ::MODULEINFO result;
        ::GetModuleInformation(::GetCurrentProcess(), hmod, &result, sizeof(result));
        return result;
    }

    std::string __gold_win32_get_module_name [[maybe_unused]] (::HMODULE hmod) {
        char temp_out[1024] {};
        ::GetModuleBaseName(::GetCurrentProcess(), hmod, temp_out, 1024);
        return std::string(temp_out);
    }

    std::string __gold_win32_get_image_name [[maybe_unused]] (::HMODULE hmod) {
        char temp_out[1024] {};
        ::GetModuleFileNameEx(::GetCurrentProcess(), hmod, temp_out, 1024);
        return std::string(temp_out);
    }

    auto __gold_win32_get_proc_addr [[maybe_unused]] (::HMODULE hmod, std::string_view sv) {
        return ::GetProcAddress(hmod, sv.data());
    }


}

namespace gold {

    dynamic_library::dynamic_library(std::string_view name)
    : m_handle_(__gold_win32_load_library(name)) {}

    dynamic_library::dynamic_library(dynamic_library&& other) noexcept
    : m_handle_(std::exchange(other.m_handle_, nullptr)) {}

    dynamic_library::~dynamic_library() { this->reset(); }

    dynamic_library& dynamic_library::operator=(dynamic_library&& other) noexcept {
        if (this == &other)
            return *this;
        m_handle_ = std::exchange(other.m_handle_, nullptr);
        return *this;
    }

    bool dynamic_library::has_loaded() const noexcept { return m_handle_; }

    dynamic_library::operator bool() const noexcept { return this->has_loaded(); }

    void dynamic_library::reset() {
        if (*this)
            __gold_win32_free_library(m_handle_);
    }

    void dynamic_library::reset(std::string_view name) {
        this->reset();
        m_handle_ = __gold_win32_load_library(name);
    }

    const void* dynamic_library::base_address() const {
        return __gold_win32_get_module_info(m_handle_).lpBaseOfDll;
    }

    const void* dynamic_library::entry_point_address() const {
        return __gold_win32_get_module_info(m_handle_).EntryPoint;
    }

    std::size_t dynamic_library::size() const {
        return __gold_win32_get_module_info(m_handle_).SizeOfImage;
    }

    std::string dynamic_library::name() const {
        return this->module_name();
    }

    std::string dynamic_library::module_name() const {
        return __gold_win32_get_module_name(m_handle_);
    }

    std::string dynamic_library::image_name() const {
        return __gold_win32_get_image_name(m_handle_);
    }

    std::vector<std::string> dynamic_library::symbol_names() const {
        ::LOADED_IMAGE           l_loaded_img_;
        std::vector<std::string> l_syms_;

        if (::MapAndLoad(this->image_name().c_str(), nullptr, &l_loaded_img_, true, true)) {
            auto l_directory_size_      = 0ul;
            auto l_image_exp_directory_ = static_cast<::IMAGE_EXPORT_DIRECTORY*>(
                ::ImageDirectoryEntryToData(
                    l_loaded_img_.MappedAddress, false,
                    IMAGE_DIRECTORY_ENTRY_EXPORT,
                    &l_directory_size_
                )
            );
            if (l_image_exp_directory_ != nullptr) {
                auto l_name_rva_ = static_cast<::DWORD*>(
                    ::ImageRvaToVa(
                        l_loaded_img_.FileHeader,
                        l_loaded_img_.MappedAddress,
                        l_image_exp_directory_->AddressOfNames,
                        nullptr
                    )
                );
                for (std::size_t i = 0; i < l_image_exp_directory_->NumberOfNames; ++i) {
                    l_syms_.emplace_back(
                        static_cast<char*>(::ImageRvaToVa(
                            l_loaded_img_.FileHeader,
                            l_loaded_img_.MappedAddress,
                            l_name_rva_[i],
                            nullptr
                        ))
                    );
                }
            }
            ::UnMapAndLoad(&l_loaded_img_);
        }
        return l_syms_;
    }

    std::vector<gold::dl_symbol> dynamic_library::symbols() const {
        auto transform_this = std::views::transform([this](std::string_view sv) { return this->symbol(sv); });
        auto result         = this->symbol_names() | transform_this;
        return std::vector(result.begin(), result.end());
    }

    gold::dl_symbol dynamic_library::symbol(std::string_view name) const {
        return __gold_win32_get_proc_addr(m_handle_, name);
    }


} // namespace gold
