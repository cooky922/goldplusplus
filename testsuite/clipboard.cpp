#include <gold/clipboard>
#include <gold/print>

int main() {
    gold::println("Pasted content:");

#if 0
    // this will be automatically set by first gold::print[|ln] call
    if (terminal.output_encoding() != gold::text_encoding::UTF8)
        terminal.output_encoding(gold::text_encoding::UTF8);

    // convert copy pasted to utf8 string
    std::string pasted_str (std::from_range, gold::wclipboard::paste_to() | gold::uc::to_utf8);
#endif

    gold::print("{}", gold::clipboard::paste_to());
}
