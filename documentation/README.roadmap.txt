gold++ roadmap
==================================

Current Development | TODO
----------------------------
| + gold::basic_zstring_view
| + gold::ranges::projected_view
| + gold::tuples::into_tuple

| [gold++.utility.dynamic_library]
  + revamp <gold/dynamic_library>
    + gold::dynamic_library -> gold::dyn_lib

| [gold++.io.terminal]
  + add <gold/terminal> replacing <gold/console>

+ [gold++.system]
  + add <gold/runtime> and <gold/os> [gold++.system.runtime] [gold++.system.os]

| [gold++.utility.any.poly]
  + add: <gold/poly_any>

| [gold++.canvas]
  + improve: <gold/preview/canvas>

| [gold++.numeric.math.linalg]
  + add: <gold/preview/linalg>

| [gold++.utility.memory]
  + modify: <gold/memory>
    + add: constexpr gold::shared_ptr and gold::weak_ptr

| [gold++.container.flat]
  + add gold::flat_map
  + add gold::flat_set

| [gold++.__internals__]
  + gold::__util::is_valid_instantiation
  + gold::__util::defer_t

The Big Roadmap
----------------------------
 | [gold++.numeric.math] Math Libraries
   | [gold++.numeric.math.linalg] Linear Algebra
   | [gold++.numeric.math.stats] Statistics

 | [gold++.pattern] Pattern Matching

 | [gold++.canvas] Abstract Canvas Library

 | [gold++.app] Application Development Toolkit Library
   | [gold++.app.graphics] Graphics Library
   | [gold++.app.gui] GUI Library

 | [gold++.system] System Library
   | [gold++.system.runtime] Program Runtime
   | [gold++.system.os] Operating System Information

 | [gold++.io] I/O Library
   | [gold++.io.events] I/O Event Handling
   | [gold++.io.terminal] Terminal

 | [gold++.io.data] Specialized Data Processing
   | [gold++.io.data.csv]
   | [gold++.io.data.xml]
   | [gold++.io.data.yaml]
   | [gold++.io.data.json]
   | [gold++.io.data.base64]
   | [gold++.io.data.image]
   | [gold++.io.data.audio]

 | [gold++.text] Text Processing
   | [gold++.text.encoding] Text Encoding Identification
   | [gold++.text.unicode] Unicode Support

Development Level Legend
----------------------------
| [STAGE 0]
  -> planned
  -> no interface yet

| [STAGE 1]
  -> has interface
  -> no implementation yet

| [STAGE 1.5]
  -> has interface
  -> has initial implementation (half-baked)

| [STAGE 2]
  -> has interface and implementation

| [STAGE 3]
  -> has final interface and implementation

| [STAGE 4]
  -> approved and adopted in library

PATCH 2024.Next Plan (TODO, wala pa nahuman ug edit)
----------------------------------------------------
| [gold++.general] General
  -> new implementation naming schemes
     -> unified namespace '__goldx'

~~~~~
| [gold++.utility.fiber_context] Fiber Context
  + add: <gold/fiber_context>
  + gold::fiber_context

| [gold++.app] App Development Kit Library
  + [gold++.app.stage] Windows Handle
  + [gold++.app.scene] Windows Frame
  + [gold++.app.canvas] Graphics Context (independent of any application)

~~~~~

| [gold++.macros] [new]
  -> new header <gold/macros>
     + __GOLDM_ASSERT(<expr>, <msg> ?)
     + __GOLDM_IS_LVALUE(<expr>)
     + __GOLDM_IS_XVALUE(<expr>)
     + __GOLDM_IS_PRVALUE(<expr>)
     + __GOLDM_IS_CONSTEXPR(<expr>)
     + __GOLDM_MOVE(<expr>)
     + __GOLDM_FWD(<expr>)
     + __GOLDM_FWD_LIKE(<expr>, <type>)
     + __GOLDM_LIFT(<fn>)
     + __GOLDM_TYPE_NAME(<expr>)
     + __GOLDM_UNEVAL(<type>)

~~~~~

| [gold++.ranges]
  -> new additions in <gold/ranges>
     + gold::ranges::range_adaptor_traits [type] [stage 0]

  -> new additions in <gold/iterator>
     + gold::iter_concept_t [type-alias] [stage 1]

  -> new additions in <gold/algorithm>
     + gold::ranges::shift_left [stage 1]
     + gold::ranges::shift_right [stage 1]

~~~~~

| [gold++.text.format]
  -> add thousand separator support for
     integers without using locales
       ex: gold::format("{}", 1234567) == "1234567"
           gold::format("{:~,}", 1234567) == "1,234,567"
           gold::format("{:~ }", 1234567) == "1 234 567"

~~~~~

| [gold++.text.demangling]
  -> new additions in <gold/demangling>
     + gold::demangling_kind [enum] [stage 0]
       + gold::demangling_kind::itanium
       + gold::demangling_kind::msvc

~~~~~

| [gold++.memory]
  -> new additions in <gold/memory>
     + gold::bad_pointer_access [type] [stage 2]
     + gold::borrowed_ptr [type] [stage 1.5]
     + gold::stationary_ptr [type] [stage 1.5]
     + gold::shared_ptr [type] [stage 1]
     + gold::mem [namespace] [stage 0]
     + gold::mem::default_allocator [stage 0]
     + gold::mem::pool_allocator [stage 0]
     + gold::byte_buffer [stage 0]
     + gold::basic_buffer [stage 0]

~~~~~

| [gold++.meta.concepts]
  -> more concepts in <gold/concepts>
     + gold::bit_convertible_to [stage 0]
     + gold::invocable_r [stage 0]

~~~~~

| [gold++.utility.any]
  -> changes in <gold/any>
     + add gold::basic_any [type] [stage 0]
     + add gold::view_any::to_address() -> void* [mem-fn] [stage 0]
     ~ fix gold::any_cast which lacks constant propagation [stage 0]
     ~ fix invalidation of mem-fn .view() after the
       owner has changed its type [stage 0]
     - remove gold::view_any::operator==,operator<=> [op] [stage 0]

  -> new header <gold/poly_any> [gold++.utility.any.poly]
     + gold::poly_any [stage 2]

| [gold++.utility.fiber_context] [new]
   -> new header <gold/fiber_context>
      + gold::fiber_context [type] [stage 1

| [gold++.utility.stacktrace]
  -> new additions in <gold/stacktrace>
     + gold::stacktrace::from_current_exception [stage 0]

~~~~~

| [gold++.io.console] under <gold/console>
  -> possible overhaul of the interface and implementation [stage 0]
  -> changes in gold::console_type
     + add gold::console_type::error [mem-fn] [stage 0]
     + add gold::console_type::error_line [mem-fn] [stage 0]
     + add gold::console_type::scan_output [mem-fn] [stage 0]
     ~ rename other member function names with less
       lettery [stage 0]

~~~~~

| [gold++.tuples]
  -> new additions in <gold/tuples>
     + gold::tuples::apply_each_n [op] [stage 1.5]
     + gold::tuples::unzip [op] [stage 0]

  -> new header <gold/tuple>
     + gold::movable_tuple [type] [stage 1.5]

~~~~~

| [gold++.functional]
   -> new additions in <gold/functional>
      + gold::bind_at [op] [stage 1]
      + gold::default_overload [var] [stage 0]
      + gold::make_overload extenstions [stage 0]
        + member functions
      + gold::enable_bind_placeholder [stage 0]
      + gold::bind_placeholder[_t] and its UDL [stage 0]
      + gold::partial_bind [stage 1.5]
        + reference: [sandbox/plain_bind.cpp]

    -> new additions in <gold/functions>
       + gold::function [stage 0]
       + gold::unique_function [stage 0]

~~~~~

| [gold++.numeric]
  -> revamping and rebranching sublibraries
  -> see [programming notepad / gold.numeric.txt]

| [gold++.numeric.math.ratio]
  -> new additions in <gold/ratio>
     + runtime ratios [stage 0]

| [gold++.canvas]
  -> see [programming notepad / gold.canvas.txt]

| [gold++.system]
  -> see [programming notepad / gold.runtime_and_system.txt]

| [gold++.io] and [gold++.io.fs]
  -> see [programming notepad / gold.io.txt]

| [gold++.pattern]
  -> see [programming notepad / gold++ pattern matching.txt]

| [gold++.concurrency.thread]
  -> new header <gold/thread>
     + namespace gold::threads [stage 2]
     + gold::threads::spawn [stage 1]
     + gold::threads::basic_thread [stage 1]
     + gold::threads::thread [stage 1]
     + gold::threads::basic_lazy_thread [stage 0]
     + gold::threads::lazy_thread [stage 0]
     + gold::threads::current [stage 0]

| [gold++.coro.generator]
  -> new additions in <gold/generator>
     + gold::invocable_generator [stage 0]
     + gold::io_generator [stage 0]

| [gold++.unclassified] Unclassified Entities
  + gold::compiled_current_time
  + gold::compiled_current_date
  + gold::compiled_current_timestamp
