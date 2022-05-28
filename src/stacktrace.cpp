#include <gold/stacktrace>
#include <algorithm>
#include <sstream>
#include <gold/demangling>
#include <backtrace.h>
#include <dlfcn.h>

/// requires additional linking '-lbacktrace -ldl'

namespace gold::__stacktrace {

    struct impl {

        /// get_backtrace_state
        static ::backtrace_state* get_backtrace_state() {
            static ::backtrace_state* result;
            result = ::backtrace_create_state(__argv[0], 1, nullptr, nullptr);
            return result;
        }

        /// backtrace_capture
        static void backtrace_capture(gold::stacktrace* trace_ptr,
                                      std::size_t skip,
                                      std::size_t max_depth) {

            trace_ptr->m_entries_.reserve(std::min<int>(max_depth, 64));

            struct data_impl {
                gold::stacktrace* trace_ptr;
                std::size_t max_depth;
            } data { trace_ptr, max_depth };

            auto state = get_backtrace_state();
            auto cb = +[](void* data, std::uintptr_t pc) {
                auto& s = *static_cast<data_impl*>(data);
                gold::stacktrace_entry f;
                f.m_pc_ = pc;
                if (f && s.trace_ptr->size() < s.max_depth) {
                    s.trace_ptr->m_entries_.push_back(f);
                    return 0;
                }
                return 1;
            };

            ::backtrace_simple(state, skip + 1, cb, nullptr, &data); // plus 1 for this skip
        }

        /// backtrace_get_info
        static void backtrace_get_info(std::uintptr_t pc,
                                       std::string* func_name_ptr,
                                       std::string* module_file_name_ptr,
                                       std::ptrdiff_t* offset_ptr) {

            if (pc == std::uintptr_t(-1))
                return;

            auto state = get_backtrace_state();
            auto pc_cb = +[](void* data, std::uintptr_t, const char*, int, const char* func) -> int {
                auto& s = *static_cast<std::string*>(data);
                if (func)
                    s = gold::demangle(func);
                else
                    s = "";
                return 1;
            };
            auto sym_cb = +[](void* data, std::uintptr_t, const char* symname,
                              std::uintptr_t, std::uintptr_t) {
                auto& s = *static_cast<std::string*>(data);
                if (symname)
                    s = gold::demangle(symname);
                else
                    s = "???";
            };

            // if 'func_name_ptr' is not nullptr
            if (func_name_ptr != nullptr) {
                ::backtrace_pcinfo(state, pc, pc_cb, nullptr, func_name_ptr);
                if (func_name_ptr->empty())
                    ::backtrace_syminfo(state, pc, sym_cb, nullptr, func_name_ptr);
            }

            ::Dl_info dl_info;

            if ((module_file_name_ptr != nullptr ||
                offset_ptr != nullptr) && ::dladdr(reinterpret_cast<void*>(pc), &dl_info) != 0) {

                if (offset_ptr != nullptr) {
                    if (dl_info.dli_saddr == nullptr)
                        dl_info.dli_saddr = reinterpret_cast<void*>(pc);
                    *offset_ptr = pc - reinterpret_cast<std::uintptr_t>(dl_info.dli_saddr);
                }

                if (module_file_name_ptr != nullptr)
                    *module_file_name_ptr = dl_info.dli_fname;

            }
        }

    };

} // namespace gold::__stacktrace


namespace gold {

    std::string stacktrace_entry::function_name() const {
        std::string result;
        __stacktrace::impl::backtrace_get_info(this->m_pc_, &result, nullptr, nullptr);
        return result;
    }

    std::string stacktrace_entry::module_file_name() const {
        std::string result;
        __stacktrace::impl::backtrace_get_info(this->m_pc_, nullptr, &result, nullptr);
        return result;
    }

    stacktrace stacktrace::current() noexcept {
        return current(1, size_type(-1));
    }

    stacktrace stacktrace::current(size_type skip) noexcept {
        return current(skip + 1, size_type(-1));
    }

    stacktrace stacktrace::current(size_type skip, size_type max_depth) noexcept {
        // [[precondition: skip <= (size_type(-1) - max_depth)]]
        stacktrace result;
        __stacktrace::impl::backtrace_capture(&result, skip + 1, max_depth);
        return result;
    }

    /// operator<<
    std::ostream& operator<<(std::ostream& os, const stacktrace_entry& entry) {
        std::string func, file;
        std::ptrdiff_t offset;
        __stacktrace::impl::backtrace_get_info(entry.m_pc_, &func, &file, &offset);
        std::ios_base::fmtflags fmtf (os.flags());
        os.width(4);
        os.setf(os.showbase);
        os.setf(os.hex);
        os << reinterpret_cast<void*>(entry.m_pc_)
           << " in <" << func << '+' << offset << "> at "
           << file;
        os.flags(fmtf);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const stacktrace& st) {
        for (stacktrace::size_type i = 0; i < st.size(); ++i) {
            os.width(4);
            os << '#' << i << ' ' << st[i] << '\n';
        }
        return os;
    }

    /// to_string<stacktrace_entry>
    std::string to_string(const stacktrace_entry& entry) {
        std::ostringstream oss;
        oss << entry;
        return std::move(oss).str();
    }

    /// to_string<stacktrace>
    std::string to_string(const stacktrace& st) {
        std::ostringstream oss;
        oss << st;
        return std::move(oss).str();
    }

} // namespace gold
