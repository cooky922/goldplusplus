#include <sstream>
#include <cstdint>
#include <gold/stacktrace>
#include <gold/demangling>
#include <gold/bits/algo/min_max.hpp>
#include <dlfcn.h>

/// requires additional linking '-lstdc++_libbacktrace -ldl'

struct __glibcxx_backtrace_state;

extern "C"
{
__glibcxx_backtrace_state*
__glibcxx_backtrace_create_state(const char*, int,
				 void(*)(void*, const char*, int),
				 void*);

int
__glibcxx_backtrace_simple(__glibcxx_backtrace_state*, int,
			   int (*) (void*, std::uintptr_t),
			   void(*)(void*, const char*, int),
			   void*);

int
__glibcxx_backtrace_pcinfo(__glibcxx_backtrace_state*, std::uintptr_t,
			   int (*)(void*, std::uintptr_t,
				   const char*, int, const char*),
			   void(*)(void*, const char*, int),
			   void*);

int
__glibcxx_backtrace_syminfo(__glibcxx_backtrace_state*, std::uintptr_t addr,
			    void (*) (void*, std::uintptr_t, const char*,
				      std::uintptr_t, std::uintptr_t),
			    void(*)(void*, const char*, int),
			    void*);
}

namespace gold::__stacktrace {

    struct impl {

        /// __stacktrace::impl::error_handler
        static void error_handler(void*, const char*, int) {}

        /// __stacktrace::impl::get_backtrace_state
        static ::__glibcxx_backtrace_state* get_backtrace_state() {
            static ::__glibcxx_backtrace_state* result = nullptr;
            if (!result)
                result = ::__glibcxx_backtrace_create_state(__argv[0], 1, &error_handler, nullptr);
            return result;
        }

        /// __stacktrace::impl::s_backtrace_style_
        inline static constinit gold::stacktrace_style s_backtrace_style_ = gold::stacktrace_style::by_default;

        /// __stacktrace::impl::backtrace_capture
        static void backtrace_capture(gold::stacktrace* trace_ptr,
                                      std::size_t skip,
                                      std::size_t max_depth) {

            trace_ptr->m_entries_.reserve(gold::__algo::min(static_cast<int>(max_depth), 64));

            struct data_impl {
                gold::stacktrace* trace_ptr;
                std::size_t max_depth;
            } data { trace_ptr, max_depth };

            auto state = get_backtrace_state();
            auto cb = +[](void* data, std::uintptr_t pc) {
                auto& s = *static_cast<data_impl*>(data);
                gold::stacktrace_entry f;
                f.m_pc_ = pc;
                if (f && s.trace_ptr->m_entries_.size() < s.max_depth) {
                    s.trace_ptr->m_entries_.push_back(f);
                    return 0;
                }
                return 1;
            };

            ::__glibcxx_backtrace_simple(state, skip + 1, cb, &error_handler, &data); // plus 1 for 1 skip
        }

        /// backtrace_get_info
        static void backtrace_get_info(std::uintptr_t pc,
                                       std::string* func_name_ptr,
                                       std::string* module_file_name_ptr,
                                       std::string* source_file_name_ptr,
                                       int* source_line_ptr,
                                       std::ptrdiff_t* offset_ptr) {

            if (pc == std::uintptr_t(-1))
                return;

            struct trace_info_t {
                std::string* m_func_;
                std::string* m_file_;
                int*         m_line_;
            } trace_info { func_name_ptr, source_file_name_ptr, source_line_ptr };

            auto state = get_backtrace_state();
            auto pc_cb = +[](void* ptr, std::uintptr_t, const char* filename, int lineno, const char* func) -> int {
                auto& info = *static_cast<trace_info_t*>(ptr);
                if (info.m_func_) {
                    if (func)
                        *info.m_func_ = gold::demangle(func);
                    else
                        *info.m_func_ = "";
                }
                if (info.m_file_) {
                    if (filename)
                        *info.m_file_ = filename;
                    else
                        *info.m_file_ = "";
                }
                if (info.m_line_) {
                    *info.m_line_ = lineno;
                }
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

            if (func_name_ptr != nullptr
             || source_file_name_ptr != nullptr
             || source_line_ptr != nullptr) {
                ::__glibcxx_backtrace_pcinfo(state, pc, pc_cb, nullptr, &trace_info);
                if (func_name_ptr && func_name_ptr->empty()) {
                    ::__glibcxx_backtrace_syminfo(state, pc, sym_cb, &error_handler, func_name_ptr);
                }
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
        __stacktrace::impl::backtrace_get_info(this->m_pc_, &result, nullptr, nullptr, nullptr, nullptr);
        return result;
    }

    std::string stacktrace_entry::module_file_name() const {
        std::string result;
        __stacktrace::impl::backtrace_get_info(this->m_pc_, nullptr, &result, nullptr, nullptr, nullptr);
        return result;
    }

    std::string stacktrace_entry::source_file_name() const {
        std::string result;
        __stacktrace::impl::backtrace_get_info(this->m_pc_, nullptr, nullptr, &result, nullptr, nullptr);
        return result;
    }

    std::uint32_t stacktrace_entry::source_line() const {
        int result;
        __stacktrace::impl::backtrace_get_info(this->m_pc_, nullptr, nullptr, nullptr, &result, nullptr);
        return static_cast<std::uint32_t>(result);
    }

    stacktrace_style stacktrace::style() noexcept {
        return __stacktrace::impl::s_backtrace_style_;
    }

    void stacktrace::style(stacktrace_style style) noexcept {
        __stacktrace::impl::s_backtrace_style_ = style;
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
        std::ios_base::fmtflags saved_flags (os.flags());
        os.width(4);
        if (__stacktrace::impl::s_backtrace_style_ == stacktrace_style::by_source) {
            int line;
            __stacktrace::impl::backtrace_get_info(entry.m_pc_, &func, nullptr, &file, &line, nullptr);
                os << func << " at "
                   << file << ':'
                   << line;
        } else {
            std::ptrdiff_t offset;
            __stacktrace::impl::backtrace_get_info(entry.m_pc_, &func, &file, nullptr, nullptr, &offset);
            os.setf(os.showbase);
            os.setf(os.hex);
            os << reinterpret_cast<void*>(entry.m_pc_)
               << " in <" << func << '+' << offset << "> at "
               << file;
        }
        os.flags(saved_flags);
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
