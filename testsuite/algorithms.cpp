#include <gold/algorithm>

#define __GOLDM_DECL_TAG(Tag) \
    struct Tag##_t { \
        enum class hidden_token : bool {}; \
        explicit constexpr Tag##_t(hidden_token) noexcept {} \
    }; \
    inline constexpr Tag##_t Tag { Tag##_t::hidden_token { true } }

__GOLDM_DECL_TAG(nullopt);

int main() {

}
