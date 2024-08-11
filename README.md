# gold++ 
* A personal library project written by our own author :>
* This project is still undergoing construction

## Goal
* to apply modern C++ features
* to simplify and relax the semantics of some entities
* to serve as a simple wrapper for some API

## Specific Requirement upon using `[gold++]`
* OS: Windows
* Compiler: GCC 15 +
* Language Version: C++26
* Link Against Built: `libgold++.a`

## History of `[gold++]`
### Upcoming
* `[gold++.v4]`
  * expected to be released in September 2024

### Released
* `[gold++.v3`
  * released on August 20, 2023
  * pre-released on August 5, 2023

* `[gold++.v2]`
  * released on August 28, 2022
  * pre-released on July 31, 2022

* `[gold++.v1]`
  * released on December 31, 2021

## Categories
* `[gold++.text]` Text processing
  * `[gold++.text.format]` Text Formatting
  * `[gold++.text.demangling]` Name Demangling
  * `[gold++.text.charset]` Character Sets

* `[gold++.io]` I/O Routines
  * `[gold++.io.console]` Terminal or Console API

+ `[gold++.meta]` Metaprogramming
  + `[gold++.meta.types]` Type Aliases and Names
  + `[gold++.meta.concepts]` Type Concepts
  + `[gold++.meta.traits]` Type Traits
    + `[gold++.meta.traits.invocable]` Invocable Type Traits
  + `[gold++.meta.pack]` Parameter Pack Algorithms
  + `[gold++.meta.sequence]` Meta Sequences
    + `[gold++.meta.sequence.values]` Value Sequences
    + `[gold++.meta.sequence.types]` Type Sequences

+ `[gold++.utility]` Utility
  + `[gold++.utility.ops]` General purpose functions and casts
  + `[gold++.utility.program]` Program behavior changing Functions
  + `[gold++.utility.stacktrace]` Stacktrace
  + `[gold++.utility.scope_guard]` Scope Guard
  + `[gold++.utility.clipboard]` Clipboard API
  + `[gold++.utility.any]` Type-safe and Type-erased Type
  + `[gold++.utility.expected]` Result Type

+ `[gold++.functional]` Functional
  + `[gold++.functional.functions]` Function types
  + `[gold++.functional.bind]` Partial Binders
  + `[gold++.functional.compose]` Function Composition
  + `[gold++.functional.overload]` Function Overloading

+ `[gold++.tuples]` Tuple Facilities
+ `[gold++.ranges]` Ranges
  + `[gold++.ranges.iterators]` Range Iterators
  + `[gold++.ranges.algo]` Range Algorithms
  + `[gold++.ranges.to]` Range Conversion
  + `[gold++.ranges.views]` Range Views

+ `[gold++.containers]` Containers
  + `[gold++.containers.struct]` Structural Containers

+ `[gold++.memory]` Memory Management
  + `[gold++.memory.smart_ptr]` Smart Pointers
  + `[gold++.memory.alloc]` Allocators
  + `[gold++.memory.algo]` Memory Algorithms

+ `[gold++.coro]` Coroutine
  + `[gold++.coro.traits]` Coroutine Traits
  + `[gold++.coro.generator]` Generators
  + `[gold++.coro.lazy]` Lazy Coroutine

+ `[gold++.numeric]` Numeric
  + `[gold++.numeric.random]` Random Generation
  + `[gold++.numeric.ratio]` Compile-time Ratio
  + `[gold++.numeric.traits]` Numeric Traits
  + `[gold++.numeric.sat_op]` Saturated Integer Operations
  + `[gold++.numeric.ext_int]` Extended Integer Types [[preview]]
  + `[gold++.numeric.math.numbers]` Math Constants
  + `[gold++.numeric.math.linalg]` Linear Algebra [[preview]]
  + `[gold++.numeric.math.stat]` Statistics [[preview]]

+ `[gold++.canvas]` Canvas [[preview]]

## Roadmap
* `[gold++.numeric.math]` Math Libraries
* `[gold++.pattern]` Pattern Matching
* `[gold++.canvas]` Canvas Library
* `[gold++.graphics]` Graphics Library
* `[gold++.gui]` GUI Library
* `[gold++.os]` OS Wrapper Library
* `[gold++.io]` I/O Library
   * `[gold++.io.events]` I/O Event Handling
* `[gold++.data]` Data Processing
   * `[gold++.data.csv]`
   * `[gold++.data.xml]`
   * `[gold++.data.json]`
   * `[gold++.data.image]`
   * `[gold++.data.audio]`
