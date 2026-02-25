// gold++ library <gold/src/fiber_context.cpp>

#include <gold/fiber_context>
#include <windows.h>
#include <fibersapi.h>

namespace gold {

    namespace __fibers {

        /// __fibers::transfer_data
        struct transfer_data {
            void* prev_handle = nullptr;
            fiber_data* prev_data = nullptr;

            fiber_context (*hook_fn)(void*, fiber_context&&) = nullptr;
            void* hook_payload = nullptr;
            bool is_terminating = false;
        };

        /// __fibers::pending_transfer_ptr
        constinit static thread_local transfer_data* pending_transfer_ptr = nullptr;

        /// __fibers::is_current_thread_a_fiber
        static bool is_current_thread_a_fiber() noexcept {
            return reinterpret_cast<__UINTPTR_TYPE__>(GetCurrentFiber()) != 0x1e00;
        }

        /// __fibers::accept_transfer_impl
        fiber_context accept_transfer_impl() {
            // pre: 'data != nullptr'
            auto tdata = __fibers::pending_transfer_ptr;

            fiber_context prev;
            prev.m_handle = tdata->prev_handle;
            prev.m_data   = tdata->prev_data;

            if (tdata->is_terminating)
                if (prev.m_data != nullptr)
                    prev.m_data->done_p = true;

            auto hook = tdata->hook_fn;
            auto payload = tdata->hook_payload;
            __fibers::pending_transfer_ptr = nullptr;

            if (hook) {
                fiber_context next = hook(payload, std::move(prev));
                if (next)
                    return std::move(next).resume();
            }

            return prev;
        }

        /// __fibers::switch_context_impl
        fiber_context switch_context_impl(void* target, __fibers::transfer_data& tdata) {
            tdata.prev_handle = GetCurrentFiber();
            void* raw_data = GetFiberData();
            tdata.prev_data = raw_data ? static_cast<__fibers::fiber_data*>(raw_data) : nullptr;
            __fibers::pending_transfer_ptr = &tdata;
            SwitchToFiber(target);
            return accept_transfer_impl();
        }

        /// __fibers::fiber_proc_impl
        [[noreturn]] void __stdcall fiber_proc_impl(void* pdata) {
            auto data = static_cast<__fibers::fiber_data*>(pdata);

            // synthesize the context of the fiber that switched to us
            fiber_context prev_fiber = accept_transfer_impl();

            // run the user function. It returns the 'next' fiber to run.
            fiber_context next_fiber;
            try {
                next_fiber = data->entry->run(std::move(prev_fiber));
            } catch (...) {
                std::terminate();
            }

            // 3. The fiber has finished its task. We must resume the returned fiber.
            // NOTE: Once we switch away here, we should never return to this line
            // because this fiber is logically "dead" (function returned).
            if (next_fiber) {
                void* target_handle = next_fiber.m_handle;
                next_fiber.m_handle = nullptr;
                next_fiber.m_data   = nullptr;

                __fibers::transfer_data tdata;
                tdata.hook_fn = nullptr;
                tdata.is_terminating = true;

                tdata.prev_handle = GetCurrentFiber();
                tdata.prev_data = static_cast<__fibers::fiber_data*>(GetFiberData());
                // data.prev_main_p = false;
                __fibers::pending_transfer_ptr = &tdata;
                SwitchToFiber(target_handle);
            }

            // If resume returns (someone switched back to a finished fiber), strictly terminate.
            std::terminate();
        }

    } // namespace __fibers

    fiber_scope::fiber_scope() {
        if (!__fibers::is_current_thread_a_fiber()) {
            m_main_data.main_p = true;
            ConvertThreadToFiberEx(&m_main_data, 0);
            m_did_convert = true;
        }
    }

    fiber_scope::~fiber_scope() {
        if (m_did_convert)
            ConvertFiberToThread();
    }

    bool fiber_context::sf_is_current_thread_a_fiber_impl_() {
        return __fibers::is_current_thread_a_fiber();
    }

    void fiber_context::mf_create_impl_(__fibers::fiber_entry* entry_ptr) {
        m_data = new __fibers::fiber_data();
        m_data->entry = entry_ptr;
        m_data->main_p = false;
        m_handle = CreateFiber(0, &__fibers::fiber_proc_impl, m_data);
        if (!m_handle) {
            delete m_data;
            throw fiber_error("'gold::fiber_context' internal creation failed");
        }
    }

    void fiber_context::mf_maybe_clear_impl_() {
        if (m_handle != nullptr && m_data != nullptr && !m_data->main_p) {
            DeleteFiber(m_handle);
            delete m_data;
        }
    }

    fiber_context fiber_context::mf_resume_impl_(fiber_context (* hook_fn) (void*, fiber_context&&), void* hook_payload) && {
        if (!m_handle)
            return {};

        if (m_data != nullptr && m_data->done_p)
            throw fiber_error("user attempting to resume a finished 'gold::fiber_context'");

        void* target_handle = std::exchange(m_handle, nullptr);
        std::exchange(m_data, nullptr);

        __fibers::transfer_data tdata;
        tdata.hook_fn = hook_fn;
        tdata.hook_payload = hook_payload;

        return __fibers::switch_context_impl(target_handle, tdata);
    }

} // namespace gold
