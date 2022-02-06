# gold++ 
* A custom library written by an author :>
* It requires at least C++23 support and GCC 12 compiler
* It also requires windows platform
* This project is still undergoing construction

## Goal
* Utilizing modern C++ features and also retain its old functionality.
* Simplified and relaxed semantics of some entities.
* Simple wrapper for some APIs

## Categories
* `[gold.format]` - text formatting
* `[gold.io]`
* `[gold.value_sequence]`
* `[gold.type_sequence]`
* `[gold.tuples]` - tuple utilities
* `[gold.pack]` - algorithms for parameter packs
* `[gold.type_name]` - readable type name
* `[gold.type_traits]` - type traits
* `[gold.invocable_traits]` - type traits for invocable types
* `[gold.concepts]` - concepts
* `[gold.scope_guard]`
* `[gold.functional]`
* `[gold.vocabulary]`
* `[gold.assertion]`
* `[gold.struct_array]`
* `[gold.numbers]`
* `[gold.math_int]`
* `[gold.random]`
* `[gold.iterator]`
* `[gold.ranges]`
* `[gold.task]`
* `[gold.generator]`
* `[gold.unique_ptr]`
* `[gold.utility]`
* `[gold.any]`
* `[gold.expected]`
* `[gold.console]`
* `[gold.dynamic_library]`
* `[gold.preview.apps]`
* `[gold.preview.graphics]`
* `[gold.preview.lazy_ranges]`
* `[gold.preview.linear]`

## Roadmap
* Improved Text Formatting
* Windows GUI Wrapper
* Graphics Library - canvas, basic graphical object types
* Mathematics Library - lineary algebra, calculus, statistics, type-erased number
* File Parsing - `.csv`, `.xml`, and `.json`
* Device Handling - input events 
* OpenGL C++ Wrapper and GDI+ Wrapper through `[gold.graphics]`

## Modes of Compilation
* `gold++` library requires static linking with static library `libgold++.a`

## Dependencies 
* Win API
* GDI+
* Direct2D and DirectWrite
* OpenGL at least `v3.3` (once `[gold.preview.graphics]` is available)
