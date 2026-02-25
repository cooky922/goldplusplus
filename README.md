# gold++ 
It is a personal mini library for latest C++
which provides a set of various tools depending
on the usage.

NOTE: This project is still under construction.

## What are the goals?
* leveraging modern C++ features to simplify implementation
* providing simple wrappers for some Windows APIss

### Want to see list of items and headers?
* see [documentation/README.items.txt]

### Want to see list of changelog for the past patches?
* see [documentation/README.changelogs.txt]

### Want to see the plan for future versions?
* see [documentation/README.roadmap.txt]

## Building the Library `[gold++]`
### Requirements
To compile the `libgold++.a` static library from source, ensure your environment meets the following requirements:
* **Operating System:** Unfortunately, only Windows OS is supported for now.
* **C++ Compiler:** A compiler with C++26 support (e.g., `g++` version 15 or newer).
* **Build Tools:** GNU `make`, along with standard binutils (`ar`, `strip`, `nm`).
* **Dependencies:** The `nonstdlib` library. The build system expects this to be available locally to resolve header inclusions.

### Build Instructions
1.  **Open your terminal** and navigate to the root directory of the `gold++` project.
2.  **Run the build command.** If your `nonstdlib` folder is located right next to this project folder (the default `../nonstdlib` path), simply run:
    ```bash
    make
    ```
3.  **Specify a custom dependency path (if needed):** If your `nonstdlib` library is stored somewhere else on your machine, you can easily point the compiler to it by overriding the `NONSTDLIB_DIR` variable:
    ```bash
    make NONSTDLIB_DIR="C:/path/to/your/custom/nonstdlib"
    ```

### Output
A successful build will generate the following files in the `lib/` directory:
* `libgold++.a`: The compiled static archive, ready to be linked into your executables.
* `libgold++_sym.txt`: A dumped list of the library's symbols.

### Cleaning Up
To remove all compiled object files (`.o`), intermediate files, and the generated library so you can start fresh, run:
```bash
make clean
```

## History of `[gold++]`
### Upcoming
* `[gold++.v4]`
  * expected to be released in June 2026

### Released
* `[gold++.v3]`
  * released on August 20, 2023
  * pre-released on August 5, 2023

* `[gold++.v2]`
  * released on August 28, 2022
  * pre-released on July 31, 2022

* `[gold++.v1]`
  * released on December 31, 2021

## Categories
* [gold++.text] Text processing
  * [gold++.text.format] Text Formatting
  * [gold++.text.demangling] Name Demangling
  * [gold++.text.charset] Character Sets
  * [gold++.text.unicode] Unicode handling (New)
  * [gold++.text.ascii] ASCII Character Sets (New)

* [gold++.io] I/O Routines
  * [gold++.io.terminal] Terminal or Console API

* [gold++.meta] Metaprogramming
  * [gold++.meta.types] Type Aliases and Names
  * [gold++.meta.concepts] Type Concepts
  * [gold++.meta.traits] Type Traits
    * [gold++.meta.traits.invocable] Invocable Type Traits
  * [gold++.meta.pack] Parameter Pack Algorithms
  * [gold++.meta.sequence] Meta Sequences
    * [gold++.meta.sequence.values] Value Sequences
    * [gold++.meta.sequence.types] Type Sequences

* [gold++.utility] Utility
  * [gold++.utility.ops] General purpose functions and casts
  * [gold++.utility.program] Program behavior changing Functions
  * [gold++.utility.stacktrace] Stacktrace
  * [gold++.utility.scope_guard] Scope Guard
  * [gold++.utility.clipboard] Clipboard API
  * [gold++.utility.any] Type-safe and Type-erased Type
  * [gold++.utility.expected] Result Type

* [gold++.functional] Functional
  * [gold++.functional.functions] Function types
  * [gold++.functional.bind] Partial Binders
  * [gold++.functional.compose] Function Composition
  * [gold++.functional.overload] Function Overloading

* [gold++.tuples] Tuple Facilities
* [gold++.ranges] Ranges
  * [gold++.ranges.iterators] Range Iterators
  * [gold++.ranges.algo] Range Algorithms
  * [gold++.ranges.to] Range Conversion
  * [gold++.ranges.views] Range Views

* [gold++.containers] Containers
  * [gold++.containers.struct] Structural Containers

* [gold++.memory] Memory Management
  * [gold++.memory.smart_ptr] Smart Pointers
  * [gold++.memory.alloc] Allocators
  * [gold++.memory.algo] Memory Algorithms

* [gold++.coro] Coroutine
  * [gold++.coro*.traits] Coroutine Traits
  * [gold++.coro.generator] Generators
  * [gold++.coro.lazy] Lazy Coroutine

* [gold++.numeric] Numeric
  * [gold++.numeric.random] Random Generation
  * [gold++.numeric.ratio] Compile-time Ratio
  * [gold++.numeric.traits] Numeric Traits
  * [gold++.numeric.sat_op] Saturated Integer Operations
  * [gold++.numeric.ext_int] Extended Integer Types [[preview]]
  * [gold++.numeric.math.numbers] Math Constants
  * [gold++.numeric.math.linalg] Linear Algebra [[preview]]
  * [gold++.numeric.math.stat] Statistics [[preview]]

* [gold++.canvas] Canvas [[preview]]

## Roadmap
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
   * `[gold++.data.toml]`
   * `[gold++.data.yaml]`
   * `[gold++.data.base64]`
   * `[gold++.data.image]`
   * `[gold++.data.audio]`
