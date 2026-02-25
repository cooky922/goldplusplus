[gold++.v4]
+ <gold/console> [revamp]
+ <gold/dynamic_library> [revamp]
  + gold::dyn_lib
  + gold::dyn_lib::symbol_type
  + gold::dyn_lib::symbols_view
  + gold::dyn_lib_error_code
  + gold::dyn_lib_error
+ <gold/type_sequence> [revamp]
+ <gold/ranges> [new additions]
  + gold::views::cache_last [/]
  + gold::views::delimit [/]
  + gold::views::replace [.]
  + gold::views::replace_if [.]
  + gold::views::slice [.]
  + gold::views::scan [.]
  + gold::views::merge_by [.]
+ <gold/format> [new improvements]
  + needs unicode support from <gold/text>
+ <gold/mdspan> [new additions]
  + gold::layout_left_padded
  + gold::layout_right_padded
+ <gold/functional>
  + gold::function_view -> gold::function_ref
+ <gold/tagged_tuple> [+] [/]
+ <gold/poly_any> [+]
+ <gold/text> [+]
  + gold::text_encoding [.]
  + gold::text::uc
    + gold::uc::utf_view
      + gold::uc::utf8_view
      + gold::uc::utf16_view
      + gold::uc::utf32_view
    + gold::uc::grapheme_view
    + gold::uc::code_point
    + gold::uc::code_unit
+ <gold/stopwatch> [+]
  + gold::stopwatch [/]
+ <gold/flat_map> [+]
  + gold::flat_map
+ <gold/flat_set> [+]
  + gold::flat_set
+ <gold/inplace_vector> [+]
  + gold::inplace_vector [/]
+ <gold/preview/canvas> [+]
+ <gold/preview/os> [+]
+ <gold/preview/linalg> [+]
+ <gold/preview/stats> [+]

+ gold::aggregate_field_size[_v]
+ gold::aggregate_field_type[_t]
+ gold::aggregate_apply

ex:

gold::aggregate_apply (aggr, fn);

// equivalent to:
auto fwd [...args] = aggr;
fn (fwd args ...)

+ gold::emplace_from (fn)

- used for non-movable objects

/* make fn NTTP as possible but also callable */

ex:
constexpr auto fn = fnttp <Op>;
fn (args ...);

==================================
C++ build manager
------------------------------

cppc [simplified c++ toolkit]

cppc <file>.cpp [compile and build to <file>.exe]
cppc module <file>.cpp [compile as module and build to <file>.exe]
cppc build lib <proj>.cppc
cppc build dll <proj>.cppc

cppc get
cppc set
cppc add

==================================

ex:

gold::dyn_lib_errc::no_load
gold::dyn_lib_errc::no_symbol

add gold::basic_dyn_lib
 | with options -> caches result
		-> stores
 | must not be default constructible

gold::basic_dyn_lib<gold::>

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Data Processing
=======================
<gold/io/data/json>
<gold/io/data/xml>
<gold/io/data/yaml>
<gold/io/data/toml>
<gold/io/data/image>
<gold/io/data/audio>

gold::image vs. gold::canvas::image
+ gold::image is merely a data consisting of pixels
  with specified format
  | represented by only std::byte

+ gold::canvas::image is purely bytes of pixel
  with one format that can be drawn to screen.
  | represented by pixels of gold::canvas::color

  gold::canvas::drawable <gold::canvas::image> is true.

File System
=======================
<gold/fs>
 | gold::fs::path
 | gold::fs::path_view
 | gold::fs::file_entry

gold::fs::file_entry vs. gold::io::file
+ gold::fs::file_entry is a file with more properties
  such as its path, permission, and last updated

+ gold::io::file
  - file handle actively opened without name

ex: gold::fs::file_entry(gold::fs::path::current()).open()
    creates a gold::io::file object

==================================
<gold/linalg>

gold::linalg::basic_mat
 -> constructions
    -> runtime size
    -> construct with fill value
    -> construct with initial entries
 -> inplace transformations
    -> rotate()
    -> flip()
    -> transpose()

gold::linalg::basic_transform_mat

gold::linalg::basic_vec

===================================
gold::views::merge_by

ex:
std::vector<int> v1 { 1, 3, 4, 5 };
std::vector<int> v2 { 2, 4, 6 };

gold::views::merge_by(std::ranges::less, v1, v2)
| produces a view equal to [1, 2, 3, 4, 4, 5, 6]

====================================
std::vector<int> v { 1, 2, 3, 4, 5, 6, 7 };

v == [1, 2, 3, 4, 5, 6, 7]

v | views::chunk(3) == [[1, 2, 3], [4, 5, 6], [7]]

v | views::chunk(5) | views::join == views::all(v)

std::vector<int> w { 4, 4, 2, 3, 3, 3, 1, 4 };

w | views::chunk_by(ranges::equal_to) == [[4, 4], [2], [3, 3, 3], [1], [4]]

==========================================
[gold++.v4]

+ Math
  <gold/linalg>
  <gold/stats>

+ Data Processing
  <gold/data/zip>
  <gold/data/json>
  <gold/data/xml>
  <gold/data/toml>
  <gold/data/yaml>
  <gold/data/image>
  <gold/data/audio>
  <gold/data/base64>

<gold/io>
 + gold::io::file

<gold/fs>
 + gold::fs::path

<gold/dyn_lib>

gold::dyn_lib::current().dependencies()
gold::dyn_lib("...").contains_name("...")
gold::dyn_lib("...").symbols()

