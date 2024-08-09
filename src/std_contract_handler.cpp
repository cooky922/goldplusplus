#include <iostream>
#include <experimental/contract>

namespace stdx = std::experimental;

void handle_contract_violation(const stdx::contract_violation& violation) {
    bool level_default_p     = violation.assertion_level() == "default";
    bool role_default_p      = violation.assertion_role() == "default";
    bool cont_mode_default_p = violation.continuation_mode() == stdx::contract_violation_continuation_mode::never_continue;
    const char* modes[] { "off", "on" };
    std::cerr << "contract violation in function " << violation.function_name()
              << " at " << violation.file_name()
              << ':'    << violation.line_number()
              << " : "  << violation.comment();
    const char* delimiter = "\n[";
    if (!level_default_p) {
        std::cerr << delimiter << "level:" << violation.assertion_level();
        delimiter = ", ";
    }
    if (!role_default_p) {
        std::cerr << delimiter << "role:" << violation.assertion_role();
        delimiter = ", ";
    }
    if (!cont_mode_default_p) {
        std::cerr << delimiter << "continue:"
                  << modes[static_cast<int>(violation.continuation_mode()) & 1];
        delimiter = ", ";
    }
    if (delimiter[0] == ',')
        std::cerr << ']';
    std::cerr << std::endl;
}
