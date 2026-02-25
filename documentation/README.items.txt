gold++ items
======================================

List of Items in Each Header
-----------------------------
<gold/algorithm>
    | gold::ranges::contains[_subrange]
    | gold::ranges::[starts|ends]_with
    | gold::ranges::out_value_result
    | gold::ranges::in_value_result
    | gold::ranges::iota[_result]
    | gold::ranges::fold_left_with_iter[_result]
    | gold::ranges::fold_left_first_with_iter[_result]
    | gold::ranges::fold_left[_first]
    | gold::ranges::fold_right[_right]
    | gold::ranges::lexicographical_compare_three_way
    | gold::ranges::find_last[_if][_not]
    | gold::ranges::is_uniqued

<gold/any>
    | gold::any
    | gold::holds_current_type
    | gold::make_any
    | gold::any_cast
    | gold::unique_any
    | gold::make_unique_any
    | gold::view_any
    | gold::in_place_viewable[_t]

<gold/ascii>
    | gold::is_ascii
    | gold::is_ascii_digit
    | gold::is_ascii_bit
    | gold::is_ascii_octal_digit
    | gold::is_ascii_hex_digit
    | gold::is_ascii_lower_alpha
    | gold::is_ascii_upper_alpha
    | gold::is_ascii_alpha
    | gold::is_ascii_alphanumeric
    | gold::is_ascii_punctuation
    | gold::is_ascii_graphical
    | gold::is_ascii_printable
    | gold::is_ascii_horizontal_whitespace
    | gold::is_ascii_whitespace
    | gold::is_ascii_control
    | gold::ascii_to_lower
    | gold::ascii_to_upper

<gold/assertion>
    | gold::assertion_error
    | gold::system_assert

<gold/basic_types>
    | gold::int_t
    | gold::short_t
    | gold::int[8|16|32|64|128|max|min]_t
    | gold::uint_t
    | gold::ushort_t
    | gold::uint[8|16|32|64|128|max|min]_t
    | gold::float[16|32|64|128|max]_t
    | gold::float[16|32|64|128]x_t
    | gold::bfloat[16|min]_t
    | gold::floatmax_t
    | gold::decimal[32|64|128|max|min]_t
    | gold::[|d|q]word_t
    | gold::tchar_t
    | gold::cstring_t
    | gold::wcstring_t
    | gold::tcstring_t
    | gold::restricted
    | gold::vectorized

<gold/bit>
    | gold::to_bytes
    | gold::to_writable_bytes
    | gold::from_bytes
    | gold::bit_reverse
    | gold::bit_repeat
    | gold::next_bit_permutation
    | gold::prev_bit_permutation
    | gold::bit_compress[l|r]
    | gold::bit_expand[l|r]

<gold/clipboard>
    | gold::clipboard_error
    | gold::basic_clipboard
    | gold::clipboard
    | gold::wclipboard

<gold/concepts>
    | gold::closure_type
    | gold::generic_closure_type
    | gold::stateless_closure_type
    | gold::character_type
    | gold::ordering_type
    | gold::complete_type
    | gold::structural_type
    | gold::scalar_numeric_type
    | gold::similar_to
    | gold::same_all
    | gold::different_from
    | gold::brace_constructible_from
    | gold::disable_string_like
    | gold::basic_string_like
    | gold::string_like
    | gold::wstring_like
    | gold::basic_stream_insertable
    | gold::basic_stream_extractable
    | gold::stream_insertable
    | gold::wstream_insertable
    | gold::stream_extractable
    | gold::wstream_extractable
    | gold::range_basic_stream_insertable
    | gold::range_stream_insertable
    | gold::range_wstream_insertable
    | gold::relocatable
    | gold::layout_compatible_with
    | gold::decayable
    | gold::[implicitly|explicitly]_convertible_to
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
    | gold::stateless_allocator

<gold/console>
    | gold::console_type
    | gold::console

<gold/coroutine>
    | gold::always_suspend
    | gold::never_suspend
    | gold::suspend_if
    | gold::suspend_maybe
    | gold::coroutine_error
    | gold::simple_awaitable
    | gold::awaitable

<gold/ctype_info>
    | gold::ctype_info
    | gold::ctype_id

<gold/demangling>
    | gold::demangle

<gold/dynamic_library>
    | gold::dynlib_error_code
    | gold::dynlib_error
    | gold::dynlib

<gold/expected>
    | gold::bad_expected_access
    | gold::bad_unexpected_access
    | gold::unexpected
    | gold::unexpect[_t]
    | gold::expected

<gold/fiber_context>
    | gold::fiber_error
    | gold::fiber_scope
    | gold::fiber_context

<gold/format>
    | gold::format_error
    | gold::format_parse_context
    | gold::format_context
    | gold::format_arg
    | gold::format_args
    | gold::formatter
    | gold::formattable
    | gold::vformat_to
    | gold::format_to
    | gold::vformat
    | gold::format
    | gold::formatted_size
    | gold::format_to_n_result
    | gold::format_to_n
    | gold::to_formatted[_string]
    | gold::format_string
    | gold::unchecked_format_string
    | gold::make_format_args
    | gold::visit_format_arg
    | gold::range_formatter
    | gold::tuple_formatter
    | gold::range_format_kind
    | gold::range_format
    | gold::try_format
    | gold::try_format_result
    | gold::try_formatter_parse_result
    | gold::try_formatter_write_result
    | gold::format_error_code

<gold/functional>
    | gold::make_overload
    | gold::unwrap_ref
    | gold::invoke[_r]
    | gold::consteval_invoke[_r]
    | gold::void_like[_v]
    | gold::try_invoke
    | gold::bind_back
    | gold::bind_front
    | gold::bind_rest
    | gold::compose
    | gold::convert_as
    | gold::construct_object

<gold/functions>
    | gold::function_ref

<gold/generator>
    | gold::ranges::yieldable_view
    | gold::ranges::to_yieldable
    | gold::generator

<gold/inplace_vector>
    | gold::inplace_vector

<gold/invocable_traits>
    | gold::is_noexcept[_v]
    | gold::remove_noexcept[_t]
    | gold::add_noexcept[_t]
    | gold::has_lvalue_reference_qualifier[_v]
    | gold::has_rvalue_reference_qualifier[_v]
    | gold::has_reference_qualifier[_v]
    | gold::remove_reference_qualifier[_t]
    | gold::add_lvalue_reference_qualifier[_t]
    | gold::add_rvalue_reference_qualifier[_t]
    | gold::has_const_qualifier[_v]
    | gold::has_volatile_qualifier[_v]
    | gold::has_cv_qualifier[_v]
    | gold::has_cvref_qualifier[_v]
    | gold::has_cvref_qualifier_or_noexcept[_v]
    | gold::remove_const_qualifier[_t]
    | gold::remove_volatile_qualifier[_t]
    | gold::remove_cv_qualifier[_t]
    | gold::remove_cvref_qualifier[_t]
    | gold::remove_cvref_qualifier_noexcept[_t]
    | gold::add_const_qualifier[_t]
    | gold::add_volatile_qualifier[_t]
    | gold::add_cv_qualifier[_t]
    | gold::has_va_args[_v]
    | gold::va_args_tag[_t]
    | gold::invoke_arity[_v]
    | gold::invoke_return[_t]
    | gold::invoke_parameter[_t]
    | gold::is_overloaded[_v]

<gold/iterator>
    | gold::indirect_value_result_t
    | gold::projected_value
    | gold::null_sentinel[_t]
    | gold::iterator_interface_access
    | gold::iterator_interface
    | gold::proxy_iterator_interface
    | gold::proxy_arrow_result

<gold/lazy>
    | gold::lazy

<gold/math>
    | gold::math::midpoint
    | gold::math::signum
    | gold::math::power
    | gold::math::log_2
    | gold::math::is_perfect_square
    | gold::math::square_root
    | gold::math::cube_root
    | gold::math::factorial
    | gold::math::permutation
    | gold::math::combination
    | gold::math::binomial_coeff
    | gold::math::digit_count
    | gold::math::is_prime
    | gold::math::[next|prev]_prime
    | gold::math::are_co_prime
    | gold::math::first_prime_factor
    | gold::math::euler_phi
    | gold::math::extended_gcd[_result]
    | gold::math::mod_[residue|mul|inv|div|pow]
    | gold::math::sum
    | gold::math::product
    | gold::math::inner_product
    | gold::math::closed_interval
    | gold::math::[left_|right_|]open_interval
    | gold::math::unit_interval
    | gold::math::transmute_value
    | gold::math::normalize_value
    | gold::numbers::tau[_v]
    | gold::numbers::silver_ratio[_v]
    | gold::numbers::inv_phi[_v]
    | gold::numbers::w1_omega[_v]

<gold/mdarray>
    | gold::mdarray

<gold/mdspan>
    | gold::equal_mdspan

<gold/memory>
    | gold::voidify
    | gold::[iter_]construct_at
    | gold::[iter_]relocate_at
    | gold::relocate
    | gold::[iter_]destroy_at
    | gold::destroy[_n]
    | gold::default_construct_at
    | gold::aggregate_construct_at
    | gold::construct_at_from
    | gold::uninit_default_construct[_n]
    | gold::uninit_fill[_n]
    | gold::uninit_copy[_n][_result]
    | gold::uninit_move[_n][_result]
    | gold::uninit_relocate[_n][_result]
    | gold::in_ptr[_t]
    | gold::inout_ptr[_t]
    | gold::cloned_ptr
    | gold::make_cloned
    | gold::make_cloned_for_overwrite
    | gold::allocate_cloned_ptr
    | gold::derived[_t]
    | gold::default_deleter
    | gold::default_cloner
    | gold::uninitialized

<gold/numeric>
    | gold::sat_add
    | gold::sat_sub
    | gold::sat_mul
    | gold::sat_div
    | gold::saturate_cast

<gold/pack>
    | gold::pack_size
    | gold::pack_element_at
    | gold::pack_at_front
    | gold::pack_at_back
    | gold::pack_for_each
    | gold::pack_for_each_index
    | gold::pack_get
    | gold::pack_apply_index
    | gold::pack_fold_left
    | gold::pack_fold_right

<gold/print>
    | gold::vprint
    | gold::print
    | gold::println

<gold/random>
    | gold::qualified_distribution_type
    | gold::random_seed_t
    | gold::random_number_generator
    | gold::default_random_generator
    | gold::random

<gold/ranges>
    | gold::ranges::range_adaptor_closure_object
    | gold::ranges::range_adaptor_closure
    | gold::ranges::range_adaptor
    | gold::ranges::infinite_range
    | gold::ranges::nested_range
    | gold::ranges::statically_sized_range
    | gold::ranges::const_iterable_range
    | gold::from_range[_t]
    | gold::ranges::to
    | gold::ranges::generate_view
        | gold::views::generate
    | gold::ranges::maybe_view
        | gold::views::maybe
    | gold::ranges::move_each_view
        | gold::views::move_each
    | gold::ranges::const_each_view
        | gold::views::const_each
    | gold::ranges::cycle_view
        | gold::views::cycle
        | gold::views::cycle_n
    | gold::ranges::concat_view
        | gold::views::concat
    | gold::ranges::delimit_view
        | gold::views::delimit
    | gold::ranges::cache_last_view
        | gold::views::cache_last

<gold/ratio>
    | gold::ratio_t
    | gold::ratio
    | gold::to_ratio
    | [ some ratio constants ... ]

<gold/scope_guard>
    | gold::scope_guard

<gold/stacktrace>
    | gold::stacktrace_style
    | gold::stacktrace_entry
    | gold::stacktrace

<gold/stopwatch>
    | gold::basic_stopwatch
    | gold::stopwatch

<gold/struct_array>
    | gold::struct_array

<gold/struct_string>
    | gold::struct_string

<gold/sync_value>
    | gold::sync_value

<gold/tagged_tuple>
    | gold::tagged_tuple
    | gold::tagged_member
    | gold::tagged_arg
    | gold::tagged_get

<gold/text_encoding>
    | gold::text_encoding

<gold/tuples>
    | gold::tuples::tuple_size[_v]
    | gold::tuples::tuple_element[_t]
    | gold::tuples::get
    | gold::tuples::get_element
    | gold::tuples::get_key
    | gold::tuples::get_value
    | gold::tuples::size
    | gold::tuples::gettable_at
    | gold::tuples::gettable
    | gold::tuples::tuple_like
    | gold::tuples::tuple_like_for_size
    | gold::tuples::empty_like
    | gold::tuples::single_like
    | gold::tuples::pair_like
    | gold::tuples::enable_array_like
    | gold::tuples::array_like
    | gold::tuples::applicable
    | gold::tuples::apply_result[_t]
    | gold::tuples::apply
    | gold::tuples::apply_each
    | gold::tuples::apply_each_result[_t]
    | gold::tuples::concat
    | gold::tuples::to
    | gold::tuples::fold_left[_first]
    | gold::tuples::fold_right[_last]
    | gold::tuples::zip
    | gold::tuples::visit_at
    | gold::tuples::visitable_with
    | gold::tuples::visitor_for
    | gold::tuples::destructurable_aggregate_type
    | gold::tuples::aggregate_field_count[_v]
    | gold::tuples::aggregate_get
    | gold::tuples::aggregate_apply

<gold/type_name>
    | gold::type_name[|_r|_no_cvref|_no_collapse]
    | gold::raw_t[|n|t|nt]t_arg_names
    | gold::template_arg_names

<gold/type_sequence>
    | gold::type_seq[_t]
    | gold::empty_type_seq[_t]
    | gold::concat_type_seq[_t]
    | gold::zip_type_seq[_t]

<gold/type_traits>
    | gold::constant[_t]
    | gold::types_inherited
    | gold::size_constant
    | gold::index_constant[_v]
    | gold::always_true
    | gold::always_false
    | gold::is_character_type[_v]
    | gold::is_ordering_type[_v]
    | gold::is_scalar_numeric_type[_v]
    | gold::is_complete_type[_v]
    | gold::is_structural_type[_v]
    | gold::pointer_rank[_v]
    | gold::is_not_same[_v]
    | gold::are_same[_v]
    | gold::are_not_same[_v]
    | gold::is_any[_v]
    | gold::is_specialization_of_v
    | gold::is_template_instantiation[_v]
    | gold::conditionals[_t]
    | gold::type_apply[_t]
    | gold::type_apply_if[_t]
    | gold::disable_if[_t]
    | gold::is_detected[_v]
    | gold::detected_t
    | gold::detected_or[_t]
    | gold::is_detected_exact[_v]
    | gold::is_detected_convertible[_v]
    | gold::invoke_class_template[_t]
    | gold::bases[_t]
    | gold::direct_bases[_t]
    | gold::has_bases[_v]
    | gold::is_direct_base_of[_v]
    | gold::is_indirect_base_of[_v]
    | gold::base_at[_t]
    | gold::direct_base_at[_t]
    | gold::base_count[_v]
    | gold::direct_base_count[_v]
    | gold::is_convertible_without_narrowing[_v]

<gold/unicode>
    | gold::uc::transcoding_error
    | gold::uc::transcoding_state
    | gold::uc::to_utf8[|_view]
    | gold::uc::to_utf16[|_view]
    | gold::uc::to_utf32[|_view]
    | gold::uc::to_utf8_string
    | gold::uc::to_utf8_u8string
    | gold::uc::to_utf16_string
    | gold::uc::to_utf32_string
    | gold::uc::validate_utf_sequence

<gold/utility>
    | gold::in_place_braced[_t]
    | gold::in_place_type_braced[_t]
    | gold::in_place_index_braced[_t]
    | gold::in_place_value[_t]
    | gold::in_place_value_braced[_t]
    | gold::enum_ops::enable_enum_ops
    | gold::to_unsigned
    | gold::to_signed
    | gold::as_consteval
    | gold::as_const
    | gold::as_mutable
    | gold::decay_copy
    | gold::decay_move
    | gold::discard_unused
    | gold::unreachable
    | gold::constant_arg[_t]
    | gold::assume

<gold/value_sequence>
    | gold::value_seq[_t]
    | gold::integer_seq[_t]
    | gold::index_seq[_t]
    | gold::int_seq[_t]
    | gold::char_seq[_t]
    | gold::bool_seq[_t]
    | gold::floating_point_sequence[_t]
    | gold::float_seq[_t]
    | gold::make_[integer|index]_seq[_t]
    | gold::index_seq_for[_t]
    | gold::make_consecutive_[integer|index]_seq[_t]
    | gold::make_reverse_[integer|index]_seq[_t]
    | gold::make_interval_[integer|index]_seq[_t]
    | gold::make_repeat_[integer|index]_seq[_t]
    | gold::value_sequence
    | gold::make_array_from_sequence
    | gold::make_sequence_from_array

<gold/vocabulary>
    | gold::any_to_variant_cast
    | gold::variant_cast

List of Broken Headers
----------------------
    <gold/task>
    <gold/tuple>
    <gold/tuple_ranges>
    <gold/char_class>

List of Upcoming Headers (in any order)
---------------------------------------
    revamped <gold/tuple_ranges>
    <gold/stats>
    <gold/linalg>
    <gold/strong_typedefs>
    <gold/units>
    <gold/io>
    <gold/fs>
    <gold/os>
