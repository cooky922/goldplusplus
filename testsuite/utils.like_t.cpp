#include <gold/bits/__util/like_t.hpp>

struct A {};

int h();
int& j();
int&& k();

int main() {
    using gold::__util::merge_cvref_t;
    using gold::__util::copy_cvref_t;
    using gold::__util::member_like_t;
    using gold::__util::__impl::cvref_kind_of;

    static_assert(cvref_kind_of<int> == cvref_kind_of<A>);
    static_assert(cvref_kind_of<const int> == cvref_kind_of<const A>);

    static_assert(__is_same(decltype(h()), int));
    static_assert(__is_same(decltype(j()), int&));
    static_assert(__is_same(decltype(k()), int&&));

    static_assert(__is_same(merge_cvref_t<int, A>, int));
    static_assert(__is_same(merge_cvref_t<int, const A>, const int));
    static_assert(__is_same(merge_cvref_t<int, A&>, int&));
    static_assert(__is_same(merge_cvref_t<int, const A&>, const int&));
    static_assert(__is_same(merge_cvref_t<int, A&&>, int&&));
    static_assert(__is_same(merge_cvref_t<int, const A&&>, const int&&));
    static_assert(__is_same(merge_cvref_t<const int, A>, const int));
    static_assert(__is_same(merge_cvref_t<const int, const A>, const int));
    static_assert(__is_same(merge_cvref_t<const int, A&>, const int&));
    static_assert(__is_same(merge_cvref_t<const int, const A&>, const int&));
    static_assert(__is_same(merge_cvref_t<const int, A&&>, const int&&));
    static_assert(__is_same(merge_cvref_t<const int, const A&&>, const int&&));

    static_assert(__is_same(merge_cvref_t<int&, A>, int&));
    static_assert(__is_same(merge_cvref_t<int&, const A>, const int&));
    static_assert(__is_same(merge_cvref_t<int&, A&>, int&));
    static_assert(__is_same(merge_cvref_t<int&, const A&>, const int&));
    static_assert(__is_same(merge_cvref_t<int&, A&&>, int&));
    static_assert(__is_same(merge_cvref_t<int&, const A&&>, const int&));

    static_assert(__is_same(merge_cvref_t<const int&, A>, const int&));
    static_assert(__is_same(merge_cvref_t<const int&, const A>, const int&));
    static_assert(__is_same(merge_cvref_t<const int&, A&>, const int&));
    static_assert(__is_same(merge_cvref_t<const int&, const A&>, const int&));
    static_assert(__is_same(merge_cvref_t<const int&, A&&>, const int&));
    static_assert(__is_same(merge_cvref_t<const int&, const A&&>, const int&));

    static_assert(__is_same(merge_cvref_t<int&&, A>, int&&));
    static_assert(__is_same(merge_cvref_t<int&&, const A>, const int&&));
    static_assert(__is_same(merge_cvref_t<int&&, A&>, int&));
    static_assert(__is_same(merge_cvref_t<int&&, const A&>, const int&));
    static_assert(__is_same(merge_cvref_t<int&&, A&&>, int&&));
    static_assert(__is_same(merge_cvref_t<int&&, const A&&>, const int&&));

    static_assert(__is_same(merge_cvref_t<const int&&, A>, const int&&));
    static_assert(__is_same(merge_cvref_t<const int&&, const A>, const int&&));
    static_assert(__is_same(merge_cvref_t<const int&&, A&>, const int&));
    static_assert(__is_same(merge_cvref_t<const int&&, const A&>, const int&));
    static_assert(__is_same(merge_cvref_t<const int&&, A&&>, const int&&));
    static_assert(__is_same(merge_cvref_t<const int&&, const A&&>, const int&&));

    static_assert(__is_same(copy_cvref_t<int, A>, int));
    static_assert(__is_same(copy_cvref_t<int, const A>, const int));
    static_assert(__is_same(copy_cvref_t<int, A&>, int&));
    static_assert(__is_same(copy_cvref_t<int, const A&>, const int&));
    static_assert(__is_same(copy_cvref_t<int, A&&>, int&&));
    static_assert(__is_same(copy_cvref_t<int, const A&&>, const int&&));

    static_assert(__is_same(copy_cvref_t<const int&, A&&>, int&&));

    static_assert(__is_same(member_like_t<int, A>, int&&));
    static_assert(__is_same(member_like_t<int, const A>, const int&&));
    static_assert(__is_same(member_like_t<int, A&>, int&));
    static_assert(__is_same(member_like_t<int, const A&>, const int&));
    static_assert(__is_same(member_like_t<int, A&&>, int&&));
    static_assert(__is_same(member_like_t<int, const A&&>, const int&&));

    static_assert(__is_same(member_like_t<int&, A&&>, int&));
    static_assert(__is_same(member_like_t<int&, const A&>, int&));
    static_assert(__is_same(member_like_t<const int&, A&&>, const int&));
    static_assert(__is_same(member_like_t<const int&, const A&>, const int&));
}
