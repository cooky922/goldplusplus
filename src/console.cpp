#include <bit>
#include <gold/utility>
#include <gold/console>
#include <windows.h>

namespace {
    inline static constinit char __gold_win32_console_title_buffer [gold::console_type::max_title_size()] {};
}

namespace gold {

    /// console_type::console_type
    console_type::console_type()
    : m_out_(::GetStdHandle(STD_OUTPUT_HANDLE)),
      m_in_(::GetStdHandle(STD_INPUT_HANDLE)),
      m_err_(::GetStdHandle(STD_ERROR_HANDLE)) {}

    /// console_type::mf_get_screen_buffer_info_
    __consoles::screen_buffer_info_record console_type::mf_get_screen_buffer_info_() {
        ::CONSOLE_SCREEN_BUFFER_INFO csbi;
        ::GetConsoleScreenBufferInfo(m_out_, &csbi);
        return {
            .size = { .width = csbi.dwSize.X, .height = csbi.dwSize.Y },
            .cursor_position = { .x = csbi.dwCursorPosition.X, .y = csbi.dwCursorPosition.Y },
            .attributes = consoles::color_kind(csbi.wAttributes),
            .window = {
                .left = csbi.srWindow.Left,
                .top  = csbi.srWindow.Top,
                .right = csbi.srWindow.Right,
                .bottom = csbi.srWindow.Bottom
            },
            .maximum_window_size = { .x = csbi.dwMaximumWindowSize.X, .y = csbi.dwMaximumWindowSize.Y }
        };
    }

    /// console_type::mf_scan_input_
    __consoles::input_record console_type::mf_scan_input_() {
        dword_t events_recorded;
        ::INPUT_RECORD input_info;
        ::ReadConsoleInput(m_in_, &input_info, 1, &events_recorded);
        switch (input_info.EventType) {
            case MOUSE_EVENT:
                return {
                    .event_kind = __consoles::input_kind::mouse_event,
                    .event = {
                        .mouse_event = {
                            .mouse_position = {
                                .x = input_info.Event.MouseEvent.dwMousePosition.X,
                                .y = input_info.Event.MouseEvent.dwMousePosition.Y
                            },
                            .button_state = consoles::mouse_button_state_kind {
                                static_cast<gold::uint8_t>(input_info.Event.MouseEvent.dwButtonState)
                            },
                            .control_key_state = consoles::control_key_state_kind {
                                static_cast<gold::uint16_t>(input_info.Event.MouseEvent.dwControlKeyState)
                            },
                            .event_flags = consoles::mouse_event_flag_kind {
                                static_cast<gold::uint8_t>(input_info.Event.MouseEvent.dwEventFlags)
                            }
                        }
                    }
                };
            case KEY_EVENT:
                return {
                    .event_kind = __consoles::input_kind::key_event,
                    .event = {
                        .key_event = {
                            .is_key_down = static_cast<bool>(
                                input_info.Event.KeyEvent.bKeyDown
                            ),
                            .repeat_count = static_cast<word_t>(
                                input_info.Event.KeyEvent.wRepeatCount
                            ),
                            .virtual_key_code = consoles::virtual_key_code_kind {
                                static_cast<gold::uint8_t>(
                                    input_info.Event.KeyEvent.wVirtualKeyCode
                                )
                            },
                            .virtual_scan_code = static_cast<gold::word_t>(
                                input_info.Event.KeyEvent.wVirtualScanCode
                            ),
                            .character = std::bit_cast<consoles::char_union>(
                                input_info.Event.KeyEvent.uChar
                            ),
                            .control_key_state = consoles::control_key_state_kind {
                                static_cast<gold::uint16_t>(input_info.Event.KeyEvent.dwControlKeyState)
                            }
                        }
                    }
                };
            case WINDOW_BUFFER_SIZE_EVENT:
                return {
                    .event_kind = __consoles::input_kind::window_buffer_size_event,
                    .event = {
                        .window_buffer_size_event = {
                            .size = {
                                .x = input_info.Event.WindowBufferSizeEvent.dwSize.X,
                                .y = input_info.Event.WindowBufferSizeEvent.dwSize.Y
                            }
                        }
                    }
                };
            case MENU_EVENT:
                return {
                    .event_kind = __consoles::input_kind::menu_event,
                    .event = {
                        .menu_event = {
                            .command_id = input_info.Event.MenuEvent.dwCommandId
                        }
                    }
                };
            case FOCUS_EVENT:
                return {
                    .event_kind = __consoles::input_kind::focus_event,
                    .event = {
                        .focus_event = {
                            .is_set_focus = static_cast<bool>(
                                input_info.Event.FocusEvent.bSetFocus
                            )
                        }
                    }
                };
            default:;
        }
        gold::unreachable();
    }

    /// console_type::mf_getch_
    char console_type::mf_getch_() {
        using enum consoles::input_mode_kind;
        using namespace gold::enum_ops;

        gold::dword_t cc;
        auto old_mode = this->input_mode();
        this->input_mode(old_mode & ~(enable_line_input | enable_echo_input));
        char result;
        ::ReadConsole(m_in_, &result, 1, &cc, nullptr);
        this->input_mode(old_mode);
        return result;
    }

    /// console_type::mf_sleep_
    void console_type::mf_sleep_(gold::dword_t dur) { ::Sleep(dur); }

    /// console_type::mf_beep_
    void console_type::mf_beep_(gold::dword_t freq, gold::dword_t dur) { ::Beep(freq, dur); }

    /// console_type::input_handle
    console_type::native_handle_type console_type::input_handle() const noexcept {
        return m_in_;
    }

    /// console_type::output_handle
    console_type::native_handle_type console_type::output_handle() const noexcept {
        return m_out_;
    }

    /// console_type::error_handle
    console_type::native_handle_type console_type::error_handle() const noexcept {
        return m_err_;
    }

    /// console_type::output_mode
    consoles::output_mode_kind console_type::output_mode() {
        gold::dword_t result;
        ::GetConsoleMode(m_out_, &result);
        return consoles::output_mode_kind(result);
    }

    console_type& console_type::output_mode(consoles::output_mode_kind arg) {
        ::SetConsoleMode(m_out_, std::to_underlying(arg));
        return *this;
    }

    /// console_type::input_mode
    consoles::input_mode_kind console_type::input_mode() {
        gold::dword_t result;
        ::GetConsoleMode(m_in_, &result);
        return consoles::input_mode_kind(result);
    }

    console_type& console_type::input_mode(consoles::input_mode_kind arg) {
        ::SetConsoleMode(m_in_, std::to_underlying(arg));
        return *this;
    }

    /// console_type::dimension
    consoles::dimension_record console_type::dimension() { return mf_get_screen_buffer_info_().size; }

    /// console_type::column_count
    consoles::unit_t console_type::column_count() { return this->dimension().width; }

    /// console_type::row_count
    consoles::unit_t console_type::row_count() { return this->dimension().height; }

    /// console_type::cursor_pos
    consoles::coord_record console_type::cursor_pos() { return mf_get_screen_buffer_info_().cursor_position; }

    console_type& console_type::cursor_pos(consoles::coord_record arg) {
        ::SetConsoleCursorPosition(m_out_, { arg.x, arg.y });
        return *this;
    }

    /// console_type::title
    std::string_view console_type::title() {
        ::GetConsoleTitle(__gold_win32_console_title_buffer, max_title_size());
        return std::string_view(__gold_win32_console_title_buffer, __builtin_strlen(__gold_win32_console_title_buffer));
    }

    console_type& console_type::title(std::string_view sv) {
        // note: undefined behavior if called more than once
        ::SetConsoleTitle(sv.data());
        return *this;
    }

    /// console_type::print_char
    console_type& console_type::print_char(char c) {
        char buffer[2] { c, '\0' };
        ::WriteConsole(m_out_, buffer, 1, nullptr, nullptr);
        return *this;
    }

    /// console_type::print_char_at
    console_type& console_type::print_char_at(consoles::coord_record cr, char c) {
        auto original_cursor = this->cursor_pos();
        this->cursor_pos(cr);
        this->print_char(c);
        return this->cursor_pos(original_cursor);
    }

    /// console_type::unformatted_print
    console_type& console_type::unformatted_print(std::string_view sv) {
        ::WriteConsole(m_out_, sv.data(), sv.size(), nullptr, nullptr);
        return *this;
    }

    /// console_type::scan_char
    char console_type::scan_char() { return ::getchar(); }

    /// console_type::scan_key
    char console_type::scan_key() { return mf_getch_(); }

    /// console_type::scanline
    std::string console_type::scanline() {
        std::string result;
        char ch;
        while ((ch = this->scan_char()) && ch != '\n')
            result.push_back(ch);
        return result;
    }

    /// console_type::scan_mouse_input
    consoles::mouse_event_record console_type::scan_mouse_input() {
        auto input_result = mf_scan_input_();
        while (true) {
            switch (input_result.event_kind) {
                using enum __consoles::input_kind;
                case mouse_event:
                    return input_result.event.mouse_event;
                default:;
            }
            input_result = mf_scan_input_();
        }
        gold::unreachable();
    }

    /// console_type::scan_key_input
    consoles::key_event_record console_type::scan_key_input() {
        auto input_result = mf_scan_input_();
        while (true) {
            switch (input_result.event_kind) {
                using enum __consoles::input_kind;
                case key_event:
                    return input_result.event.key_event;
                default:;
            }
            input_result = mf_scan_input_();
        }
        gold::unreachable();
    }

    /// console_type::clear
    console_type& console_type::clear() {
        gold::dword_t n, size = this->column_count() * this->row_count();
        ::FillConsoleOutputCharacter(m_out_, ' ', size, {0, 0}, &n);
        ::FillConsoleOutputAttribute(
            m_out_,
            std::to_underlying(mf_get_screen_buffer_info_().attributes),
            size,
            {0, 0},
            &n
        );
        this->cursor_pos({0, 0});
        return *this;
    }

    /// console_type::color
    consoles::color_kind console_type::color() {
        return mf_get_screen_buffer_info_().attributes;
    }

    console_type& console_type::color(consoles::color_kind color_attr) {
        ::SetConsoleTextAttribute(m_out_, std::to_underlying(color_attr));
        return *this;
    }

    /// console_type::color_screen
    console_type& console_type::color_screen(consoles::color_kind color_attr) {
        gold::dword_t n, size = this->column_count() * this->row_count();
        ::FillConsoleOutputAttribute(
            m_out_,
            std::to_underlying(color_attr),
            size,
            {0, 0},
            &n
        );
        return this->color(color_attr);
    }

    /// console_type::pause
    console_type& console_type::pause() {
        (void) mf_getch_();
        return *this;
    }

    namespace __consoles {

        console_type& current_console() {
            static console_type instance;
            return instance;
        }

    }

    /// console
    inline console_type& console { __consoles::current_console() };

} // namespace gold
