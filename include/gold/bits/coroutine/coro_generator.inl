
namespace gold {

    /// current_val_
    template <typename T>
    inline auto generator<T>::current_val_() -> value_type {
            return *current_iter_;
    }

    /// is_done_
    template <typename T>
    inline bool generator<T>::is_done_() const noexcept {
        return current_iter_ == std::default_sentinel;
    }

    /// is_not_done_
    template <typename T>
    inline bool generator<T>::is_not_done_() const noexcept {
        return current_iter_ != std::default_sentinel;
    }

    /// resume_
    template <typename T>
    inline void generator<T>::resume_() {
        if (!has_started_) has_started_ = true;
        if (is_not_done_()) ++current_iter_;
    }

    /// next_
    template <typename T>
    inline auto generator<T>::next_() -> gold::next_result_t<value_type> {
        if (handle_) {
            if (is_not_done_()) {
                resume_();
                return { .value = current_val_(), .done = is_done_() };
            } else {
                return { .value = std::nullopt, .done = true };
            }
        } else {
            return { .value = std::nullopt, .done = false };
        }
    }

    /// iter_begin_
    template <typename T>
    inline auto generator<T>::iter_begin_() {
        if (handle_ && is_not_done_() && !has_started_)
            resume_();
        return current_iter_;
    }

    //// Promise Type

    /// promise_type::get_return_object
    template <typename T>
    inline generator<T> generator<T>::promise_type::get_return_object() {
        return generator(handle_type::from_promise(*this));
    }

    /// promise_type::initial_suspend
    template <typename T>
    inline std::suspend_always generator<T>::promise_type::initial_suspend() { return {}; }

    /// promise_type::final_suspend
    template <typename T>
    inline std::suspend_always generator<T>::promise_type::final_suspend() noexcept { return {}; }

    /// promise_type::unhandled_exception
    template <typename T>
    [[noreturn]] void generator<T>::promise_type::unhandled_exception() { throw; }

    /// return_void
    template <typename T>
    void generator<T>::promise_type::return_void() noexcept {}

}
