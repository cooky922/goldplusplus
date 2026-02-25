#include <gold/dynamic_library>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

// implementation
namespace gold {

    /// dynlib::CachedImpl
    struct dynlib::CachedImpl {
        std::vector <::IMAGE_SECTION_HEADER> section_headers;
    };

    namespace __dynlib {

        /// __dynlib::native_handle_type
        using native_handle_type = dynlib::native_handle_type;
        using address_type       = dynlib::address_type;

        /// __dynlib::get_pe_header
        static ::PIMAGE_NT_HEADERS get_pe_header(__dynlib::native_handle_type handle) {
            if (!handle)
                return nullptr;
            char* base = static_cast<char*>(handle);
            if (::IsBadReadPtr(base, sizeof(::IMAGE_DOS_HEADER)))
                return nullptr;
            auto* dos = reinterpret_cast<::PIMAGE_DOS_HEADER>(base);
            if (dos->e_magic != IMAGE_DOS_SIGNATURE)
                return nullptr;
            if (::IsBadReadPtr(base + dos->e_lfanew, sizeof(::IMAGE_NT_HEADERS)))
                return nullptr;
            auto* nt = reinterpret_cast<::PIMAGE_NT_HEADERS>(base + dos->e_lfanew);
            if (nt->Signature != IMAGE_NT_SIGNATURE)
                return nullptr;
            return nt;
        }

        /// __dynlib::get_lang_kind
        static lang_kind_impl get_lang_kind(const char* raw_name) {
            if (!raw_name || raw_name[0] == '\0')
                return lang_kind_impl::unknown;
            // MSVC ABI mangling
            if (raw_name[0] == '?')
                return lang_kind_impl::cpp;
            // Itanium CXX ABI mangling
            if (raw_name[0] == '_' && raw_name[1] == 'Z')
                return lang_kind_impl::cpp;
            return lang_kind_impl::c;
        }

        /// __dynlib::get_symbol_address_impl
        static __dynlib::address_type get_symbol_address_impl(__dynlib::native_handle_type handle, const char* name) {
            if (!handle)
                return nullptr;
            return reinterpret_cast<__dynlib::address_type>(::GetProcAddress(static_cast<::HMODULE>(handle), name));
        }

        struct dynlib_friend {
            static __dynlib::item_kind_impl get_symbol_item_kind_impl(const void* ptr, __dynlib::address_type sym_addr) {
                if (!ptr || !sym_addr)
                    return __dynlib::item_kind_impl::unknown;
                dynlib* plib = static_cast<dynlib*>(const_cast<void*>(ptr));
                if (!plib->m_lib_handle_)
                    return __dynlib::item_kind_impl::unknown;
                __dynlib::native_handle_type phandle = plib->m_lib_handle_;
                if (!plib->m_cached_ptr_) {
                    plib->m_cached_ptr_ = new dynlib::CachedImpl;
                    auto nt = __dynlib::get_pe_header(phandle);
                    if (nt) {
                        ::PIMAGE_SECTION_HEADER first = IMAGE_FIRST_SECTION(nt);
                        plib->m_cached_ptr_->section_headers.assign(first, first + nt->FileHeader.NumberOfSections);
                    }
                }
                char* base = static_cast<char*>(phandle);
                if (sym_addr < base)
                    return __dynlib::item_kind_impl::forwarder;

                DWORD rva = static_cast<DWORD>(static_cast<const char*>(sym_addr) - base);
                for (const auto& sec : plib->m_cached_ptr_->section_headers) {
                    if (rva >= sec.VirtualAddress && rva < (sec.VirtualAddress + sec.Misc.VirtualSize)) {
                        if (sec.Characteristics & IMAGE_SCN_MEM_EXECUTE)
                            return __dynlib::item_kind_impl::function;
                        if ((sec.Characteristics & IMAGE_SCN_MEM_READ) ||
                            (sec.Characteristics & IMAGE_SCN_MEM_WRITE))
                            return __dynlib::item_kind_impl::variable;
                    }
                }
                return __dynlib::item_kind_impl::unknown;
            }
        };

        /// __dynlib::get_symbol_item_kind_impl
        [[gnu::always_inline]] inline __dynlib::item_kind_impl get_symbol_item_kind_impl(const void* ptr, __dynlib::address_type sym_addr) {
            return dynlib_friend::get_symbol_item_kind_impl(ptr, sym_addr);
        }


    } // namespace __dynlib

    /// dynlib::reset [impl]
    void dynlib::reset() {
        if (m_lib_handle_ && m_owned_) {
            ::FreeLibrary(static_cast<::HMODULE>(m_lib_handle_));
            m_lib_handle_ = nullptr;
            m_owned_ = false;
        }
        if (m_cached_ptr_) {
            delete m_cached_ptr_;
            m_cached_ptr_ = nullptr;
        }
    }

    /// dynlib::unchecked_load [impl]
    dynlib dynlib::unchecked_load(std::string_view path) noexcept {
        ::HMODULE h = ::LoadLibraryExA(path.data(), nullptr, 0);
        return dynlib{dynlib::SpecialCtor{}, h, true};
    }

    /// dynlib::try_load [impl]
    std::expected<dynlib, dynlib_error_code> dynlib::try_load(std::string_view path) noexcept {
        ::HMODULE h = ::LoadLibraryExA(path.data(), nullptr, 0);
        if (!h) {
            DWORD err = ::GetLastError();
            if (err == ERROR_MOD_NOT_FOUND || err == ERROR_FILE_NOT_FOUND)
                return std::unexpected(dynlib_error_code::library_not_found);
            if (err == ERROR_ACCESS_DENIED)
                return std::unexpected(dynlib_error_code::access_denied);
            if (err == ERROR_BAD_FORMAT)
                return std::unexpected(dynlib_error_code::invalid_format);
            return std::unexpected(dynlib_error_code::load_failed);
        }
        return dynlib{dynlib::SpecialCtor{}, h, true};
    }

    /// dynlib::load [impl]
    dynlib dynlib::load(std::string_view path) {
        auto result = try_load(path);
        if (result.has_value())
            return std::move(*result);
        throw dynlib_error(result.error());
    }

    /// dynlib::unchecked_attach [impl]
    dynlib dynlib::unchecked_attach(std::string_view path) noexcept {
        ::HMODULE h = ::GetModuleHandleA(path.data());
        return dynlib{dynlib::SpecialCtor{}, h, false};
    }

    dynlib dynlib::unchecked_attach(native_handle_type handle) noexcept {
        return dynlib{dynlib::SpecialCtor{}, handle, false};
    }

    /// dynlib::try_attach [impl]
    std::expected<dynlib, dynlib_error_code> dynlib::try_attach(std::string_view path) noexcept {
        ::HMODULE h = ::GetModuleHandleA(path.data());
        if (!h)
            return std::unexpected(dynlib_error_code::library_not_found);
        return dynlib{dynlib::SpecialCtor{}, h, false};
    }

    /// dynlib::attach [impl]
    dynlib dynlib::attach(std::string_view path) {
        auto result = try_attach(path);
        if (result.has_value())
            return std::move(*result);
        throw dynlib_error(result.error());
    }

    /// dynlib::has_already_loaded [impl]
    bool dynlib::has_already_loaded(std::string_view path) noexcept {
        void* h = ::GetModuleHandleA(path.data());
        return h != nullptr;
    }

    /// dynlib::entrypoint_address [impl]
    dynlib::address_type dynlib::entrypoint_address() const noexcept {
        auto nt = __dynlib::get_pe_header(m_lib_handle_);
        return nt ? (static_cast<char*>(m_lib_handle_) + nt->OptionalHeader.AddressOfEntryPoint) : nullptr;
    }

    /// dynlib::size_bytes [impl]
    std::size_t dynlib::size_bytes() const noexcept {
        auto nt = __dynlib::get_pe_header(m_lib_handle_);
        return nt ? nt->OptionalHeader.SizeOfImage : 0;
    }

    /// dynlib::location_path [impl]
    std::string dynlib::location_path() const {
        if (!m_lib_handle_)
            return "";
        char buffer[MAX_PATH];
        if (!::GetModuleFileNameA(static_cast<::HMODULE>(m_lib_handle_), buffer, MAX_PATH))
            return "";
        return std::string(buffer);
    }

    /// dynlib::name [impl]
    std::string dynlib::name() const {
        std::string full = location_path();
        if (full.empty())
            return "";
        auto idx = full.find_last_of("\\/");
        return idx != std::string::npos ? full.substr(idx + 1) : full;
    }

    /// dynlib::contains_symbol
    bool dynlib::contains_symbol(std::string_view name) const noexcept {
        return __dynlib::get_symbol_address_impl(m_lib_handle_, name.data()) != nullptr;
    }

    /// dynlib::unchecked_extract_symbol
    dynlib::symbol_type dynlib::unchecked_extract_symbol(std::string_view name) const noexcept {
        const char* raw_name = name.data();
        dynlib::address_type addr = __dynlib::get_symbol_address_impl(m_lib_handle_, raw_name);
        __dynlib::item_kind_impl kind = __dynlib::get_symbol_item_kind_impl(this, addr);
        return symbol_type{symbol_type::SpecialCtor{}, raw_name, addr, 0, __dynlib::get_lang_kind(raw_name), kind};
    }

    /// dynlib::try_extract_symbol
    std::expected<typename dynlib::symbol_type, dynlib_error_code> dynlib::try_extract_symbol(std::string_view name) const noexcept {
        const char* raw_name = name.data();
        dynlib::address_type addr = __dynlib::get_symbol_address_impl(m_lib_handle_, raw_name);
        if (!addr)
            return std::unexpected(dynlib_error_code::symbol_not_found);
        __dynlib::item_kind_impl kind = __dynlib::get_symbol_item_kind_impl(this, addr);
        return symbol_type{symbol_type::SpecialCtor{}, raw_name, addr, 0, __dynlib::get_lang_kind(raw_name), kind};
    }

    /// dynlib::extract_symbol
    dynlib::symbol_type dynlib::extract_symbol(std::string_view name) const {
        auto result = try_extract_symbol(name);
        if (result.has_value())
            return *result;
        throw dynlib_error(result.error());
    }

    //// dynlib::symbols_view [Implementations]
    /// dynlib::symbols_view::symbols_view
    dynlib::symbols_view::symbols_view(const dynlib* parent)
    : m_parent_(parent) {
        if (m_parent_ && m_parent_->native_handle()) {
            auto nt = __dynlib::get_pe_header(m_parent_->native_handle());
            if (!nt)
                return;
            auto export_rva = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
            if (export_rva == 0)
                return;
            char* base = static_cast<char*>(m_parent_->native_handle());
            auto* dir  = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(base + export_rva);
            m_size_ = dir->NumberOfNames;
        }
    }

    /// dynlib::symbols_view::iterator::iterator
    dynlib::symbols_view::iterator::iterator(const dynlib* parent, std::size_t index, std::size_t size)
    : m_parent_(parent), m_index_(index), m_size_(size) {
        if (m_parent_ && m_size_ > 0) {
            auto nt = __dynlib::get_pe_header(m_parent_->native_handle());
            auto export_rva = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
            m_base_addr_ = static_cast<char*>(m_parent_->native_handle());
            auto* dir = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(m_base_addr_ + export_rva);

            m_name_rvas_ = reinterpret_cast<const DWORD*>(m_base_addr_ + dir->AddressOfNames);
            m_ordinals_  = reinterpret_cast<const WORD*>(m_base_addr_ + dir->AddressOfNameOrdinals);
            m_function_rvas_ = reinterpret_cast<const DWORD*>(m_base_addr_ + dir->AddressOfFunctions);
        }
    }

    /// dynlib::symbols_view::iterator::operator*
    dynlib::symbol_type dynlib::symbols_view::iterator::operator*() const {
        if (!m_base_addr_ || m_index_ >= m_size_)
            return symbol_type{};
        const char* name_ptr = m_base_addr_ + m_name_rvas_[m_index_];
        WORD ord = m_ordinals_[m_index_];
        DWORD rva = m_function_rvas_[ord];
        void* addr = nullptr;
        __dynlib::item_kind_impl kind = __dynlib::item_kind_impl::unknown;
        if (rva != 0) {
            addr = m_base_addr_ + rva;
            kind = __dynlib::get_symbol_item_kind_impl(m_parent_, addr);
        }
        return symbol_type{symbol_type::SpecialCtor{}, name_ptr, addr, static_cast<std::size_t>(ord), __dynlib::get_lang_kind(name_ptr), kind};
    }

    //// dynlib::loaded_libraries_view [Implementations]
    /// dynlib::loaded_libraries_view::iterator::Impl
    struct dynlib::loaded_libraries_view::iterator::Impl {
        HANDLE snapshot = INVALID_HANDLE_VALUE;
        MODULEENTRY32 current_entry {};
        Impl() {
            current_entry.dwSize = sizeof(MODULEENTRY32);
        }

        ~Impl() {
            if (snapshot != INVALID_HANDLE_VALUE)
                ::CloseHandle(snapshot);
        }
    };

    /// dynlib::loaded_libraries_view::iterator::iterator
    dynlib::loaded_libraries_view::iterator::iterator(SpecialCtor) {
        m_impl_ = new Impl;
        m_impl_->snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, 0);
        if (m_impl_->snapshot == INVALID_HANDLE_VALUE) {
            delete m_impl_;
            m_impl_ = nullptr;
            return;
        }
        if (!::Module32First(m_impl_->snapshot, &m_impl_->current_entry)) {
            delete m_impl_;
            m_impl_ = nullptr;
            return;
        }
    }

    /// dynlib::loaded_libraries_view::iterator::~iterator
    dynlib::loaded_libraries_view::iterator::~iterator() {
        if (m_impl_)
            delete m_impl_;
    }

    /// dynlib::loaded_libraries_view::iterator::iterator(iterator&&)
    dynlib::loaded_libraries_view::iterator::iterator(iterator&& other) noexcept
    : m_impl_(other.m_impl_) {
        other.m_impl_ = nullptr;
    }

    /// dynlib::loaded_libraries_view::iterator::operator=(iterator&&)
    auto dynlib::loaded_libraries_view::iterator::operator=(iterator&& other) noexcept -> iterator& {
        if (this != &other) {
            if (m_impl_ != nullptr)
                delete m_impl_;
            m_impl_ = other.m_impl_;
            other.m_impl_ = nullptr;
        }
        return *this;
    }

    /// dynlib::loaded_libraries_view::iterator::operator*
    auto dynlib::loaded_libraries_view::iterator::operator*() const -> reference {
        if (!m_impl_)
            return dynlib{};
        return dynlib::unchecked_attach(m_impl_->current_entry.hModule);
    }

    /// dynlib::loaded_libraries_view::iterator::operator++
    auto dynlib::loaded_libraries_view::iterator::operator++()-> iterator& {
        if (m_impl_) {
            if (!::Module32Next(m_impl_->snapshot, &m_impl_->current_entry)) {
                delete m_impl_;
                m_impl_ = nullptr;
            }
        }
        return *this;
    }

    /// dynlib::loaded_libraries_view::iterator::operator==
    bool dynlib::loaded_libraries_view::iterator::operator==(const iterator& other) const noexcept {
        return !m_impl_ == !other.m_impl_;
    }

    /// dynlib::loaded_libraries [impl]
    dynlib::loaded_libraries_view dynlib::loaded_libraries() noexcept {
        return {};
    }

    /// dynlib::extract_symbols [impl]
    dynlib::symbols_view dynlib::extract_symbols() const noexcept {
        return symbols_view{this};
    }

} // namespace gold
