#include <algorithm>
#include <memory>
#include <vector>
#include <ranges>
#include <gold/dynamic_library>
#include <gold/utility>
#include <windows.h>
#include <psapi.h>
#include <dbghelp.h>
#include <imagehlp.h>

/// requires additional linker flags:
/// -ldbghelp -limagehlp

namespace __goldx {

    /// __goldx::dl_module_storage_max_size
    [[maybe_unused]] static constexpr std::size_t dl_module_storage_max_size = 128;

    /// __goldx::dl_module_storage_max_align
    [[maybe_unused]] static constexpr std::size_t dl_module_storage_max_align = 8;

    /// __goldx::dl_module_byte_array_ref
    using dl_module_byte_array_ref = std::byte (&) [dl_module_storage_max_size];

    /// __goldx::dl_module_storage_impl
    struct dl_module_storage_impl {
        std::vector<std::string>     m_sym_names_;
        std::vector<gold::dl_symbol> m_syms_;
        std::string                  m_mod_name_;
        std::string                  m_img_name_;
        bool                         m_auto_free_ = false;
        ::HINSTANCE__*               m_handle_    = nullptr;

        void swap(dl_module_storage_impl& other) noexcept {
            using std::swap;
            swap(m_sym_names_, other.m_sym_names_);
            swap(m_syms_, other.m_syms_);
            swap(m_mod_name_, other.m_mod_name_);
            swap(m_img_name_, other.m_img_name_);
            swap(m_auto_free_, other.m_auto_free_);
            swap(m_handle_, other.m_handle_);
        }

        static dl_module_storage_impl* s_get_(dl_module_byte_array_ref data) {
            return std::launder(reinterpret_cast<dl_module_storage_impl*>(data));
        }

        static dl_module_storage_impl* s_init_(dl_module_byte_array_ref data) {
             return std::construct_at(s_get_(data));
        }

        static void s_destroy_(dl_module_byte_array_ref data) {
            std::destroy_at(s_get_(data));
        }
    };

    /// __goldx::load_library
    static ::HMODULE load_library [[maybe_unused]] (std::string_view sv) {
        return ::LoadLibrary(sv.data());
    }

    /// __goldx::get_library
    static ::HMODULE get_library [[maybe_unused]] (std::string_view sv) {
        return ::GetModuleHandle(sv.data());
    }

    /// __goldx::is_library_loaded
    static bool is_library_loaded [[maybe_unused]] (std::string_view sv) {
        return get_library(sv) != nullptr;
    }

    /// __goldx::safe_gotten_library_result
    struct safe_gotten_library_result {
        ::HMODULE mod;
        bool      auto_free;
    };

    /// __goldx::safe_get_library
    static safe_gotten_library_result safe_get_library [[maybe_unused]] (std::string_view sv) {
        if (auto result = get_library(sv); result != nullptr)
            return { result, false };
        else
            return { load_library(sv), true };
    }

    /// __goldx::free_library
    static void free_library [[maybe_unused]] (::HMODULE hmod) {
        ::FreeLibrary(hmod);
    }

    /// __goldx::get_library_info
    static ::MODULEINFO get_library_info [[maybe_unused]] (::HMODULE hmod) {
        ::MODULEINFO result;
        ::GetModuleInformation(::GetCurrentProcess(), hmod, &result, sizeof(result));
        return result;
    }

    /// __goldx::get_library_name
    static std::string get_library_name [[maybe_unused]] (::HMODULE hmod) {
        char temp_out [1024] {};
        ::GetModuleBaseName(::GetCurrentProcess(), hmod, temp_out, 1024);
        return std::string(temp_out);
    }

    /// __goldx::get_library_image_name
    static std::string get_library_image_name [[maybe_unused]] (::HMODULE hmod) {
        char temp_out [1024] {};
        ::GetModuleFileNameEx(::GetCurrentProcess(), hmod, temp_out, 1024);
        return std::string(temp_out);
    }

    /// __goldx::get_proc_addr
    static auto get_proc_addr [[maybe_unused]] (::HMODULE hmod, std::string_view fname) {
        return ::GetProcAddress(hmod, fname.data());
    }

    /// __goldx::get_storage
    static auto get_storage [[maybe_unused]] (dl_module_byte_array_ref data) {
        return dl_module_storage_impl::s_get_(data);
    }

    /// __goldx::swap_storage
    static void swap_storage [[maybe_unused]] (dl_module_byte_array_ref lhs, dl_module_byte_array_ref rhs) {
        get_storage(lhs)->swap(*get_storage(rhs));
    }

    /// __goldx::get_symbol_names
    static auto get_symbol_names [[maybe_unused]] (std::string_view fname) {
        ::LOADED_IMAGE           loaded_img;
        std::vector<std::string> result;

        if (::MapAndLoad(fname.data(), nullptr, &loaded_img, true, true)) {
            auto dir_size    = 0ul;
            auto img_exp_dir = static_cast<::IMAGE_EXPORT_DIRECTORY*>(
                ::ImageDirectoryEntryToData(
                    loaded_img.MappedAddress, false,
                    IMAGE_DIRECTORY_ENTRY_EXPORT,
                    &dir_size
                )
            );
            if (img_exp_dir != nullptr) {
                auto name_rva = static_cast<::DWORD*>(
                    ::ImageRvaToVa(
                        loaded_img.FileHeader,
                        loaded_img.MappedAddress,
                        img_exp_dir->AddressOfNames,
                        nullptr
                    )
                );
                for (std::size_t i = 0; i < img_exp_dir->NumberOfNames; ++i) {
                    result.emplace_back(
                        static_cast<char*>(::ImageRvaToVa(
                            loaded_img.FileHeader,
                            loaded_img.MappedAddress,
                            name_rva[i],
                            nullptr
                        ))
                    );
                }
            }
            ::UnMapAndLoad(&loaded_img);
        }
        return result;
    }

    /// __goldx::contains_one
    static bool contains_one(const std::vector<std::string>& vect, std::string_view elem) {
        for (const std::string& i : vect) {
            if (i == elem)
                return true;
        }
        return false;
    }


} // namespace __goldx

namespace gold {

    /// dl_module ctors
    dl_module::dl_module(std::string_view name) {
        auto [handle, auto_free] = __goldx::safe_get_library(name);
        __goldx::dl_module_storage_impl::s_init_(m_data_);
        auto* m_data_ptr_ = __goldx::get_storage(m_data_);
        m_data_ptr_->m_handle_    = handle;
        m_data_ptr_->m_auto_free_ = auto_free;
    }

    dl_module::dl_module(dl_module&& other) noexcept : dl_module() {
        other.swap(*this);
    }

    /// dl_module dtor
    dl_module::~dl_module() {
        if (this->has_value() && __goldx::get_storage(m_data_)->m_auto_free_)
            __goldx::free_library(__goldx::get_storage(m_data_)->m_handle_);
        __goldx::dl_module_storage_impl::s_destroy_(m_data_);
    }

    /// dl_module assignment
    dl_module& dl_module::operator=(dl_module&& other) noexcept {
        gold::decay_move(other).swap(*this);
        return *this;
    }

    /// has_value
    bool dl_module::has_value() const noexcept {
        return __goldx::get_storage(const_cast<dl_module*>(this)->m_data_)->m_handle_;
    }

    /// operator bool
    dl_module::operator bool() const noexcept { return this->has_value(); }

    /// swap
    void dl_module::swap(dl_module& other) noexcept {
        __goldx::swap_storage(m_data_, other.m_data_);
    }

    /// reset
    void dl_module::reset() {
        if (this->has_value()) {
            auto* m_data_ptr_ = __goldx::get_storage(m_data_);
            m_data_ptr_->m_auto_free_ = false;
            m_data_ptr_->m_sym_names_.clear();
            m_data_ptr_->m_syms_.clear();
            m_data_ptr_->m_mod_name_.clear();
            m_data_ptr_->m_img_name_.clear();
            __goldx::free_library(m_data_ptr_->m_handle_);
            m_data_ptr_->m_handle_ = nullptr;
        }
    }

    void dl_module::reset(std::string_view name) {
        this->reset();
        *this = dl_module(name);
    }

    /// base_address
    const void* dl_module::base_address() const {
        return __goldx::get_library_info(__goldx::get_storage(const_cast<dl_module*>(this)->m_data_)->m_handle_).lpBaseOfDll;
    }

    /// entry_point_address
    const void* dl_module::entry_point_address() const {
        return __goldx::get_library_info(__goldx::get_storage(const_cast<dl_module*>(this)->m_data_)->m_handle_).EntryPoint;
    }

    /// size
    std::size_t dl_module::size() const {
        return __goldx::get_library_info(__goldx::get_storage(const_cast<dl_module*>(this)->m_data_)->m_handle_).SizeOfImage;
    }

    /// name
    std::string_view dl_module::name() const { return this->module_name(); }

    /// module_name
    std::string_view dl_module::module_name() const {
        if (!this->has_value())
            return "";

        auto* m_data_ptr_ = __goldx::get_storage(const_cast<dl_module*>(this)->m_data_);

        if (m_data_ptr_->m_mod_name_.empty()) {
            m_data_ptr_->m_mod_name_ = __goldx::get_library_name(m_data_ptr_->m_handle_);
        }

        return m_data_ptr_->m_mod_name_;
    }

    /// image_name
    std::string_view dl_module::image_name() const {
        if (!this->has_value())
            return "";

        auto* m_data_ptr_ = __goldx::get_storage(const_cast<dl_module*>(this)->m_data_);

        if (m_data_ptr_->m_img_name_.empty()) {
            m_data_ptr_->m_img_name_ = __goldx::get_library_image_name(m_data_ptr_->m_handle_);
        }

        return m_data_ptr_->m_img_name_;
    }

    /// symbol_names
    std::span<const std::string> dl_module::symbol_names() const {
        if (!this->has_value())
            return {};

        auto* m_data_ptr_ = __goldx::get_storage(const_cast<dl_module*>(this)->m_data_);
        if (m_data_ptr_->m_sym_names_.empty()) {
            m_data_ptr_->m_sym_names_ = __goldx::get_symbol_names(this->image_name());
        }
        return std::span<const std::string>{m_data_ptr_->m_sym_names_};
    }

    /// symbols
    std::span<gold::dl_symbol> dl_module::symbols() const {
        if (!this->has_value())
            return {};

        auto* m_data_ptr_ = __goldx::get_storage(const_cast<dl_module*>(this)->m_data_);
        if (m_data_ptr_->m_syms_.empty()) {
            m_data_ptr_->m_syms_ = [&] {
                auto transform_f = std::views::transform([this](std::string_view sv) { return this->symbol(sv); });
                auto result      = this->symbol_names() | transform_f;
                return std::vector(result.begin(), result.end());
            }();
        }
        return std::span{m_data_ptr_->m_syms_};
    }

    /// symbol
    gold::dl_symbol dl_module::symbol(std::string_view name) const {
        if (!this->has_value())
            return nullptr;
        auto* m_data_ptr_ = __goldx::get_storage(const_cast<dl_module*>(this)->m_data_);
        if (!m_data_ptr_->m_sym_names_.empty()) {
            if (!__goldx::contains_one(m_data_ptr_->m_sym_names_, name))
                throw dynamic_library_error("the symbol name requested does not belong to this module");
        }
        return __goldx::get_proc_addr(m_data_ptr_->m_handle_, name);
    }

} // namespace gold
