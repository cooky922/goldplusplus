// <gold/bits/consoles.hpp> - gold++ library

// Copyright (C) [ 2021 - 2022 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_CONSOLES_HPP
#define __GOLD_BITS_CONSOLES_HPP

#include <gold/basic_types>
#include <gold/bits/enum_ops.hpp>

namespace gold {

    namespace consoles {

        /// unit_t
        using unit_t = gold::short_t;

        /// color_kind
        enum class color_kind : gold::uint16_t {
            black = 0x00u,
            blue = 0x01u,
            green = 0x02u,
            aqua = 0x03u,
            red = 0x04u,
            purple = 0x05u,
            yellow = 0x06u,
            white = 0x07u,
            default_color = white,
            gray = 0x08u,
            light_blue = 0x09u,
            light_green = 0x0Au,
            light_aqua = 0x0Bu,
            light_red = 0x0Cu,
            light_purple = 0x0Du,
            light_yellow = 0x0Eu,
            bright_white = 0x0Fu,
            default_back_color = black,
            back_black = black,
            back_blue = 0x10u,
            back_green = 0x20u,
            back_aqua = 0x30u,
            back_red = 0x40u,
            back_purple = 0x50u,
            back_yellow = 0x60u,
            back_white = 0x70u,
            back_gray = 0x80u,
            back_light_blue = 0x90u,
            back_light_green = 0xA0u,
            back_light_aqua = 0xB0u,
            back_light_red = 0xC0u,
            back_light_purple = 0xD0u,
            back_light_yellow = 0xE0u,
            back_bright_white = 0xF0u
        };

        /// input_mode_kind
        enum class input_mode_kind : gold::uint32_t {
            enable_processed_input = 0x0001u,
            enable_line_input = 0x0002u,
            enable_echo_input = 0x0004u,
            enable_window_input = 0x0008u,
            enable_mouse_input = 0x0010u,
            enable_insert_mode = 0x0020u,
            enable_quick_edit_mode = 0x0040u,
            enable_virtual_terminal_input = 0x0200u
        };

        /// output_mode_kind
        enum class output_mode_kind : gold::uint32_t {
            enable_processed_output = 0x0001u,
            enable_wrap_at_eol_output = 0x0002u,
            enable_virtual_terminal_processing = 0x0004u,
            disable_newline_auto_return = 0x0008u,
            enable_lvb_grid_worldwide = 0x0010u
        };

        /// char_union
        union char_union {
            wchar_t unicode_char;
            char    ascii_char;
        };

        /// char_info_record
        struct char_info_record {
            char_union character;
            color_kind attributes;
        };

        /// coord_record
        struct coord_record { unit_t x, y; };

        /// small_rect_record
        struct small_rect_record {
            unit_t left, top, right, bottom;
        };

        /// dimension_record
        struct dimension_record { unit_t width, height; };

        /// sound_frequency
        struct sound_frequency {
            dword_t value;
        };

        /// mouse_button_state_kind
        enum class mouse_button_state_kind : gold::uint8_t {
            left_pressed = 0x01u,
            right_pressed = 0x02u,
            second_pressed = 0x04u,
            third_pressed = 0x08u,
            fourth_pressed = 0x10u
        };

        /// control_key_state_kind
        enum class control_key_state_kind : gold::uint16_t {
            capslock_on = 0x0080u,
            enhanced_key = 0x0100u,
            left_alt_pressed = 0x0002u,
            left_ctrl_pressed = 0x0008u,
            numlock_on = 0x0020u,
            right_alt_pressed = 0x0001u,
            right_ctrl_pressed = 0x0004u,
            scroll_lock_on = 0x0040u,
            shift_pressed = 0x0010
        };

        /// mouse_event_flag_kind
        enum class mouse_event_flag_kind : gold::uint8_t {
            pressed = 0,
            released = 0,
            double_clicked = 0x02u,
            hwheeled = 0x08u,
            wheeled = 0x04u,
            moved = 0x01u
        };

        /// virtual_key_code_kind
        enum class virtual_key_code_kind : gold::uint8_t {
            left_mouse_button = 0x01u,
            right_mouse_button = 0x02u,
            cancel = 0x03u,
            middle_mouse_button = 0x04u,
            x1_mouse_button = 0x05u,
            x2_mouse_button = 0x06u,
            back_key = 0x08u,
            tab_key = 0x09u,
            clear_key = 0x0cu,
            return_key = 0x0du,
            enter_key = return_key,
            shift_key = 0x10u,
            control_key = 0x11u,
            alt_key = 0x12u,
            pause_key = 0x13u,
            capslock_key = 0x14u,
            ime_kana = 0x15u,
            ime_hangul = 0x15u,
            ime_on = 0x16u,
            ime_junja = 0x17u,
            ime_final = 0x18u,
            ime_hanja = 0x19u,
            ime_kanji = 0x19u,
            ime_off = 0x1au,
            escape_key = 0x1bu,
            ime_convert = 0x1cu,
            ime_nonconvert = 0x1du,
            ime_accept = 0x1eu,
            ime_modechange = 0x1fu,
            spacebar_key = 0x20u,
            page_up_key = 0x21u,
            page_down_key = 0x22u,
            end_key = 0x23u,
            home_key = 0x24u,
            left_arrow_key = 0x25u,
            up_arrow_key = 0x26u,
            right_arrow_key = 0x27u,
            down_arrow_key = 0x28u,
            select_key = 0x29u,
            print_key = 0x2au,
            execute_key = 0x2bu,
            print_screen_key = 0x2cu,
            insert_key = 0x2du,
            delete_key = 0x2eu,
            help_key = 0x2fu,
            number_0_key = 0x30u,
            number_1_key = 0x31u,
            number_2_key = 0x32u,
            number_3_key = 0x33u,
            number_4_key = 0x34u,
            number_5_key = 0x35u,
            number_6_key = 0x36u,
            number_7_key = 0x37u,
            number_8_key = 0x38u,
            number_9_key = 0x39u,
            letter_A_key = 0x41u,
            letter_B_key = 0x42u,
            letter_C_key = 0x43u,
            letter_D_key = 0x44u,
            letter_E_key = 0x45u,
            letter_F_key = 0x46u,
            letter_G_key = 0x47u,
            letter_H_key = 0x48u,
            letter_I_key = 0x49u,
            letter_J_key = 0x4au,
            letter_K_key = 0x4bu,
            letter_L_key = 0x4cu,
            letter_M_key = 0x4du,
            letter_N_key = 0x4eu,
            letter_O_key = 0x4fu,
            letter_P_key = 0x50u,
            letter_Q_key = 0x51u,
            letter_R_key = 0x52u,
            letter_S_key = 0x53u,
            letter_T_key = 0x54u,
            letter_U_key = 0x55u,
            letter_V_key = 0x56u,
            letter_W_key = 0x57u,
            letter_X_key = 0x58u,
            letter_Y_key = 0x59u,
            letter_Z_key = 0x5au,
            left_win_key = 0x5bu,
            right_win_key = 0x5cu,
            apps_key = 0x5du,
            sleep_key = 0x5fu,
            numpad_0_key = 0x60u,
            numpad_1_key = 0x61u,
            numpad_2_key = 0x62u,
            numpad_3_key = 0x63u,
            numpad_4_key = 0x64u,
            numpad_5_key = 0x65u,
            numpad_6_key = 0x66u,
            numpad_7_key = 0x67u,
            numpad_8_key = 0x68u,
            numpad_9_key = 0x69u,
            multiply_key = 0x6au,
            add_key = 0x6bu,
            separator_key = 0x6cu,
            subtract_key = 0x6du,
            decimal_key = 0x6eu,
            divide_key = 0x6fu,
            f1_key = 0x70u,
            f2_key = 0x71u,
            f3_key = 0x72u,
            f4_key = 0x73u,
            f5_key = 0x74u,
            f6_key = 0x75u,
            f7_key = 0x76u,
            f8_key = 0x77u,
            f9_key = 0x78u,
            f10_key = 0x79u,
            f11_key = 0x7au,
            f12_key = 0x7bu,
            f13_key = 0x7cu,
            f14_key = 0x7du,
            f15_key = 0x7eu,
            f16_key = 0x7fu,
            f17_key = 0x80u,
            f18_key = 0x81u,
            f19_key = 0x82u,
            f20_key = 0x83u,
            f21_key = 0x84u,
            f22_key = 0x85u,
            f23_key = 0x86u,
            f24_key = 0x87u,
            numlock_key = 0x90u,
            scroll_lock_key = 0x91u,
            left_shift_key = 0xa0u,
            right_shift_key = 0xa1u,
            left_control_key = 0xa2u,
            right_control_key = 0xa3u,
            left_menu_key = 0xa4u,
            right_menu_key = 0xa5u,
            browser_back_key = 0xa6u,
            browser_forward_key = 0xa7u,
            browser_refresh_key = 0xa8u,
            browser_stop_key = 0xa9u,
            browser_search_key = 0xaau,
            browser_fav_key = 0xabu,
            browser_home_key = 0xacu,
            volume_mute_key = 0xadu,
            volume_down_key = 0xaeu,
            volume_up_key = 0xafu,
            media_next_track_key = 0xb0u,
            media_prev_track_key = 0xb1u,
            media_stop_key = 0xb2u,
            media_play_pause = 0xb3u,
            launch_mail_key = 0xb4u,
            launch_media_select_key = 0xb5u,
            launch_app_1_key = 0xb6u,
            launch_app_2_key = 0xb7u,
            ime_process_key = 0xe5u
        };

        /// mouse_event_record
        struct mouse_event_record {
            consoles::coord_record            mouse_position;
            consoles::mouse_button_state_kind button_state;
            consoles::control_key_state_kind  control_key_state;
            consoles::mouse_event_flag_kind   event_flags;
        };

        /// key_event_record
        struct key_event_record {
            bool                             is_key_down;
            word_t                           repeat_count;
            consoles::virtual_key_code_kind  virtual_key_code;
            word_t                           virtual_scan_code;
            consoles::char_union             character;
            consoles::control_key_state_kind control_key_state;
        };

    } // namespace consoles

    namespace __consoles {

        /// screen_buffer_info_record
        struct screen_buffer_info_record {
            consoles::dimension_record  size;
            consoles::coord_record      cursor_position;
            consoles::color_kind        attributes;
            consoles::small_rect_record window;
            consoles::coord_record      maximum_window_size;
        };

        /// menu_event_record
        struct menu_event_record {
            uint_t command_id;
        };

        /// focus_event_record
        struct focus_event_record {
            bool is_set_focus;
        };

        /// window_buffer_size_record
        struct window_buffer_size_record {
            consoles::coord_record size;
        };

        /// input_kind
        enum class input_kind {
            key_event,
            mouse_event,
            window_buffer_size_event,
            menu_event,
            focus_event
        };

        /// input_record
        struct input_record {
            input_kind event_kind;
            union {
                consoles::key_event_record key_event;
                consoles::mouse_event_record mouse_event;
                window_buffer_size_record window_buffer_size_event;
                menu_event_record menu_event;
                focus_event_record focus_event;
            } event;
        };
    } // namespace __consoles

    namespace enum_ops {
        template <>
        inline constexpr bool enable_enum_ops<consoles::color_kind> = true;

        template <>
        inline constexpr bool enable_enum_ops<consoles::input_mode_kind> = true;

        template <>
        inline constexpr bool enable_enum_ops<consoles::output_mode_kind> = true;

        template <>
        inline constexpr bool enable_enum_ops<consoles::mouse_button_state_kind> = true;

        template <>
        inline constexpr bool enable_enum_ops<consoles::control_key_state_kind> = true;

        template <>
        inline constexpr bool enable_enum_ops<consoles::mouse_event_flag_kind> = true;

        template <>
        inline constexpr bool enable_enum_ops<consoles::virtual_key_code_kind> = true;

    } // namespace enum_ops

    inline namespace literals {
        consteval consoles::sound_frequency operator""_hertz(unsigned long long arg) {
            return consoles::sound_frequency{static_cast<dword_t>(arg)};
        }
    }

} // namespace gold

#endif // __GOLD_BITS_CONSOLES_HPP
