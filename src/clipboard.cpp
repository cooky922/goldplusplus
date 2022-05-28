// gold++ library <gold/src/clipboard.cpp>

#include <gold/clipboard>
#include <windows.h>
#include <cstring>

namespace gold {

    template <gold::char_type CharT>
    inline void basic_clipboard<CharT>::clear() {
        if (!::OpenClipboard(nullptr))
            throw gold::clipboard_error("failed to open clipboard");
        if (!::EmptyClipboard())
            throw gold::clipboard_error("failed to empty clipboard");
        ::CloseClipboard();
    }

    template <gold::char_type CharT>
    inline bool basic_clipboard<CharT>::empty() {
        return ::CountClipboardFormats() == 0;
    }

    template <gold::char_type CharT>
    inline void basic_clipboard<CharT>::copy_from(std::basic_string_view<CharT> sv) {
        if (!::OpenClipboard(nullptr))
            throw gold::clipboard_error("failed to open clipboard");
        if (!::EmptyClipboard())
            throw gold::clipboard_error("failed to empty clipboard");

        std::size_t string_size = (sv.size() + 1) * sizeof(CharT);
        ::HANDLE string_handle  = ::GlobalAlloc(GMEM_MOVEABLE, string_size);

        if (string_handle) {
            std::memcpy(::GlobalLock(string_handle), sv.data(), string_size);
            ::GlobalUnlock(string_handle);
            if constexpr (s_is_char_)
                ::SetClipboardData(CF_TEXT, string_handle);
            else
                ::SetClipboardData(CF_UNICODETEXT, string_handle);
        }

        ::CloseClipboard();
    }

    template <gold::char_type CharT>
    inline std::basic_string<CharT> basic_clipboard<CharT>::paste_to() {
        std::basic_string<CharT> result;

        if (empty()) {
            // return empty string
            if constexpr (s_is_char_)
                return "";
            else
                return L"";
        }

        if (!::OpenClipboard(nullptr))
            throw gold::clipboard_error("failed to open clipboard");

        ::HANDLE clipboard_handle = []{
            if constexpr (s_is_char_)
                return ::GetClipboardData(CF_TEXT);
            else
                return ::GetClipboardData(CF_UNICODETEXT);
        }();

        if (!clipboard_handle) {
            ::CloseClipboard();
            throw gold::clipboard_error("clipboard data is empty");
        }

        result = std::basic_string<CharT>(static_cast<CharT*>(::GlobalLock(clipboard_handle)));
        ::GlobalUnlock(clipboard_handle);

        ::CloseClipboard();

        return result;
    }

    template class basic_clipboard<char>;
    template class basic_clipboard<wchar_t>;

} // namespace gold
