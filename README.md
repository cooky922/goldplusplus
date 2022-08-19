# gold++ 
* A custom library written by an author :>
* It requires at least C++23 support and GCC 12 compiler
* It also requires windows platform
* This project is still undergoing construction

## Goal
* Utilizing modern C++ features and also retain its old functionality.
* Simplified and relaxed semantics of some entities.
* Simple wrapper for some APIs

## Status
* `[gold++.1]` was released in December 2021
* `[gold++.2]` was out for preview on July 31, 2022
* `[gold++.2]` is expected to release in August 2022

## Categories
* `[gold.text]` - Text processing sublibrary
  * `[gold.text.format]` - text formatting
  * `[gold.text.demangling]` - name demangling
  * `[gold.text.charset]` - character sets
* `[gold.io]` - I/O sublibrary
  * `[gold.io.console]` - terminal or console API
* `[gold.meta]` - Metaprogramming sublibrary
  * `[gold.meta.types]` - types aliases and name
  * `[gold.meta.concepts]` - type concepts
  * `[gold.meta.traits]` - type traits
  * `[gold.meta.traits.invocable]` - invocable type trait
  * `[gold.meta.pack]` - parameter pack algorithms
  * `[gold.meta.sequence]` - meta sequences
  * `[gold.meta.sequence.values]` - value sequences
  * `[gold.meta.sequence.types]` - type sequences
* `[gold.utility]`
  * `[gold.utility.ops]` - general purpose functions and casts
  * `[gold.utility.program]` - program behavior changing functions
  * `[gold.utility.scope_guard]` - scope guard
  * `[gold.utility.any]` - type-safe and type-erased type
  * `[gold.utility.expected]` - result type
* `[gold.functional]` - Functional sublibrary
  * `[gold.functional.functions]` - functions type
  * `[gold.functional.bind]` - partial binders
  * `[gold.functional.compose]` - function composition
  * `[gold.functional.overload]` - function overloading
* `[gold.numeric]` - Numeric sublibrary
  * `[gold.numeric.numbers]` - mathematical constants and numeric traits
  * `[gold.numeric.ops]` - operations
  * `[gold.numeric.random]` - random generation
  * `[gold.numeric.ratio]` - compile-time ratio
* `[gold.tuples]` - Tuple facilities sublibrary
* `[gold.containers]` - Containers sublibrary
  * `[gold.containers.struct]` - Structural containers
* `[gold.ranges]` - Ranges sublibrary
  * `[gold.ranges.iterators]` - range iterators
  * `[gold.ranges.algorithm]` - range algorithms
  * `[gold.ranges.to]` - range conversion
  * `[gold.ranges.views]` - range views
* `[gold.coro]` - Coroutine sublibrary
  * `[gold.coro.traits]` - coroutine traits
  * `[gold.coro.generator]` - generator coroutine
  * `[gold.coro.lazy]` - lazy coroutine
* `[gold.memory]` - Memory management sublibrary
  * `[gold.memory.smart_ptrs]` - smart pointers
  * `[gold.memory.alloc]` - allocators
  * `[gold.memory.algorithms]` - memory algorithms
* `[gold.dynamic_library]` - dynamic module library
* `[gold.preview.apps]`
* `[gold.preview.system]`
* `[gold.preview.canvas]`
* `[gold.preview.numeric.linear]`
* 
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
