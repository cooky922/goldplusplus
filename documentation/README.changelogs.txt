gold++ changelogs
===============================

LIST OF NEW HEADERS ON gold++
==================================
    <gold/expected>        | 2022.01.02 [gold++.v2]
    <gold/basic_types>     | 2022.01.24 [gold++.v2]
    <gold/console>         | 2022.01.24 [gold++.v2]
    <gold/dynamic_library> | 2022.01.28 [gold++.v2]
    <gold/demangling>      | 2022.01.29 [gold++.v2]
    <gold/char_class>      | 2022.02.19 [gold++.v2]
    <gold/ratio>           | 2022.02.22 [gold++.v2]
    <gold/coroutine>       | 2022.02.23 [gold++.v2]
    <gold/stacktrace>      | 2022.03.02 [gold++.v2]
    <gold/ctype_info>      | 2022.03.18 [gold++.v2]
    <gold/clipboard>       | 2022.04.07 [gold++.v2]
    <gold/memory>          | 2022.05.06 [gold++.v2]
    <gold/functions>       | 2022.05.24 [gold++.v2]
    <gold/tuples>          | 2022.06.09 [gold++.v2]
    <gold/type_sequence>   | 2022.06.27 [gold++.v2]
    <gold/algorithm>       | 2022.06.30 [gold++.v2]
    <gold/ranges>          | 2022.06.30 [gold++.v2]
    <gold/lazy>            | 2022.07.06 [gold++.v2]
    <gold/struct_string>   | 2022.08.14 [gold++.v2]
    <gold/sync_value>      | 2023.06.30 [gold++.v3]
    <gold/mdspan>          | 2023.07.25 [gold++.v3]
    <gold/print>           | 2023.07.26 [gold++.v3]
    <gold/mdarray>         | 2023.07.27 [gold++.v3]
    <gold/tagged_tuple>    | 2024.02.11 [gold++.v4]
    <gold/math>            | 2024.06.23 [gold++.v4]
    <gold/numeric>         | 2024.06.23 [gold++.v4]
    <gold/inplace_vector>  | 2024.07.23 [gold++.v4]
    <gold/stopwatch>       | 2024.07.23 [gold++.v4]
    <gold/text_encoding>   | 2025.05.26 [gold++.v4]
    <gold/ascii>           | 2025.05.28 [gold++.v4]
    <gold/unicode>         | 2025.05.28 [gold++.v4]
    <gold/fiber_context>   | 2025.12.29 [gold++.v4]

SUMMARY
==========================
[gold++.v4] [SOON]

+ added <gold/tagged_tuple>
+ added <gold/inplace_vector>
+ added <gold/stopwatch>
+ added <gold/math>
+ added <gold/numeric>
+ added <gold/text_encoding>
+ added <gold/ascii>
+ added <gold/unicode>
+ added <gold/fiber_context>
+ revamped <gold/dynamic_library>
+ deprecated <gold/char_class>
+ removed <gold/math_int>
+ removed <gold/numbers>

+ new <gold/math>
    + gold::math::closed_interval [added]
    + gold::math::[left_|right_|]open_interval [added]
    + gold::math::unit_interval [added]
    + gold::math::transmute_value [added]
    + gold::math::normalize_value [added]
    + moved items from <gold/math_int>
    + moved numeric constants from <gold/numbers>

+ new <gold/numeric>
    + moved saturation arithmetic from <gold/numbers>

+ new <gold/tagged_tuple>
    + gold::tagged_member
    + gold::tagged_arg
    + gold::tagged_tuple

+ new <gold/inplace_vector>
    + gold::inplace_vector

+ new <gold/stopwatch>
    + gold::basic_stopwatch
    + gold::stopwatch

+ new <gold/text_encoding>
    + gold::text_encoding

+ new <gold/ascii>
    + ASCII character classification

+ new <gold/unicode>
    + Unicode UTF-8, UTF-16, and UTF-32 Transcoding API

+ <gold/ranges> changes
    + gold::views::cycle_n [added]
    + gold::views::delimit [added]
    + gold::views::cache_last [added]
    + gold::ranges::to [revamped]
    + gold::ranges::infinite_range [added]
    + gold::ranges::nested_range [added]
    + gold::ranges::statically_sized_range [added]
    + gold::ranges::const_iterable_range [added]

+ <gold/format> changes
    + added 'format_as' ADL mechanism

+ <gold/utility> changes
    - gold::forward_like [removed]
      * use std::forward_like instead
    ~ gold::non_type[_t] -> gold::constant_arg[_t] [renamed]

+ <gold/type_traits> changes
    - gold::decay[_if][_t] [removed]
    + gold::constant[_t] [added]
    + gold::base_at[_t] [added]
    + gold::direct_base_at[_t] [added]
    + gold::base_count[_v] [added]
    + gold::direct_base_count[_v] [added]
    + gold::is_scalar_numeric_type[_v] [added]
    ~ gold::is_char[_v] -> gold::is_character_type[_v] [renamed]
    ~ gold::is_structural[_v] -> gold::is_structural_type[_v] [renamed]

+ <gold/concepts> changes
    ~ gold::lambda -> gold::closure_type [renamed]
    ~ gold::qualified_numeric -> gold::scalar_numeric_type [renamed]

+ <gold/functional> changes
    ~ gold::unref -> gold::unwrap_ref [renamed]
    + gold::convert_as [added]
    + gold::construct_object [added]

+ <gold/functions> changes
    ~ gold::function_view -> gold::function_ref [renamed]

+ <gold/tuples> changes
    + gold::tuples::get_element [added]
    + gold::tuples::get_key [added]
    + gold::tuples::get_value [added]
    + gold::tuples::visit_at [added]
    + gold::tuples::visitor_for [added]
    + gold::tuples::visitable_with [added]

+ <gold/mdspan> changes
    + gold::strided_slice [added]
    + gold::ranged_slice [added]
    + gold::submdspan_mapping_result [added]
    + gold::submdspan_extents [added]
    + gold::submdspan [added]
    + gold::equal_mdspan [added]

+ <gold/bit> changes
    + gold::bit_reverse [added]
    + gold::bit_repeat [added]
    + gold::next_bit_permutation [added]
    + gold::prev_bit_permutation [added]
    + gold::bit_compress[l|r] [added]
    + gold::bit_expand[l|r] [added]

+ <gold/memory> changes
    + gold::uninitialized [added]

+ <gold/iterator> changes
    + gold::iterator_interface [added]
    - gold::dummy_counting_iterator [removed]
    - gold::enumerated_iterator [removed]

+ <gold/vocabulary> changes
    - gold::optional_ref [removed]
    - gold::make_optional_ref [removed]
    - gold::make_optional_cref [removed]

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[gold++.v3] [August 20, 2023]
+ [general]
  + added 'static' specifiers in CPO's 'operator()'
    if it is already stateless
  + rebase onto C++26

+ added <gold/mdarray>
+ added <gold/mdspan>
+ added <gold/print>
+ added <gold/sync_value>
+ removed <gold/array>

+ <gold/algorithm>
  + added 'gold::ranges::is_uniqued'
  + added forward iterators and ranges support
    for 'gold::ranges::find_last[|_if|_if_not]
  + fixed 'gold::ranges::fold_right'
    and 'gold::ranges::fold_left_with_iter'
    from potential const_cast or reinterpret_cast

+ <gold/any>
  + added 'gold::in_place_viewable[_t]' and
    support pointers upon construction
    on the fly on 'gold::view_any'

+ <gold/basic_types>
  + added 'gold::restricted' type alias
  + added 'gold::vectorized' type alias
  + added extended floating-point type aliases
  + removed generalized floating-point type
    aliases
  + removed '#include <cstdint>'

+ <gold/concepts>
  + added 'gold::stateless_allocator'
  + added 'gold::basic_string_like',
    'gold::string_like' and 'gold::wstring_like'
  + added 'gold::disable_string_like'
  + removed 'gold::enable_qualified_string',
    'gold::qualified_basic_string',
    'gold::qualified_string', and
    'gold::qualified_wstring'

+ <gold/expected>
  + added new member functions 'has_error',
    'error_or_else', and 'value_or_else'
    for 'gold::expected'
  + changes constraints of monadic operations
    of gold::expected as per LWG 3877
  + fixed gold::expected::transform_error for void specialization
    by removing one faulty constraint if void is either move or
    copy constructible which is obviously neither of them

+ <gold/format>
  + revamped all text formatting designs
    rebasing onto [std]
    + improved floating-point formatting at compile-time
    + compile-time format string check
  + provides gold::expected-API for noexcept guarantees
  + added formatter support for ranges and tuples
  + added new escaped presentation of characters and string

+ <gold/functional>
  + added deleted overload for 'gold::make_overload'
    with no arguments
  + added NTTP overload for 'gold::bind_front',
    'gold::bind_back', and 'gold::compose'
  + revamped 'gold::try_invoke' by adding
    "exception" handling in consteval context

+ <gold/functions>
  + added restrictions to 'gold::function_view'
    constructors to prevent dangling references or
    pointers

+ <gold/iterator>
  + added 'gold::null_sentinel[_t]'
  + removed types of basic const iterator family

+ <gold/memory>
  + added 'gold::in_ptr[_t]' and 'gold::inout_ptr[_t]'
  + added cloned-pointer support
    | 'gold::cloned_ptr'
    | 'gold::derived[_t]'
    | 'gold::default_cloner'
    | 'gold::default_deleter'
    | 'gold::make_cloned'
    | 'gold::make_cloned_for_overwrite'
    | 'gold::allocate_cloned_ptr'

+ <gold/numbers>
  + added saturation arithmetic support
    | 'gold::sat_add'
    | 'gold::sat_sub'
    | 'gold::sat_mul'
    | 'gold::sat_div'
    | 'gold::saturate_cast'

+ <gold/pack>
  + added 'gold::pack_apply_index'
  + added 'gold::pack_for_each_index'
  + added 'gold::pack_fold_left'
  + added 'gold::pack_fold_right'
  + added attribute '[[gnu::always_inline]]'
    to all pack functions
  + reimplemented 'gold::pack_get'
  + reimplemented 'gold::pack_element_at',
    'gold::pack_at_front', and
    'gold::pack_at_back' in terms of new
    intrinsic '__type_pack_element'

+ <gold/ranges>
  + added 'gold::ranges::concat_view' and 'gold::views::concat'
  + added move-only support for 'gold::ranges::maybe_view'
  + added new case for 'gold::views::const_each'
    when the range denotes 'std::ranges::ref_view' and its
    template argument that is const-ed models
    'std::ranges::constant_range'
  + added new case for 'gold::views::const_each'
    when the range denotes 'std::ranges::empty_view'
  + improved member function on 'gold::ranges::move_each_view'
    for non-common range cases
  + added in-place construction on 'gold::range_adaptor_closure'
  + 'gold::ranges::to' return type should not be
    a reference type
  + reimplemented 'gold::ranges::to' to fix
    nested conversion of ranges and to optimize
    compile-time instantiations
  + removed some of the iterators and ranges that were
    already implemented in libstdc++
    | primitive types aliases
    | 'gold::ranges::repeat_view' and 'gold::views::repeat'
    | 'gold::ranges::enumerate_view' and 'gold::views::enumerate'
    | 'gold::ranges::slide_view' and 'gold::views::slide'
    | 'gold::ranges::chunk_view' and 'gold::views::chunk'
    | 'gold::ranges::chunk_by_view' and 'gold::views::chunk_by'
    | 'gold::ranges::zip_view' and 'gold::views::zip'
    | 'gold::ranges::zip_transform_view' and 'gold::views::zip_transform'
    | 'gold::ranges::elements_view' and 'gold::views::elements'
    | 'gold::ranges::stride_view' and 'gold::views::stride'

+ <gold/ratio>
  + added new constants
    | 'gold::quecto'
    | 'gold::ronto'
    | 'gold::ronna'
    | 'gold::quetta'

+ <gold/stacktrace>
  + added 'gold::stacktrace_style'
    and 'gold::stacktrace::style'
  + fixed potential bug on stack tracing by
    providing default error handler
  + reimplemented in terms of -lstdc++_libbacktrace

+ <gold/tuples>
  + added 'gold::tuples::size'
  + added 'gold::tuples::fold_left[_first]'
  + added 'gold::tuples::fold_right[_last]'
  + added 'gold::tuples::zip'
  + added requirement for 'gold::tuples::get'
    to have 'noexcept' invocation
  + fixed 'gold::tuples::to' class private issue

+ <gold/type_traits>
  + added 'gold::is_convertible_without_narrowing[_v]'
  + removed 'gold::is_narrowing_convertible[_v]'

+ <gold/utility>
  + removed 'gold::unmove'
  + removed 'gold::unforward'
  + removed 'gold::dynamic_extent[_t]'
  + removed 'gold::extent[_t]'

+ <gold/value_sequence>
  + added 'gold::make_repeat_[integer|index]_seq[_t]'
  + added concept 'gold::value_sequence'
  + added new functions 'gold::make_array_from_sequence'
    and 'gold::make_sequence_from_array'
  + added member alias 'partial_sum'
    to 'gold::integer_seq_t'
  + added 'operator[]' to
    'gold::integer_seq_t' and
    'gold::floating_point_seq_t'

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[gold++.v2] [August 28, 2022]
+ added <gold/algorithm>
+ added <gold/basic_types>
+ added <gold/console>
+ added <gold/coroutine>
+ added <gold/char_class>
+ added <gold/clipboard>
+ added <gold/ctype_info>
+ added <gold/demangling>
+ added <gold/dynamic_library>
+ added <gold/expected>
+ added <gold/functions>
+ added <gold/lazy>
+ added <gold/memory>
+ added <gold/ranges>
+ added <gold/stacktrace>
+ added <gold/struct_string>
+ added <gold/ratio>
+ added <gold/tuples>
+ added <gold/type_sequence>
+ removed <gold/property>
+ removed <gold/tuple>
+ removed <gold/tuple_ranges>

+ <gold/any>
  + added comparison support
    for 'gold::any' and 'gold::unique_any'
  + added member function 'ctype_info'
    on 'gold::any' and 'gold::unique_any'
  + added member function 'view'
    on 'gold::any' and 'gold::unique_any'
  + added 'gold::view_any'
  + removed 'gold::maybe_any_cast'
  + unified 'gold::any_cast'
    and 'gold::holds_current_type'
    as CPO
  + fixed self-assignment with null-manager

+ <gold/concepts>
  + added 'gold::relocatable'
  + added 'gold::layout_compatible_with'
  + added 'gold::decayable'
  + added 'gold::similar_to'
  + added 'gold::implicitly_convertible_to'
  + added 'gold::explicitly_convertible_to'
  + added 'gold::move_assignable'
  + added 'gold::copy_assignable'
  + added trivial operation concepts
    | 'gold::trivially_destructible'
    | 'gold::trivially_default_initializable'
    | 'gold::trivially_move_constructible'
    | 'gold::trivially_copy_constructible'
    | 'gold::trivially_relocatable'
    | 'gold::trivially_move_assignable'
    | 'gold::trivially_copy_assignable'
    | 'gold::trivially_copyable'
    | 'gold::trivial_type'
  + fixed the definition of
    'gold::brace_constructible_from'
  + renamed 'gold::structural' to
    'gold::structural_type'

+ <gold/functional>
  + added 'gold::consteval_invoke[_r]'
  + improved 'gold::void_like'
    by adding swallow operations
    'operator()' and new constant
    'gold::void_like_v'
  + added 'gold::bind_front'
  + added 'gold::compose'

+ <gold/generator>
  + revamped 'gold::generator'
    to match with [std] implementations
    and interfaces

+ <gold/iterator>
  + renamed 'gold::counting_iterator' to
    'gold::dummy_counting_iterator'
  + renamed 'gold::coop_counting_iterator'
    to 'gold 'gold::enumerated_iterator'
  + added new primitive types / aliases
    | 'gold::basic_const_iterator'
    | 'gold::const_[iterator|sentinel]'
    | 'gold::make_const_[iterator|sentinel]'
    | 'gold::iter_val_t'
    | 'gold::iter_ref_t'
    | 'gold::iter_cref_t'
    | 'gold::iter_rref_t'
    | 'gold::iter_common_ref_t'
    | 'gold::iter_diff_t'
    | 'gold::indirect_value_result_t'
    | 'gold::projected_value'

+ <gold/numbers>
  + added 'gold::numbers::inv_phi[_v]'

+ <gold/random>
  + renamed 'gold::random_number_generator'
    to 'gold::basic_random_generator'
  + added alias 'gold::default_random_generator'
  + added member functions
    'closure_generate',
    'closure_next_int', and
    'closure_next_real'
    to 'gold::basic_random_generator'
  + simplified member function 'choose'
    on 'gold::basic_random_generator'

+ <gold/struct_array>
  + changed string literal 'fs' from 'constexpr'
    to 'consteval'
  + removed concept 'gold::be_struct_string'
  + demystified the member data name of 'gold::struct_array'
    into 'elements'
  + disabled 'std::ranges::enable_view'
    and 'std::ranges::enable_borrowed_range'
    for 'gold::struct_array'
  + removed interoperability with 'std::array'
  + removed member functions 'cdata' and 'at'
  + make all constructors of 'gold::struct_array'
    consteval

+ <gold/type_traits>
  + renamed 'gold::is_any[_v]' to
    'gold::is_any_of[_v]'
  + added 'gold::decay[_t]'
  + added 'gold::size_constant_v'
  + added 'gold::is_template_instantiation[_v]'
  + added base traits
    | 'gold::bases[_t]'
    | 'gold::direct_bases[_t]'
    | 'gold::has_bases[_v]'
    | 'gold::is_direct_base_of[_v]'
    | 'gold::is_indirect_base_of[_v]'
  + removed 'gold::type_sentinel'

+ <gold/utility>
  + added 'gold::forward_like'
  + added 'gold::as_const'
  + added 'gold::as_mutable'
  + added 'gold::assume'
  + added 'gold::enum_ops' and
    'gold::enum_ops::enable_enum_ops'
    in implementing bitwise operations
    for scoped enums
  + added 'gold::non_type[_t]'
  + deprecated gold::unmove and
    gold::unforward
  + many cast functions were
    declared '[[gnu::always_inline]]'

+ <gold/value_sequence>
  + massive renaming and redesigning
    of entities
  + basic necessities are declared
    as member templates inside 'gold::value_seq_t'

ADDITIONS SINCE 2022
==========================
    | JANUARY 2022
    =======================================================
    | from <gold/utility> add 'gold::forward_like' (1/1/2022)
    | added new header <gold/expected> (1/2/2022)
    | from <gold/functional> add 'gold::consteval_invoke[_r]' (1/15/2022)
    | add new header <gold/basic_types> (1/24/2022)
    | from <gold/utility> add 'gold::enum_ops::enable_enum_ops' (1/24/2022)
    | add new header <gold/console> (1/24/2022)
    | new static library 'libgold++.a' (linked by using flag '-lgold++') (1/24/2022)
    | new makefile created (1/24/2022)
    | fixed bugs on <gold/any> (1/25/2022)
        | especially on moving 'any_kind' object in which
          handle is accidentally assign from 'nullptr'
    | added new member functions from 'gold::console_type' (1/28/2022)
        | '.scan_mouse_input'
        | '.scan_key_input'
        | '.color_screen'
        | '.print_at'
        | '.print_char_at'
    | added new header <gold/dynamic_library> (1/28/2022)
    | added new header <gold/demangling> (1/29/2022)
        | remove 'gold::demangle' from <gold/type_name>
    | added new member functions from 'gold::console_type' (1/29/2022)
        | '.input_handle'
        | '.output_handle'
        | '.error_handle'
    | new improvements on <gold/any> (1/30/2022)
        | replaced 'gold::bad_any_cast' with 'gold::bad_any_access'
        | support for 'operator==' and 'operator<=>' for both any-types
    | removed header <gold/property> (1/30/2022)
    | FEBRUARY 2022
    ============================================================
    | improved 'gold::void_like' (2/1/2022)
        | added 'gold::void_like_v'
        | added member function 'operator()'
    | fixed bugs on <gold/any> (2/1/2022)
        | fixed 'gold::__any::basic_any_handler::s_swap_'
          where non-consteval branch invokes 'swap'
          that some of those parameters passed in are not actually
          swappable
        | fixed 'operator==' and 'operator<=>' for void-containing
          any-like objects
    | renamed 'gold::is_any[_v]' to 'gold::is_any_of[_v]' on <gold/type_traits> (2/7/2022)
    | added 'gold::float80_t' on <gold/basic_types> (2/15/2022)
    | added on <gold/basic_types>: (2/16/2022)
        | 'gold::intmax_t'
        | 'gold::uintmax_t'
        | 'gold::floatmax_t'
    | added new header <gold/char_class> (2/19/2022)
    | revised <gold/random> (2/19/2022)
        | added 'engine_result_type' member type
        | make member type 'default_seeder_type' public
        | added 'closure_generate', 'closure_next_int', 'closure_next_real'
          as synonymous to 'generate_l', 'next_int_l', 'next_real_l'
          respectively
    | added new internal header <gold/bits/unreachable.hpp> (2/19/2022)
        | moving 'gold::unreachable' from <gold/utility> to this header
    | added new internal header <gold/bits/to_integer.hpp> (2/19/2022)
    | added new header <gold/ratio> (2/22/2022)
    | <gold/dynamic_library> changes (2/23/2022)
        | optimize implementation
            | renamed 'gold::dynamic_library' to 'gold::dl_module'
            | renamed 'gold::dl_symbol::address_to' to 'gold::dl_symbol::to_address'
            | changed some of 'gold::dl_module' method return types:
                | 'gold::dl_module::name' ('std::string' to 'std::string_view')
                | 'gold::dl_module::module_name' ('std::string' to 'std::string_view')
                | 'gold::dl_module::image_name' ('std::string' to 'std::string_view')
                | 'gold::dl_module::symbol_names' ('std::vector<std::string>' to 'std::span<const std::string>')
                | 'gold::dl_module::symbols' ('std::vector<gold::dl_symbol>' to 'std::span<gold::dl_symbol>')
            | changed the implementation scheme of 'gold::dl_module'
                | changed the member data
            | renamed 'gold::dl_module::symbol_as' to 'gold::dl_module::entity'
            | renamed 'gold::dl_module::symbol_as_if' to 'gold::dl_module::entity_ptr'
            | renamed 'gold::dl_module::invoke_symbol' to 'gold::dl_module::entity_invoke'
            | added 'gold::dl_module::swap' member and friend
            | renamed 'gold::dl_module::has_loaded' to 'gold::dl_module::has_value'
    | added new header <gold/coroutine> (2/23/2022)
    | MARCH 2022
    | ==========================================
    | added new header <gold/stacktrace> (3/2/2022)
    | added 'gold::generic_lambda' and 'gold::stateless_lambda' on <gold/concepts> (3/5/2022)
    | reimplemented 'gold::system_assert' with compile-time friendly error msg on <gold/assertion> (3/5/2022)
    | added on <gold/numbers> (3/5/2022)
        | 'gold::numbers::inv_phi_v'
        | 'gold::numbers::inv_phi'
    | added 'gold::invoke_class_template[_t]' on <gold/type_traits> (3/6/2022)
    | added 'gold::allocator_of' on <gold/concepts> (3/10/2022)
    | added new internal header <gold/bits/memory/ops.hpp> (3/18/2022)
        | 'gold::construct_at'
        | 'gold::relocate_at'
        | 'gold::destroy_at'
    | added new header <gold/ctype_info> (3/18/2022)
        | 'gold::ctype_info'
        | 'gold::ctype_id'
    | added member function 'ctype_info' on 'gold::any' and 'gold::unique_any' <gold/any> (3/18/2022)
        | adjusted implementation by using 'gold::ctype_info' as uniform type-info
    | added static member function 'unique_id' on 'gold::ctype_info' <gold/ctype_info> (3/21/2022)
    | APRIL 2022
    | ==================================
    | added new header <gold/clipboard> (4/7/2022)
    | tweaks on <gold/char_class> (4/8/2022)
        | renamed original class template gold::char_class to gold::basic_char_class
        | added type alias gold::char_class
        | added type alias gold::wchar_class
    | tweaks on <gold/utility> (4/10/2022)
        | deprecated gold::unmove
        | deprecated gold::unforward
        | added gold::as_mutable
        | declared [[gnu::always_inline]]
            | gold::to_signed
            | gold::to_unsigned
            | gold::as_consteval
            | gold::decay_copy
            | gold::decay_move
            | gold::forward_like
            | gold::discard_unused
    | major revamp on <gold/any> (4/10/2022)
        | added gold::view_any
        | tweaked implementations
            | both gold::any and gold::unique_any will no longer have old 'type_info' member
            | both gold::any and gold::unique_any will have 'view' member
            | deleted gold::maybe_any_cast variants
        | unifying gold::any_cast as CPO
        | unifying gold::holds_current_type as CPO
    | minor tweaks on <gold/ctype_info> (4/11/2022)
        | changed gold::ctype_info::unique_id
            | from function template to function object
                | to "prevent" from using its
                  template argument directly
    | minor changes on <gold/type_traits> (4/12/2022)
        | added gold::decay[_t]
        | added gold::size_constant_v
    | minor changes on <gold/utility> (4/12/2022)
        | divided cast-section into <gold/bits/casts.hpp>
    | minor tweaks on <gold/any> (4/12/2022)
        | fixed gold::any_cast CPO overloads
        | fixed temporary gold::any in gold::any_cast returning a copy
            | that should supposedly be reference since it refers
              to somebody
        | fixed self-assignment with null-manager
        | gold::view_any supports referencing to arrays
        | explicitly deleting overload for gold::view_any ctor
          taking reference to functions
            | but taking reference to function pointers is allowed!
    | changes on <gold/basic_types> (4/13/2022)
        | removed gold::native_handle_t
            | reason: no use
        | added new types and aliases:
            | gold::qword_t
            | gold::decimal32_t
            | gold::decimal64_t
            | gold::decimal128_t
            | gold::decimalmax_t
            | gold::[b|d]float[16|32|64|128]_t
    | changes on <gold/basic_types> (4/15/2022)
        | added new alias:
            | gold::bfloat80_t
    | minor fix on <gold/console> (4/20/2022)
        | fixed gold::console_type::native_handle_type
            | assign from 'void*' instead of
              undeclared 'gold::native_handle_t'
    | MAY 2022
    | =========================
    | new additions on <gold/concepts> (5/3/2022)
        | 'gold::relocatable' [defined in <gold/bits/concepts/types.hpp>]
        | 'gold::layout_compatible_with' [defined in <gold/bits/concepts/types.hpp>]
    | added new internal header <gold/bits/memory/ops_algo.hpp> (5/3/2022)
        | 'gold::iter_construct_at'
        | 'gold::iter_destroy_at'
        | 'gold::iter_relocate_at'
        | 'gold::destroy[_n]'
        | 'gold::uninit_default_construct[_n]'
        | 'gold::uninit_fill[_n]'
        | 'gold::uninit_copy[_n][_result]'
        | 'gold::uninit_move[_n][_result]'
        | 'gold::uninit_relocate[_n][_result]'
    | added new header <gold/memory> (5/6/2022)
        | consists of bits headers:
            | <gold/bits/memory/voidify.hpp>
                | modify the implementation,
                  deleted overload for 'const T&&'
            | <gold/bits/memory/ops.hpp>
            | <gold/bits/memory/ops_algo.hpp>
    | cleanups (5/7/2022)
        | <gold/pack>
            | internal name renaming
        | <gold/bits/type_traits/detection.hpp>
            | internal name renaming
        | <gold/clipboard>
            | changed gold::clipboard_error
              member name type to 'const char*'
              from 'std::string_view'
        | <gold/bit>
            | tweaked the definition of concept
              'gold::__bit::trivially_copyable'
        | removed <gold/bits/demangling.hpp>
            | moved the content to <gold/demangling>
        | <gold/type_name>
            | moved <gold/bits/typeof/runtime_typeof.hpp>
                 to <gold/bits/type_name/type_name_r.hpp>
            | moved <gold/bits/typeof/compile_typeof.hpp>
                 to <gold/bits/type_name/type_name_c.hpp>
            | internal name renaming
        | <gold/concepts>
            | internal name renaming
                | <gold/bits/concepts/lambda.hpp>
        | <gold/random>
            | renamed 'gold::random_number_generator'
                   to 'gold::basic_random_generator'
            | added 'gold::default_random_generator'
            | replaced 'gold::random' object of type
              with 'gold::default_random_generator'
            | simplify 'gold::random_generator::choose'
                | the return type is 'decltype(auto)'
                    | for sized ranges overload
        | <gold/struct_array>
            | changed 'fs' string literal to 'consteval' from
              'constexpr' modifier
    | new type aliases on <gold/basic_types> (5/7/2022)
        | new gold::intmin_t
        | new gold::uintmin_t
        | new gold::floatmin_t
        | new gold::decimalmin_t
        | new gold::bfloatmin_t
        | new gold::dfloatmin_t
    | added 'gold::non_type[_t]' on <gold/utility> (5/24/2022)
        | on <gold/bits/non_type.hpp> internally
    | added new internal header <gold/bits/discard_unused.hpp> (5/24/2022)
        | gold::discard_unused resides here
    | added new header <gold/functions> (5/24/2022)
        | gold::function_view
    | missing features on gold::function_view from <gold/functions> (5/25/2022)
        | added deduction guide for gold::function_view
        | added gold::function_view constructor overload
          taking gold::non_type_t and the state
    | added 'gold::is_template_instantiation[_v]' on <gold/type_traits> (5/25/2022)
        | on <gold/bits/type_traits/template_instantiation.hpp> internally
    | added 'gold::decayable' concept on <gold/concepts> (5/26/2022)
        | on <gold/bits/concepts/types.hpp> internally
    | fixed 'gold::decay_t' on <gold/type_traits> (5/26/2022)
        | fixed the problem where it rejects the types
          that are neither copy nor move constructible
    | replacing 'gold::decay_t' with 'std::decay_t' on (5/26/2022)
        | <gold/bits/any/any.hpp>
        | <gold/bits/any/unique_any.hpp>
    | JUNE 2022
    | ============================
    | changed 'inline namespace' to 'namespace'
      on 'gold::__cpo_access' on <gold/bits/any/cpo.hpp> (6/1/2022)
    | [project gold.tuples] added new header <gold/tuples> (6/9/2022)
    | [project gold.tuples] deprecated header <gold/tuple> (6/9/2022)
    | added 'gold::as_const' on <gold/utility> (6/16/2022)
    | added 'gold::bind_front' on <gold/functional> (6/19/2022)
    | revamped 'gold::bind_back' to align with 'gold::bind_front'
      binder function object (6/19/2022)
    | [project gold.tuples] added conditional noexcept to 'gold::tuples::apply_each' invocation
      on <gold/functional> (6/19/2022)
    | [project gold.tuples] moved <gold/tuple> and <gold/tuple_ranges>
      to <gold/disparaged/tuple> and <gold/disparaged/tuple_ranges>
      respectively (6/23/2022)
    | minor implementation change on 'gold::relocate_at' and 'gold::iter_relocate_at'
      on <gold/memory> (6/24/2022)
        | the use of RAII in relocated-from pointer
    | added 'gold::relocate' on <gold/memory> (6/24/2022)
    | added 'gold::assume' on <gold/utility> (6/24/2022)
    | updated the interface of 'gold::unreachable'
      on <gold/utility> (6/24/2022)
    | fixed 'gold::iter_relocate_at' implementation
      specifically on <gold/bits/memory/ops_algo.hpp> (6/25/2022)
        | 'T*' to 'decltype(std::to_address(from))' as type of
          member 'ptr' of local class 'guard'
            | because 'T' does not exist
    | fixed typo on 'gold::scope_guard' move constructor on
      <gold/scope_guard> (6/25/2022)
    | using 'gold::assume' on 'gold::function_view::operator()'
      on <gold/functions> (6/25/2022)
    | [project gold.tuples] [internal]
      added internal header <gold/bits/tuples/fwd.hpp>
      for 'gold::tuples::tuple_[size|element]' forward
      declarations (6/27/2022)
    | changed the noexcept specification of 'gold::construct_at'
      on <gold/memory> (6/27/2022)
    | [internal] added 'gold::__algo::min_element' and
                       'gold::__algo::max_element' as
                 expositions only on internal header
      <gold/bits/algo/min_max.hpp> (6/27/2022)
    | removed 'gold::type_sentinel' on <gold/type_traits>
      (6/27/2022)
    | [project gold.sequence] major revamp on <gold/value_sequence> (6/27/2022)
        | massive renaming and redesigning of entities
        | basic necessities are declared as member templates
          inside 'gold::value_seq_t'
    | [project gold.sequence] added new header <gold/type_sequence> (6/27/2022)
    | adjusted the interface of both 'gold::bind_front'
      and 'gold::bind_back' on <gold/functional> (6/30/2022)
        | the 'decltype(auto)' return type clause was
          changed to 'auto'
        | added conditional 'noexcept' spec. to both
          'gold::bind_front' and 'gold::bind_back'
        | added more constraints to both 'gold::bind_front'
          and 'gold::bind_back'
            | that all template parameters must satisfy
              std::move_constructible
    | added 'gold::compose' on <gold/functional> (6/30/2022)
    | adjusted both interface and implementation of 'gold::bind_rest'
      moving to <gold/bits/functional/bind_partial.hpp>
      on <gold/functional> (6/30/2022)
        | in order to align with other partial binders design structure
    | moved 'gold::try_invoke' to its own internal
      header <gold/bits/functinoal/try_invoke.hpp> from
      <gold/bits/functional/invoke.hpp> to possibly reduce
      compilation time on <gold/functional> (6/30/2022)
    | added base traits on <gold/type_traits> (6/30/2022)
        | 'gold::bases[_t]'
        | 'gold::direct_bases[_t]'
        | 'gold::has_bases[_v]'
        | 'gold::is_direct_base_of[_v]'
        | 'gold::is_indirect_base_of[_v]'
    | [project gold.ranges] major revamp on <gold/iterator> (6/30/2022)
        | rename 'gold::counting_iterator' with
                 'gold::dummy_counting_iterator'
        | rename 'gold::coop_counting_iterator' with
                 'gold::enumerated_iterator'
        | added
            | 'gold::basic_const_iterator'
            | 'gold::const_iterator'
            | 'gold::const_sentinel'
            | 'gold::make_const_iterator'
            | 'gold::make_const_sentinel'
            | 'gold::iter_val_t'
            | 'gold::iter_ref_t'
            | 'gold::iter_cref_t'
            | 'gold::iter_rref_t'
            | 'gold::iter_common_ref_t'
            | 'gold::iter_diff_t'
            | 'gold::indirect_value_result_t'
            | 'gold::projected_value'
    | [project gold.ranges] added new header <gold/algorithm> (6/30/2022)
    | [project gold.ranges] added new header <gold/ranges> (6/30/2022)
    | JULY 2022
    | =============================
    | [project gold.tuples] renamed 'gold::tuples::make_from' to
                                   'gold::tuples::to'
      on <gold/tuples> (7/1/2022)
    | [project gold.tuples] added conditional
      noexcept specification of 'gold::tuples::to'
      instead of always noexcept on <gold/tuples> (7/1/2022)
    | [project gold.sequence] added 'fill' member
      template alias of 'gold::value_seq_t' on <gold/value_sequence> (7/1/2022)
    | [project gold.tuples] added 'gold::tuples::concat' on <gold/tuples> (7/1/2022)
    | [internal] added new internal header <gold/bits/coroutine/promise_allocator.hpp> (7/1/2022)
        | gold::__coro::promise_allocator
    | [project gold.ranges] fixed 'gold::ranges::to' on <gold/ranges> (7/4/2022)
        | removing the 'std::invocable<T, __ranges::uneval_range&>'
          requirement from concept 'gold::ranges::range_adaptor_closure_object'
    | [project gold.ranges] fixed 'gold::ranges::zip_view' implementation and interfaces (7/4/2022)
        | previously, even this type cannot even satisfy 'std::ranges::input_range'.
          now fixed
        | when there are 2 views, the associated pack type will now be
          'std::pair' instead of 2-type 'std::tuple'
    | [internal] added new internal header <gold/bits/algo/copy.hpp> (7/4/2022)
        | implementing unconstrained copy algorithm
    | updated the implementation of 'gold::assume'
      on <gold/utility> (7/5/2022)
    | several functions are now declared with '[[gnu::always_inline]] inline'
      on <gold/memory> specifically: (7/5/2022)
        | 'gold::construct_at'
        | 'gold::relocate_at'
        | 'gold::relocate'
        | 'gold::destroy_at'
    | [project gold.coroutines] added new header <gold/lazy> (7/6/2022)
        | 'gold::lazy'
    | [project gold.coroutines] major revamp on header <gold/generator> (7/6/2022)
    | [project gold.coroutines] [internal] removed 'gold::__coro::has_real_pointers'
      as of now on internal header
      <gold/bits/coroutine/promise_allocator.hpp> (7/6/2022)
    | [project gold.ranges] [internal] minor tweaks on internal header
      <gold/bits/ranges/to.hpp> (7/6/2022)
        | range adaptor closures
          will no longer inherit from
          gold::ranges::range_adaptor_closure_base
    | [internal] added both overloads to
                 'gold::__algo::[min|max]_element'
                 for custom comparator (7/9/2022)
        | this is to remove certain redundant definitions
          of min-related algorithms somewhere
    | [internal] added 'gold::__algo::div_ceil' (7/9/2022)
        | this is to replace 'gold::ranges::__ranges::div_ceil'
    | [project gold.ranges] added 'gold::ranges::stride_view'
                              and 'gold::views::stride' on
                            <gold/ranges> (7/9/2022)
    | [internal] removed '<utility>' include
      from internal header <gold/bits/functional/function_view.hpp> (7/18/2022)
        | reduced compilation time from approx. 1 to approx. 0 seconds
    | fixed the 'gold::forward_like' especially
      the return type on <gold/utility> (7/21/2022)
    | [project gold.ranges] fixes (7/21/2022)
        | add 'std::ranges::enable_borrowed_range'
          partial specialization for 'gold::ranges::stride_view'
        | removed default ctor of 'gold::ranges::stride_view'
        | make 'gold::ranges::repeat_view::end()' overload
          returning 'std::default_sentinel_t' noexcept
        | remove unnecessary comparison overloads in
          'gold::ranges::zip_[transform_]view::iterator' and
          remove 'three_way_comparable' atomic constraint
          on its 'operator<=>' per LWG 3692 and LWG 3702
          respectively
        | let the return type of 'gold::ranges::chunk_view::outer_iterator::value_type::size()'
          be unsigned per LWG 3707
        | make 'gold::ranges::chunk_view::end' for std::ranges::input_range only
          const-qualified per LWG 3710
        | remove the default ctor of
          'gold::ranges::chunk_view' and 'gold::ranges::slide_view'
          per LWG 3712
    | changes and additions on <gold/concepts> (7/22/2022)
        | fixed the definition of 'gold::brace_constructible_from'
        | renamed concept 'gold::structural' to 'gold::structural_type'
        | added 'gold::similar_to'
        | redefined 'gold::decayable'
        | added 'gold::_[implicitly|explicitly]_convertible_to'
        | added trivial op. concepts
            | gold::trivially_destructible
            | gold::trivially_default_initializable
            | gold::trivially_move_constructible
            | gold::trivially_copy_constructible
            | gold::trivially_relocatable
            | gold::move_assignable
            | gold::copy_assignable
            | gold::trivially_move_assignable
            | gold::trivially_copy_assignable
            | gold::trivially_copyable
            | gold::trivial_type
    | added new member functions '.source_file_name'
      and '.source_line' from 'gold::stacktrace_entry'
      on <gold/stacktrace> (7/22/2022)
    | [project gold.ranges] changes (7/24/2022)
        | simplify 'gold::make_const_iterator()'
               and 'gold::make_const_sentinel()' on <gold/iterator>
        | simplify 'gold::const_iterator'
               and 'gold::const_sentinel'
          by eliminating function template instantiations
          on <gold/iterator>
        | refining 'gold::views::const_each'
          to reduce 'gold::ranges::const_each' specializations on <gold/ranges>
                example:
                    [[expressions]]
                        [1] std::declval<std::array<int, 5>&>() | gold::views::const_each
                        [2] std::declval<std::array<int, 5>&&>() | gold::views::const_each
                        [3] std::declval<std::array<const int, 5>&>() | gold::views::const_each
                        [4] std::declval<std::span<int, 5>&>() | gold::views::const_each
                        [5] std::declval<std::span<const int, 5>&>() | gold::views::const_each
                    [[types (before)]]
                        [1] gold::ranges::const_each_view<std::ranges::ref_view<std::array<int, 5>>>
                        [2] gold::ranges::const_each_view<std::ranges::owning_view<std::array<int, 5>>>
                        [3] gold::ranges::const_each_view<std::ranges::ref_view<std::array<const int, 5>>>
                        [4] gold::ranges::const_each_view<std::span<int, 5>>
                        [5] gold::ranges::const_each_view<std::span<const int, 5>>
                    [[types (after)]]
                        [1] std::ranges::ref_view<const std::array<int, 5>>
                        [2] gold::ranges::const_each_view<std::ranges::owning_view<std::array<int, 5>>>
                        [3] std::ranges::ref_view<std::array<const int, 5>>
                        [4] std::span<const int, 5>
                        [5] std::span<const int, 5>
        | refining 'gold::views::move_each'
          to reduce 'gold::ranges::move_each_view' specializations on <gold/ranges>
          like 'gold::views::const_each'
    | [project gold.ranges] reserve size in 'gold::ranges::to'
      is corrected on <gold/ranges> (7/25/2022)
    | [project gold.tuples] added union type support
      for 'gold::tuples::get' in ADL-cases on <gold/tuples> (7/26/2022)
    | [project gold.ranges] fix noexcept spec. on
       friend 'iter_move' and 'iter_swap' on 'gold::ranges::zip_view::iterator'
       on <gold/ranges> (7/28/2022)
    | [project gold.ranges] refined 'gold::ranges::constant_range'
       'std::ranges::range' was replaced with 'std::ranges::input_range',
       since it is a constant range whose elements are not modifiable
       which its iterator can never satisfy 'std::output_iterator',
       but satisfies 'std::input_iterator' on <gold/ranges>
       (7/28/2022)
    | [project gold.ranges] refined 'gold::basic_const_iterator::operator->',
       adding more cases other than having arrow operator of its base type
       on <gold/iterator> (7/28/2022)
    | [project gold.ranges] improved 'gold::ranges::maybe_view'
       and enable borrowed range support for some types
       on <gold/ranges> (7/30/2022)
    | [project gold.ranges] added new algorithms:
        | 'gold::ranges::find_last'
        | 'gold::ranges::find_last_if'
        | 'gold::ranges::find_last_if_not'
      on <gold/algorithm> (7/30/2022)
    | [project gold.ranges] added new range adaptor closure object 'gold::views::elements'
      as a refinement for 'std::views::elements' to unzip zip views
      if any on <gold/ranges> (7/30/2022)
    | [gold++ 2.0] prerelease now out! (7/31/2022)
        | expected to release on August 2022 :>
    | AUGUST 2022
    | =============================
    | added new internal function 'gold::__algo::visit_with_index'
      on internal header <gold/bits/algo/with_index.hpp>
      (8/9/2022)
    | [project gold.ranges] fixed 'gold::ranges::zip_view::iterator::operator[]'
      by returning either a pair or a tuple instead of always tuple
      on <gold/ranges> (8/12/2022)
    | [project gold.ranges] on <gold/ranges> (8/12/2022)
        | removed 'gold::ranges::range_adaptor_base'
        | removed 'gold::ranges::range_adaptor_closure_base'
    | use 'std::ranges::swap' inside the implementation
      of 'gold::scope_guard::swap' instead of 2-step ADL-swap
      on <gold/scope_guard> (8/13/2022)
    | [project gold.tuples] added 'gold::tuples::enable_array_like'
        and 'gold::tuples::array_like'
        and adjust the implementation of 'gold::tuples::get'
        to consider 'gold::tuples::array_like' types
        on <gold/tuples> (8/13/2022)
    | [project gold.tuples] make some of the items associated with cpos on [gold.tuples]
      private on <gold/tuples> (8/14/2022)
    | [project gold.tuples] added noexcept spec. to 'gold::tuples::concat' on
      <gold/tuples> (8/14/2022)
    | [project gold.tuples] move the primary template definition of
      variable template 'gold::tuples::enable_array_like'
      from <gold/bits/tuples/array_like.hpp>
        to <gold/bits/tuples/fwd.hpp>
    | [project gold.tuples] removed 'gold::tuples::__cpo_access::applicable',
      apply the definition on 'gold::tuples::applicable'
      directly instead on <gold/tuples> (8/14/2022)
    | [project gold.tuples] added union type support
      for 'gold::tuples::apply' in ADL-cases on <gold/tuples> (8/14/2022)
    | added exposition-only concept 'gold::__concepts::not_void'
      to avoid redundant definition of the aforementioned
      concept across headers on new internal header <gold/bits/concepts/not_void.hpp>
      (8/14/2022)
    | added exposition-only concept 'gold::__concepts::class_or_union_or_enum'
      on new internal header <gold/bits/concepts/class_or_union_or_enum.hpp>
      (8/14/2022)
    | constrain 'gold::pack_get' with 'I < sizeof...(Args)' on
      <gold/pack> (8/14/2022)
    | major revamp on <gold/struct_array> (8/14/2022)
        | removed concept 'gold::be_struct_string'
        | simplified the interface of 'gold::struct_array'
            | demystify the member data name into '.elements'
            | disable the struct-array support of
              'std::ranges::enable_view' and
              'std::ranges::enable_borrowed_range'
            | enable 'gold::tuples::array_like'
            | removed interop. with 'std::array'
            | removed 'cdata' member function
            | removed 'at' member function
            | make all constructors 'consteval'
        | create a dedicated type for string, 'gold::struct_string'
    | added new header <gold/struct_string> (8/14/2022)
        | 'gold::struct_string'
        | literal ""_fs
    | remove internal header <gold/bits/struct_array.hpp> (8/14/2022)
    | added exposition-only algorithm 'gold::__algo::contains'
      on new internal header <gold/bits/algo/contains.hpp> (8/15/2022)
    | added support for move-only types in 'gold::ranges::__ranges::box'
      on <gold/ranges> (8/16/2022)
    | added size-support for 'gold::ranges::generate_view'
      on <gold/ranges> (8/16/2022)
    | modify 'iterator_category' of 'gold::ranges::enumerate_view::iterator'
      and added 'iterator_concept' member type alias
      on <gold/ranges> (8/22/2022)
    | added 'gold::ranges::cycle_view'
        and 'gold::views::cycle' on <gold/ranges> (8/26/2022)
    | added exposition-only algorithm 'gold::__algo::synth3way'
      on new internal header <gold/bits/algo/synth3way.hpp> (8/28/2022)
    | major revamp on <gold/expected> (8/28/2022)
        | gold::expected will no longer rely
          on std::variant as data member
            | to improve compile-time performance
        | constrain class templates
          gold::expected and gold::unexpected
        | wrap exception thrown into a function call
        | removed three-way or ordered comparison
          on both gold::unexpected and gold::expected
        | improved constraint requirements to
          eliminate corner cases
        | added 'rebind' and 'rebind_error'
          member alias template for gold::expected
        | improved monadic operations
            | both implementations and interfaces
              are rectified
            | 'error_transform' is renamed to
              'transform_error'
        | compile-time inclusion of header duration
          went from 1s to ~0s
    | [gold++.2] release now out! (8/28/2022)
    | [gold++.3] development commenced (8/28/2022)
    | SEPTEMBER 2022
    | =============================
    | .
    | OCTOBER 2022
    | =============================
    | .
    | NOVEMBER 2022
    | =============================
    | [gold++.3] development started (11/11/2022)
    | removed generalized floating point aliases on <gold/basic_types> (11/11/2022)
    | added extended floating-point type aliases on <gold/basic_types> (11/11/2022)
        | gold::bfloat16x_t
        | gold::float16x_t [changed from gold::float16_t]
        | gold::float32x_t
        | gold::float64x_t
        | gold::float128x_t
        | gold::bfloatmin_t [= gold::bfloat16x_t]
        | gold::floatmaxx_t [= gold::float128x_t]
    | some CPOS are added with 'static' specifiers in 'operator()' (11/12/2022)
        | gold::ranges::contains[_subrange]
        | gold::ranges::[starts|ends]_with
        | gold::ranges::lexicographical_compare_three_way
        | gold::ranges::find_last[|_if|_if_not]
        | gold::ranges::iota
        | gold::ranges::fold_left_[|first_]with_iter
        | gold::ranges::fold_left[|_first]
        | gold::ranges::fold_right[|_last]
        | gold::holds_current_type
        | gold::any_cast
        | gold::iter_[construct|destroy|relocate]_at
        | gold::destroy[_n]
        | gold::uninit_[default_construct|fill|copy|move|relocate][|_n]
        | gold::tuples::apply
        | gold::tuples::apply_each
        | gold::tuples::concat
        | gold::tuples::get
        | gold::tuples::to
    | add deleted-overload of gold::make_overload with no arguments on
      <gold/functional> (11/22/2022)
    | added another branch for gold::views::const_each when
      the range denotes std::ref_view and its template argument consted
      models gold::ranges::constant_range on <gold/ranges> (11/22/2022) [LWG 3811]
    | gold::ranges::to template parameter C should not be a reference type
      on <gold/ranges> (11/22/2022) [LWG 3787]
    | fixed gold::ranges::fold_right and gold::ranges::fold_left_with_iter
      from potential const_cast or reinterpret_cast
      on <gold/ranges> (11/22/2022) [LWG 3779]
    | added internal gold::__concepts::different_from concept
      on internal <gold/bits/concepts/types.hpp> (11/22/2022)
    | fix infinite constraint recursion in gold::basic_const_iterator::operator==
      and other comparison operators on <gold/iterator> (11/22/2022) [LWG 3769]
    | fixed bug [BUG 2022.11.12.01] on <gold/type_traits> (11/26/2022)
    | DECEMBER 2022
    | =============================
    | fixed potential bug on <gold/stacktrace>, by providing
      error handler on implementation src/stacktrace.cpp (12/2/2022)
    | add digit separator support for gold::__ext::to_integer
      on <gold/bits/to_integer.hpp> (12/9/2022)
    | added deleted overload of gold::extent_t constructor
      on <gold/bits/extents.hpp> (12/17/2022)
    | fixed implementation on <gold/bits/any/cpo.hpp>
      where static call operator has 'this' (12/17/2022)
    | removed alias 'gold::floatmaxx_t' on <gold/basic_types> (12/21/2022)
    | removed '#include <cstdint>' on <gold/basic_types> (12/21/2022)
    | added 'gold::restrict_t' alias template on <gold/basic_types> (12/21/2022)
    | added 'gold::__algo::min', 'gold::__algo::max',
            'gold::__algo::min_max_result', 'gold::__algo::min_max'
      on internal header <gold/bits/algo/min_max.hpp> (12/22/2022)
    | fixed missing 'gold::as_const' on <gold/bits/ranges/const_each_view.hpp>
      by '#include <gold/bits/casts.hpp>' (12/25/2022)
    | fixed missing 'gold::__coro::promise_allocator' on <gold/bits/coroutine/lazy.hpp>
      by '#include <gold/bits/coroutine/promise_allocator.hpp>' (12/31/2022)
    | JANUARY 2023
    | =============================
    | added another branch for gold::views::const_each when
      the range denotes std::ranges::empty_view on <gold/ranges> (1/12/2023) [LWG 3850]
    | 'gold::ranges::move_each_view::end()' should improve non-common
      case on <gold/ranges> (1/12/2023) [LWG 3829]
    | make 'gold::ranges::__ranges::to_container_fn's operator() static
      on internal <gold/bits/ranges/to.hpp> (1/15/2023)
    | added constraint on 'gold::ranges::__ranges::to_container_fn's operator()
      on internal <gold/bits/ranges/to.hpp> (1/15/2023)
    | added in-place construction in 'gold::ranges::range_adaptor_closure'
      on internal <gold/bits/ranges/range_adaptor.hpp> (1/15/2023)
    | added new constants on <gold/ratio> (1/22/2023)
        | 'gold::quecto'
        | 'gold::ronto'
        | 'gold::ronna'
        | 'gold::quetta'
    | FEBRUARY 2023
    | =============================
    | added 'gold::ranges::range_common_ref_t' on <gold/ranges> (2/18/2023) [LWG 3860]
    | added 'gold::__util::cast_from_vptr' (2/18/2023)
    | added 'gold::__util::is_valid_ptr' (2/18/2023)
    | added 'gold::__util::are_related_ptrs' (2/18/2023)
    | added 'gold::__util::is_valid_consteval' (2/18/2023)
    | revamped 'gold::try_invoke' on <gold/functional> (2/18/2023)
        | added "exception" handling in consteval context
    | revamped 'gold::ranges::find_last[|_if|_if_not]' on <gold/algorithm> (2/18/2023)
        | added forward iterators and ranges support
    | added implementation 'handle_contract_violation' for experimental contracts (2/19/2023)
    | reimplemented <gold/stacktrace> in terms of libstdc++_libbacktrace.a (2/19/2023)
    | added 'gold::stacktrace_style' and 'gold::stacktrace::style' on <gold/stacktrace> (2/19/2023)
    | reimplemented 'gold::view_any' by simplifying data members and using
      'gold::__util::cast_from_vptr' on <gold/any> (2/19/2023)
    | added 'gold::tuples::size' on <gold/tuples> (2/19/2023)
    | added new constant 'gold::numbers::w1_omega[_v]' on <gold/numbers> (2/20/2023)
    | MARCH 2023
    | =============================
    | fixed 'gold::tuples::to' class private issue on <gold/tuples> (3/5/2023)
    | APRIL 2023
    | =============================
    | .
    | MAY 2023
    | =============================
    | .
    | JUNE 2023
    | =============================
    | removed some of already-implemented iterators and ranges (6/22/2023)
        - removed shorthand alias of iterators and ranges
        - removed gold::basic_const_iterator and family
            + replaced by standard std::basic_const_iterator
        - removed
            - repeat_view
            - enumerate_view
            - slide_view
            - chunk_view
            - chunk_by_view
            - zip_view
            - zip_transform_view
            - elements_view
            - stride_view
    | removed 'gold::unmove' and 'gold::unforward' on <gold/bits/casts.hpp> (6/22/2023)
    | reimplemented 'gold::pack_element_at', 'gold::pack_at_front', and
      'gold::pack_at_back' in terms of intrinsic '__type_pack_element'
      on <gold/pack> (6/22/2023)
    | added NTTP overload for 'gold::bind_front', 'gold::bind_back',
      and 'gold::compose'
      on <gold/functional> (6/22/2023)
    | optimized gold::type_name compile-time implementation on
      <gold/type_name> (6/25/2023)
    | changes on <gold/expected> (6/27/2023)
        + changes constraints of monadic operations
          of gold::expected as per LWG 3877
        + removed unneeded friend class of gold::expected
        + fixed gold::expected::transform_error for void specialization
          by removing one faulty constraint if void is either move or
          copy constructible which is obviously neither of them
    | fixed gold::tuples::apply_each with empty tuple argument cases
      on <gold/tuples> (6/27/2023)
    | libgold++.a renamed to libgold++_core.a is now being merged with
      libstdc++_libbacktrace.a, libdl.a, libdbghelp.a, and libimagehlp.a
      to form a new libgold++.a (6/27/2023)
    | optimize implementation of internals gold::ranges::__ranges::boxable and
      gold::ranges::__ranges::box rebasing on standard implementation
      on <gold/bits/ranges/view_util.hpp> (6/28/2023)
    | added internal 'gold::__util::is_ptr_in_range'
      on <gold/bits/__util/is_ptr_in_range.hpp> (6/28/2023)
    | added internal 'gold::__concepts::class_or_union'
      on <gold/bits/concepts/class_or_union_or_enum.hpp> (6/28/2023)
    | removed 'gold::enable_qualified_string' and 'gold::qualified_basic_string', etc.
      but replaced with 'gold::basic_string_like'
      on <gold/concepts> (6/28/2023)
    | added new 'gold::in_place_viewable[_t]' and support pointers upon construction
      on the fly on <gold/any> (6/29/2023)
    | deleted internal header <gold/bits/min.hpp> (6/29/2023)
    | added internal 'gold::__util::uneval' on <gold/bits/__util/uneval.hpp> (6/29/2023)
    | fixed the implementation of internal 'gold::__util::is_ptr_in_range'
      on <gold/bits/__util/is_ptr_in_range.hpp> (6/29/2023)
    | added new internal 'gold::__util::mini_array'
      on <gold/bits/__util/mini_array.hpp> (6/30/2023)
    | added new internal 'gold::__util::empty_t'
      on <gold/bits/__util/empty_t.hpp> replacing other
      redundant types such as 'gold::__struct::empty_type'
      and 'gold::__expected::empty_type' (6/30/2023)
    | added 'gold::stateless_allocator' on <gold/concepts>
      (6/30/2023)
    | added 'gold::pack_apply_index' on <gold/pack> (6/30/2023)
    | reimplemented 'gold::pack_get' on <gold/pack> (6/30/2023)
    | added attribute [[gnu::always_inline]] to all
      pack functions on <gold/pack> (6/30/2023)
    | added 'gold::integer_seq_t::partial_sum'
      on <gold/value_sequence> (6/30/2023)
    | made 'gold::void_like::operator()' static (6/30/2023)
    | added new <gold/sync_value> and 'gold::sync_value' (6/30/2023)
    | JULY 2023
    | =============================
    | added 'gold::{integer, floating_point}_seq_t::operator[]' on <gold/value_sequence> (7/8/2023)
    | added 'gold::views::concat' on <gold/ranges> (7/10/2023)
    | added 'gold::make_repeat_[integer|index]_seq[_t]' on <gold/value_sequence> (7/10/2023)
    | move 'gold::{index, int, char, bool, float}_seq_t' to internal
      <gold/bits/sequence/value_fwd.hpp> (7/11/2023)
    | added 'gold::in_ptr[_t]' and 'gold::inout_ptr[_t]' on <gold/memory> (7/11/2023)
    | made 'gold::{integer, floating_point}_seq_t::operator[]' constexpr from consteval
      on <gold/value_sequence> (7/11/2023)
    | added default constructors for empty 'gold::__util::mini_array'
      on internal <gold/bits/__util/mini_array.hpp> (7/11/2023)
    | added 'gold::__algo::equal[_n]' on internal
      <gold/bits/algo/equal.hpp> (7/13/2023)
    | added 'gold::null_sentinel[_t]' on <gold/iterator> (7/15/2023)
    | added internal 'gold::__num::fp_parts' on <gold/bits/numbers/fp_parts.hpp> (7/25/2023)
    | added 'gold::vectorized' alias to GCC vector builtins
      on <gold/basic_types> (7/25/2023)
    | renamed 'gold::restrict_t' alias into 'gold::restricted'
      on <gold/basic_types> (7/25/2023)
    | added new <gold/mdspan> (7/25/2023)
    | rebooted <gold/format> (7/26/2023)
    | added new <gold/print> (7/26/2023)
    | revamped 'gold::ranges::to' on <gold/ranges> (7/26/2023)
      + fixed nested conversion of ranges
      + optimized compile-time instantiations
        of nested ranges
        | before: ~3m 19s
        | after:  ~16s to 18s
    | added new <gold/mdarray> (7/27/2023)
    | added structured binding support on gold::extents
      on <gold/mdspan> (7/27/2023)
    | added saturated arithmetic on <gold/numbers> (7/27/2023)
        + gold::sat_add
        + gold::sat_sub
        + gold::sat_mul
        + gold::sat_div
        + gold::saturate_cast
    | removed 'gold::is_narrowing_convertible[_v]' on <gold/type_traits> (7/27/2023)
    | added 'gold::is_convertible_without_narrowing[_v]' on <gold/type_traits> (7/27/2023)
    | added 'gold::pack_for_each_index' on <gold/pack> (7/27/2023)
    | AUGUST 2023
    | =============================
    | added 'gold::cloned_ptr' and 'gold::make_cloned' on <gold/memory> (8/1/2023)
    | removed <gold/unique_ptr> since it is no longer need (we have constexpr std::unique_ptr)
      (8/1/2023)
    | added internal 'gold::__util::mini_span'
      on <gold/bits/__util/mini_span.hpp> (8/1/2023)
    | <gold/format> improvements and new additions (8/1/2023)
      + added range and tuple formatting
      + added compile-time check for dynamic spec argument type
      + fixed some parsing issues
    | added 'gold::disable_string_like' on <gold/concepts> (8/2/2023)
    | added member function 'set_default_format' for tuple and range formatters
      to be used with 'gold::to_formatted[_string]'
      on <gold/format> (8/2/2023)
    | renamed 'gold::format_kind' to 'gold::range_format_kind'
      on <gold/format> (8/3/2023)
    | improved and fixed 'gold::make_overload'
      on <gold/functional> (8/3/2023)
      + added gold::non_type support
      + fixed parameter types of synthesizing function object from function pointer
    | removed 'std::FILE*' overload for 'gold::vprint' | 'gold::print[|ln]'
      on <gold/print> (8/6/2023)
      + reason: will use future feature such as 'gold::io::file'
    | added custom separator support as part of the format-specs
      for ranges and tuples on <gold/format> (8/6/2023)
    | removed header <gold/array> (8/7/2023)
    | added new concept 'gold::value_sequence' on <gold/value_sequence> (8/7/2023)
    | added new functions 'gold::make_array_from_sequence' and
      'gold::make_sequence_from_array' on <gold/value_sequence> (8/7/2023)
    | added non-throwing variants in text formatting such as
      gold::try_format with:
      + gold::try_format_result
      + gold::format_error_code
      + gold::formattable no longer requires member functions 'parse' and 'format'
        because it will be replaced with 'try_parse' and 'try_format'
      + 'gold::formatter::parse' and 'gold::formatter::format'
         are deprecated for all formatters
      + 'gold::formatter::try_parse' and 'gold::formatter::try_format'
         are added for all formatters
      | changes applied on <gold/format> (8/11/2023)
    | removed 'gold::dynamic_extent[_t]' and 'gold::extent_t'
      on <gold/utility> (8/12/2023)
    | fixed gold::format_to_n on <gold/format> (8/12/2023)
    | added gold::try_formatter_parse_result and
      gold::try_formatter_write_result on <gold/format> (8/12/2023)
    | added gold::ranges::is_uniqued on <gold/algorithm> (8/12/2023)
    | gold::tuples::get requires parameter to have 'noexcept'
      invocation (8/12/2023)
    | removed unused header <gold/bits/raw_buffer.hpp> (8/12/2023)
    | fixed gold::ranges::concat_view for not being
      an input range by adding default constructor
      for its iterator type on <gold/ranges> (8/13/2023)
    | fixed gold::ranges::concat_view for not being
      a random access range by adding comparisons
    | added member functions for gold::expected on <gold/expected> (8/13/2023)
        | for general
            | has_error()
            | error_or_else
        | for gold::expected<non-void, E>
            | value_or_else
    | added move-only support for gold::ranges::maybe_view
      on <gold/ranges> (8/13/2023)
    | added internal 'gold::__util::declval' equivalence
      to 'std::declval' (8/14/2023)
    | temporarily removed 'gold::ranges::cycle_view'
      on <gold/ranges> (8/14/2023)
    | added 'gold::layout[_left|_right]::strides'
      on <gold/mdspan> (8/16/2023)
    | added 'gold::pack_fold[_left|_right]'
      on <gold/pack> (8/16/2023)
    | added 'gold::tuples::fold_left[_first]'
      and 'gold::tuples::fold_right[_last]'
      on <gold/tuples> (8/16/2023)
    | improved 'gold::cloned_ptr' by adding
      custom deleter and cloner with new helpers
      'gold::derived[_t]', 'gold::default_deleter',
      'gold::default_cloner'
    | added new experimental header <gold/preview/canvas> (8/16/2023)
    | added restrictions to gold::function_view to prevent dangling references or
      pointers on <gold/functions> (8/17/2023)
    | added more escape representation for basic control codes
      on <gold/format> (8/17/2023)
    | removed 'format' and 'parse' member functions
      in formatter specializations leaving only
      'try_format' and 'try_parse' on <gold/format> (8/18/2023)
    | fixed missing uppercase fixed type for floating-point
      formatter on <gold/format> (8/19/2023)
    | optimized 'gold::format_to' taking 'std::back_insert_iterator'
      by adapting it using partial specialization of
      'gold::__format::seq_sink' on <gold/format> (8/19/2023)
    | readded 'gold::ranges::cycle_view' and 'gold::views::cycle'
      on <gold/ranges> (8/19/2023)
    | added 'gold::tuples::zip' on <gold/tuples> (8/20/2023)
    | [gold.tuples] small fixed and optimizations
      on <gold/tuples> (8/20/2023)
    | [gold++.3] release now out! (8/20/2023)
    | [gold++.4] development started (8/20/2023)
    | added forward iterator comparison support
      for gold::null_sentinel_t on
      <gold/iterator> (8/26/2023)
    | NOVEMBER 2023
    | =============================
    | fixed gold::sat_div on <gold/numbers>
      (11/27/2023)
    | DECEMBER 2023
    | =============================
    | improved 'gold::lambda' concept on
      <gold/concepts> (12/17/2023)
    | fixed initializing union member during
      constant evaluation in gold::format_arg
      on <gold/format> (12/19/2023)
    | improved CTAD on gold::extents and gold::mdspan
      with integral-constant-like types
      on <gold/mdspan> (12/20/2023)
    | removed reverse iterator member access 'rbegin, rend'
      on gold::struct_string to make it simpler
      on <gold/struct_string> (12/21/2023)
    | fixed binary rep. of formatting integers
      on <gold/format> (12/21/2023)
    | added members 'rebind_layout' and
      'rebind_accessor' for gold::mdspan
      on <gold/mdspan> (12/25/2023)
    | tweaked the implementation of gold::extents
      on <gold/mdspan> (12/27/2023)
    | fixed immediate-escalating problems on
      custom types on formatters
      on <gold/format> by removing 'if consteval'
      branch containing consteval function 'cast_from_vptr'
      (12/27/2023)
    | added 'gold::views::cycle_n', a bounded version
      of 'gold::views::cycle'
      on <gold/ranges> (12/28/2023)
    | 'gold::ranges::to' fixes and changes
      on <gold/ranges> (12/28/2023)
        + fix conversion from infinite range case
          (it didn't produce an error but it takes
           minutes to compile)
        + added disambiguation overloads
          to defer range conversion
        + added deleted overloads
          for non-verbose diagnostics
        + implemented LWG 4016:
          optimizing container
          appending of elements
    | <gold/format> changes and additions
      (12/28/2023)
        + fixed 'm' range option
          by setting underlying with:
            underlying.set_separator(": ");
            underlying.set_brackets({}, {});

        + added 'format_as' mechanism
          to simplify enabling formatters
          for user-defined types
    | JANUARY 2024
    | ========================================
    | removed 'gold::forward_like' in favor of
      'std::forward_like' on <gold/utility>
      (1/14/2024)
    | renamed 'gold::unref' to 'gold::unwrap_ref'
      on <gold/functional> (1/14/2024)
    | removed 'gold::decay[_if][_t]'
      on <gold/type_traits> (1/14/2024)
    | added 'gold::tuples::get[_element|_key|_value]'
      on <gold/tuples> (1/14/2024)
    | added submdspan facilities
      on <gold/mdspan> (1/14/2024)
    | added 'gold::tuples::visit_at',
      'gold::tuples::visitor_for', and
      'gold::tuples::visitable_with'
      on <gold/tuples> (1/14/2024)
    | renamed 'gold::[...]lambda' family
      to 'gold::[...]closure_type'
      on <gold/concepts> (1/18/2024)
    | fixed partial sum of gold::integer_seq_t (1/28/2024)
      | from:
        + gold::index_seq_t<2, 4, 1, 5>::partial_sum<> == gold::index_seq_t<0, 2, 6, 7>
      | to:
        + gold::index_seq_t<2, 4, 1, 5>::partial_sum<> == gold::index_seq_t<2, 6, 7, 12>
    | added intervals and value transmutation
      on <gold/utility> (1/28/2024)
    | ============================================
    | FEBRUARY 2024
    | added '__tuple_like' branch in gold::tuples::tuple_like
      on <gold/tuples> (2/9/2024)
      + so that types like std::tuple don't have to be checked
        that they are already tuples
    | changes of gold::value_seq_t on <gold/value_sequence>
      (2/9/2024)
      + unpack      -> unpack_t
      + offset      -> offset_t
      + scale       -> scale_t
      + fill        -> fill_t
      + partial_sum -> partial_sum_t
      + concat_with -> concat_with_t
      + added value counterparts
        'offset', 'scale', 'fill',
        'partial_sum'
    | added new base traits on <gold/type_traits>
      (2/11/2024)
        + gold::base_at[_t]
        + gold::direct_base_at[_t]
        + gold::base_count[_v]
        + gold::direct_base_count[_v]
    | added new type and constant 'gold::constant[_t]'
      on <gold/type_traits> (2/11/2024)
    | added repeat operation for 'gold::value_seq_t'
      with 'gold::constant_t' using operator*
      on <gold/value_sequence> (2/11/2024)
    | added new header <gold/tagged_tuple> (2/11/2024)
    | added new function object templates
      'gold::convert_as' and 'gold::construct_object'
      on <gold/functional> (2/17/2024)
    | added new bits functions
      on <gold/bit> (2/17/2024)
        + gold::bit_reverse
        + gold::bit_repeat
        + gold::next_bit_permutation
        + gold::prev_bit_permutation
        + gold::bit_compress[l|r]
        + gold::bit_expand[l|r]
    | added 'gold::uninitialized'
      on <gold/memory> (2/17/2024)
    | added iterator interface
      on <gold/iterator> (2/18/2024)
    | ============================================
    | MARCH 2024
    | partially simplify [gold.format]
      implementation by replacing
      checks with macro __GOLDM_TRY_FORMAT[|V] (3/2/2024)
    | added internal (3/2/2024)
        + __util::merge_cvref_t,
        + __util::copy_cvref_t,
        + __util::member_like_t
    | simplify gold::compose with deducing this
      on <gold/functional> (3/2/2024)
    | optimize gold::pack_get on
      <gold/pack> (3/2/2024)
      + newer implementation is generally
        ~1.8x faster than older one
    | simplify gold::expected with deducing this
      on <gold/expected> (3/3/2024)
    | <gold/mdspan> and <gold/mdarray> changes (3/11/2024)
      + fix index_cartesian_product
      + fix mdarray::operator[] taking array indices
      + added gold::equal_mdspan
    | added internal meta library
      on <gold/bits/__util/meta.hpp> (3/27/2024)
    | ============================================
    | JUNE 2024
    | changed implementation of console print
      to match guidelines with [gold.format]
      on <gold/console> (6/20/2024)
    | [gold.numeric] rearrangement (6/23/2024)
      + new header <gold/math> and <gold/numeric>
      - removed header <gold/math_int> and <gold/numbers>
      ~ move <gold/bits/intervals.hpp> to <gold/bits/numeric/interval.hpp>
        (<gold/utility> to <gold/math>)
      ~ move <gold/math_int> to <gold/bits/numeric/math_int.hpp>
        (<gold/math_int> to <gold/math>)
      ~ move <gold/bits/numbers/constants.hpp> to <gold/bits/numeric/numbers.hpp>
        (<gold/numbers> to <gold/math>)
      ~ move <gold/bits/numbers/sat.hpp> to <gold/bits/numeric/sat_ops.hpp>
        (<gold/numbers> to <gold/numeric>)
     | added gold::math::normalize_value on <gold/math> (6/23/2024)
     | added gold::views::cache_last and
       gold::views::delimit on <gold/ranges> (6/23/2024)
    | added internal '__util::empty_temp_t'
      on <gold/bits/__util/empty.hpp> (6/28/2024)
    | added internal '__util::maybe_present_t'
      and '__util::maybe_present_or_t'
      on <gold/bits/__util/maybe_present_t.hpp> (6/28/2024)
    | ============================================
    | JULY 2024
    | added internal '__util::mini_inplace_vec'
      on <gold/bits/__util/mini_inplace_vec.hpp> (7/20/2024)
    | added internal '__util::mini_unordered_flat_set'
      on <gold/bits/__util/mini_unordered_flat_set.hpp> (7/20/2024)
    | added new header <gold/stopwatch> (7/23/2024)
        + gold::basic_stopwatch
        + gold::stopwatch
    | added new header <gold/inplace_vector> (7/23/2024)
        + gold::inplace_vector
    | ============================================
    | AUGUST 2024
    | fixed gold::any's swap crash
      if both operands are empty on <gold/any> (8/4/2024)
    | redefined gold::construct_at family in terms
      of placement-new on <gold/memory> (now supported in constexpr placement-new)
      (8/7/2024)
    | added new functions on <gold/memory> (8/7/2024)
        + gold::default_construct_at
        + gold::aggregate_construct_at
        + gold::construct_at_from
    | fixed missing constraint of 'gold::views::delimit'
      on <gold/ranges> (8/11/2024)
    | fixed 'gold::ranges::cycle_view' CTAD's Bound
      on <gold/ranges> (8/17/2024)
    | simplified range adaptors using deducing this
      on <gold/ranges> (8/17/2024)
    | ============================================
    | SEPTEMBER 2024
    | added internal string trim algorithms
      on <gold/bits/algo/str_trim.hpp> (9/27/2024)
    | allowed brace construction of 'gold::tuples::to'
      on <gold/tuples> (9/27/2024)
        + specializations of such templates like std::array
          can now be constructed in-place
    | added <gold/preview/linalg> as preview
      (9/30/2024)
    | ============================================
    | MAY 2025
    | rewrote makefile (5/23/2025)
    | rebuild libgold++.a (5/23/2025)
    | added new range concepts
      on <gold/bits/ranges/range_primitives> (5/23/2025)
      + gold::ranges::infinite_range
      + gold::ranges::nested_range
      + gold::ranges::statically_sized_range
      + gold::ranges::const_iterable_range
    | rewrote makefile again (5/26/2025)
    | added new header <gold/text_encoding> (5/26/2025)
      + gold::text_encoding
    | use more deducing this on <gold/bits/functional/*.hpp> (5/27/2025)
    | added internal gold::__algo::string_contains
      on <gold/bits/algo/contains.hpp> (5/28/2025)
    | added new header <gold/ascii> (5/28/2025)
    | added new header <gold/unicode> (5/28/2025)
    | added new member function gold::text_encoding::wide_literal()
      on <gold/text_encoding> (5/29/2025)
    | ============================================
    | DECEMBER 2025
    | renamed 'gold::is_char[_v]' -> 'gold::is_character_type[_v]'
      on <gold/type_traits> (12/19/2025)
    | renamed 'gold::is_structural[_v]' -> 'gold::is_structural_type[_v]'
      on <gold/type_traits> (12/19/2025)
    | added 'gold::is_scalar_numeric_type[_v]'
      on <gold/type_traits> (12/19/2025)
    | renamed 'gold::char_type' -> 'gold::character_type'
      on <gold/concepts> (12/19/2025)
    | renamed 'gold::qualified_numeric' -> 'gold::scalar_numeric_type'
      on <gold/concepts> (12/19/2025)
    | renamed 'gold::non_type[_t]' -> 'gold::constant_arg[_t]'
    | renamed 'gold::function_view' -> 'gold::function_ref'
      on <gold/functions> (12/19/2025)
    | removed 'gold::dummy_counting_iterator' and 'gold::enumerated_iterator'
      on <gold/iterator> (12/19/2025)
    | removed the following on <gold/vocabulary> (12/19/2025)
        - 'gold::optional_ref'
        - 'gold::make_optional_ref'
        - 'gold::make_optional_cref'
    | simplified <gold/math> math integer operation function signatures (12/19/2025)
        - pass by value instead of by reference
    | changed the consteval-branch of 'gold::assume'
      on <gold/utility> (12/19/2025)
    | added new items on <gold/math> (12/19/2025)
      + 'gold::math::first_prime_factor'
      + 'gold::math::euler_phi'
      + 'gold::math::extended_gcd[_result]'
      + 'gold::math::mod_[residue|mul|inv|div|pow]'
    | added 'gold::destructurable_type' on <gold/concepts> (12/19/2025)
    | added new items on <gold/tuples> (12/19/2025)
      + 'gold::tuples::destructurable_aggregate_type'
      + 'gold::tuples::aggregate_field_count[_v]'
      + 'gold::tuples::aggregate_get'
      + 'gold::tuples::aggregate_apply'
    | added new items on <gold/math> (12/23/2025)
      + 'gold::math::sum'
      + 'gold::math::product'
      + 'gold::math::inner_product'
    | added new header <gold/fiber_context> (12/29/2025)
      + 'gold::fiber_scope'
      + 'gold::fiber_error'
      + 'gold::fiber_context'
    | added new internal 'gold::__util::iota_array' (12/29/2025)
    | replaced most of the items in <gold/mdspan>
      by <mdspan> (12/29/2025)
    | ============================================
    | JANUARY 2026
    | added new items on <gold/math> (1/1/2026)
      + 'gold::math::[prev|next]_prime'
      + 'gold::math::is_perfect_square'
    | FEBRUARY 2026
    | revamped <gold/dynamic_library> (2/5/2026)
      - introducing 'gold::dynlib'
        - can be constructed via static factory methods only
        - provides options to error handling (checked [expected / exceptions] / unchecked)
        - massive renaming
        - lazy-evaluated symbol extraction (gold::dynlib::symbols_view)
        - can now query currently loaded modules in the current process (gold::dynlib::loaded_libraries_view)
        - removed comparison operators
