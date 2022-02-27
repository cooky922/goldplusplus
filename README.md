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
* `[gold.format]` - Test Formatting
* `[gold.io]` - I/O Library
  * `[gold.io.console]` - Console API
* `[gold.types]` - Types Library
  * `[gold.types.basic]` - type aliases
  * `[gold.types.name]` - readable type information
  * `[gold.types.traits]` - type traits
  * `[gold.types.invocable_traits]` - invocable type traits
  * `[gold.types.concepts]` - basic type concepts
  * `[gold.types.sequence]` - type sequence
* `[gold.utility]` - General Purpose Utilities
  * `[gold.utility.ops]`
  * `[gold.utility.scope_guard]`
  * `[gold.utility.tuple]` - tuple and other tuple-related utilities
  * `[gold.utility.any]` - type-safe type-erased type
  * `[gold.utility.expected]` - alternative to exceptions
  * `[gold.utility.vocabulary]` - vocabulary types
  * `[gold.utility.demangling]` - demangling using Itanium ABI
  * `[gold.utility.pack]` - Parameter Pack Algorithms
  * `[gold.utility.value_sequence]` - Value Sequence
* `[gold.functional]` - Functional-Programming Library
* `[gold.diagnostic]` - Diagnostic Library
  * `[gold.diagnostic.assertion]`
* `[gold.containers]` - Containers and Collections Library
  * `[gold.containers.struct_array]`
* `[gold.math]` - Mathematics Library
  * `[gold.math.numbers]` - Mathematical Constants and Number Traits
  * `[gold.math.math_int]` - Integer Arithmetic
  * `[gold.math.random]` - Random Generation
  * `[gold.math.ratio]` - Compile-time Ratio
* `[gold.ranges]` - Generalized Ranges and Iterators Library
  * `[gold.ranges.iterators]` - Iterators
* `[gold.coroutines]` - Coroutine Library
  * `[gold.coroutines.traits]` - Coroutine Traits and other Awaitable Concepts
  * `[gold.coroutines.lazy]` - Lazy Evaluated Function
  * `[gold.coroutines.task]`
  * `[gold.coroutines.generator]`
* `[gold.smart_ptrs]` - Smart Pointers
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
