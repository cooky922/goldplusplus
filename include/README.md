# Note:
Official Directory

# To-Do:
## `[gold.types.basic]`
+ `gold::simple_function_ptr`
+ `gold::simple_function_ref`

```c++
namespace gold {

  /// simple_function_ptr
  template <typename T>
    requires std::is_function_v<T>
  using simple_function_ptr = T*;
  
  /// simple_function_ref
  template <typename T>
    requires std::is_function_v<T>
  using simple_function_ref = T&;

} // namespace gold
```

## `[gold.functional.functions]`
+ `gold::function` - `constexpr`-compatible `std::function`
+ `gold::unique_function` - `constexpr`-compatible `std::move_only_function`

```c++
namespace gold {
 
  /// function
  template <typename...>
  class function;
  
  template <typename R, typename... Args>
  class function<R(Args...) /* cv */ /* ref */ noexcept(/* is-noexcept */)>;
  
  template <typename R, typename... Args>
  class function<R(Args......) /* cv */ /* ref */ noexcept(/* is-noexcept */)>;
  
  /// unique_function
  template <typename...>
  class unique_function;
  
  template <typename R, typename... Args>
  class unique_function<R(Args...) /* cv */ /* ref */ noexcept(/* is-noexcept */)>;
  
  template <typename R, typename... Args>
  class unique_function<R(Args......) /* cv */ /* ref */ noexcept(/* is-noexcept */)>;

} // namespace gold
```

## `[gold.types.concepts]`

```c++
namespace gold {
  
  /// bit_convertible_to
  template <typename From, typename To>
  concept bit_convertible_to = /* see description */;
  
  /// invocable_r
  template <typename R, typename F, typename... Args>
  concept invocable_r = std::is_invocable_r_v<R, F, Args...> && std::convertible_to<std::invoke_result_t<F, Args...>, R>;
  
} // namespace gold
```

## `[gold.format]`
Revamp Text Formatting

+ `gold::format_write_context`
+ `gold::format_parse_context`

## `[gold.functional]`
+ `gold::curry`
+ `gold::uncurry`
+ `gold::compose`
+ `gold::make_pipeable`
+ `gold::bind_at`

## `[gold.smart_ptrs]`
+ `gold::stationary_ptr`
+ `gold::unique_ptr`
+ `gold::shared_ptr` (unsynchronized version for compile-time support)
+ `gold::cloned_ptr`
+ `gold::borrowed_ptr`
+ `gold::out_ptr[_t]`
+ `gold::inout_ptr[_t]`

```c++
namespace gold {

  /// bad_pointer_access
  class bad_pointer_access;

  /// default_deleter
  template <typename>
  class default_deleter;
  
  template <typename T>
  class default_deleter<T[]>;
  
  /// default_cloner
  template <typename>
  class default_cloner;
  
  template <typename T>
  class default_cloner<T[]>;
  
  /// stationary_ptr
  template <typename T, typename D = default_deleter<T>>
  class stationary_ptr;
  
  template <typename T, typename D>
  class stationary_ptr<T[], D>;
  
  /// unique_ptr
  template <typename T, typename D = default_deleter<T>>
  class unique_ptr;
  
  template <typename T, typename D>
  class unique_ptr<T[], D>;
  
  /// shared_ptr
  template <typename T>
  class shared_ptr;
  
  template <typename T>
  class shared_ptr<T[]>;
  
  /// cloned_ptr
  template <typename T, typename C = default_cloner<T>, typename D = default_deleter<T>>
  class cloned_ptr;
  
  template <typename T, typename C, typename D>
  class cloned_ptr<T[], C, D>;
  
  /// make_stationary_ptr
  template <typename T, typename... Args>
  constexpr stationary_ptr<T> make_stationary_ptr(Args&&...);
  
  /// make_stationary_ptr_for_overwrite
  template <typename T, typename... Args>
  constexpr stationary_ptr<T> make_stationary_ptr_for_overwrite() noexcept;
  
  /// allocate_stationary_ptr
  template <typename T, typename Alloc, typename... Args>
  constexpr auto allocate_stationary_ptr(Alloc&, Args&&...);
  
  /// make_unique_ptr
  template <typename T, typename... Args>
  constexpr unique_ptr<T> make_unique_ptr(Args&&...);
  
  /// make_unique_ptr_for_overwrite
  template <typename T>
  constexpr unique_ptr<T> make_unique_ptr_for_overwrite() noexcept;
  
  /// allocate_unique_ptr
  template <typename T, typename Alloc, typename... Args>
  constexpr unique_ptr<T> allocate_unique_ptr(Alloc&, Args&&...);
  
  /// make_shared_ptr
  template <typename T, typename... Args>
  constexpr shared_ptr<T> make_shared_ptr(Args&&...);
  
  /// make_shared_ptr_for_overwrite
  template <typename T>
  constexpr shared_ptr<T> make_shared_ptr_for_overwrite() noexcept;
  
  /// allocate_shared_ptr
  template <typename T, typename Alloc, typename... Args>
  constexpr shared_ptr<T> allocate_shared_ptr(Alloc&, Args&&...);
  
  /// make_cloned_ptr
  template <typename T, typename... Args>
  constexpr cloned_ptr<T> make_cloned_ptr(Args&&...);
  
  /// make_cloned_ptr_for_overwrite
  template <typename T>
  constexpr cloned_ptr<T> make_cloned_ptr_for_overwrite() noexcept;
  
  /// allocate_cloned_ptr
  template <typename T, typename Alloc, typename... Args>
  constexpr cloned_ptr<T> allocate_cloned_ptr(Alloc&, Args&&...);

} // namespace gold
```

## `[gold.types.movable_tuple]`
+ `gold::movable_tuple` - implemented using lambda expressions

## `[gold.type.tuples]`
+ `gold::tuples::get_like`
+ `gold::tuples::size`
+ `gold::tuples::apply_each_n`
+ `gold::tuples::zip`
+ `gold::tuples::unzip`
+ `gold::tuples::concat`
+ `gold::tuples::fold_left`
+ `gold::tuples::fold_right`
+ `gold::tuples::fold_left_first`
+ `gold::tuples::fold_right_last`

```c++

namespace gold::tuples {

  inline namespace /* unspecified */ {
    // get_like
    template <std::size_t>
    inline constexpr /* unspecified */ get_like = /* unspecified */;
  }
  
  inline namespace /* unspecified */ {
    // apply_each_n
    template <std::size_t>
    inline constexpr /* unspecified */ apply_each_n = /* unspecified */;
  }
  
  inline namespace /* unspecified */ {
    // zip
    inline constexpr /* unspecified */ zip = /* unspecified */;
  }
  
  inline namespace /* unspecified */ {
    // unzip
    inline constexpr /* unspecified */ unzip = /* unspecified */;
  }
  
  inline namespace /* unspecified */ {
    // fold_left
    inline constexpr /* unspecified */ fold_left = /* unspecified */;
  }
  
  inline namespace /* unspecified */ {
    // fold_right
    inline constexpr /* unspecified */ fold_right = /* unspecified */;
  }
  
  inline namespace /* unspecified */ {
    // fold_left_first
    inline constexpr /* unspecified */ fold_left_first = /* unspecified */;
  }
  
  inline namespace /* unspecified */ {
    // fold_right_last
    inline constexpr /* unspecified */ fold_right_last = /* unspecified */;
  }

} // namespace gold::tuples
```

## `[gold.coroutines]`

### `[gold.coroutines.generator]`
+ `gold::invocable_generator`
+ `gold::state_generator`
+ `gold::io_generator`

### `[gold.coroutines.task]`
+ revamp `gold::task`

## `[gold.containers]`
+ `gold::mdspan`

## `[gold.numeric]`
+ `gold::basic_number` and `gold::number`
+ `gold::math::vector` and `gold::math::matrix`
+ `gold::fraction`
+ `gold::to_unsafe_bytes`
+ `gold::basic_int_2x` and `gold::basic_uint_2x`
+ `gold::basic_int_nx` and `gold::basic_uint_nx`
+ `gold::float_parts`

## `[gold.ranges]`
+ `gold::views::adjacent`
+ `gold::views::adjacent_transform`
+ `gold::views::cycle`
+ `gold::views::concat`
+ `gold::views::cartesian_product`

## `[gold.preview.lazy_ranges]`
+ `gold::lazy_ranges::cartesian_product`
+ `gold::lazy_ranges::zip`
+ `gold::lazy_ranges::offset`
+ `gold::lazy_ranges::continue_with`

## `[gold.preview.io]`

### `[gold.preview.io.file]`
```c++
namespace gold::io {

  /// file_descriptor
  enum class file_descriptor {};

  /// open_mode
  class open_mode {
   public:
    enum class /* unspecified */ {
      read, write, append, binary, extended
    };
    using enum /* unspecified */;

    static constexpr open_mode from_string(std::string_view) noexcept;
    constexpr std::string_view to_string() const noexcept;
  };

  /// seek_mode
  enum class seek_mode {
    begin,
    current,
    end
  };

  /// file_pos
  class file_pos;
  
  //// Low-Level and General File Handle
  /// file_handle
  class file_handle;

  //// File Handle Objects
  inline /* unspecified */ in = /* unspecified */;
  inline /* unspecified */ out = /* unspecified */;
  inline /* unspecified */ err = /* unspecified */;

  //// Interoperability with C++ Standard Library I/O
  file_handle from_fstream(const std::fstream&);
  file_handle from_fstream(const std::ofstream&);
  file_handle from_fstream(const std::ifstream&);

} // namespace gold::io
```

### `[gold.preview.io.stream]`
```c++
namespace gold::io {

  //// Stream Concepts
  /// input_stream
  template <typename T>
  concept input_stream = /* see description */;
  
  /// output_stream
  template <typename T>
  concept output_stream = /* see description */;
  
  /// stream
  template <typename T>
  concept stream = input_stream<T> || output_stream<T>;
  
  /// input_output_stream
  template <typename T>
  concept input_output_stream = input_stream<T> && output_stream<T>;
  
  /// seekable_stream
  template <typename T>
  concept seekable_stream = stream<T> && /* see description */;

  //// Stream Based Operations
  /// stream_base
  struct stream_base;

  /// file_stream
  class file_stream : public stream_base;
  
  /// span_stream
  class span_stream : public stream_base;
  
  /// memory_stream
  class memory_stream : public stream_base;

  //// Stream Writer / Reader Operations
  /// text_writer
  class text_writer; 

  /// text_reader
  class text_reader;

  /// binary_writer
  class binary_writer;

  /// binary_reader
  class binary_reader;

} // namespace gold::io
```

### `[gold.preview.io.fs]`
```c++
namespace gold::io::fs {

  /// filesystem_error
  class filesystem_error;

  /// file_status
  class file_status;	

  /// permission_kind
  enum class permission_kind;

  /// set_permission_options
  enum class set_permission_options;

  /// space_info
  class space_info;

  /// file_kind
  enum class file_kind {
    none,
    not_found,
    regular,
    directory,
    symlink,
    block,
    character,
    fifo,
    socket,
    unknown
  };
  
  /// copy_options
  enum class copy_options;

  /// path
  class path;

  /// file_entry
  class file_entry;

  /// directory_entry
  class directory_entry;

  /// socket_entry
  class socket_entry;

} // namespace gold::io::fs

namespace gold { namespace fs = namespace io::fs; }
```

## `[gold.preview.concurrent]`
### `[gold.preview.concurrent.thread]`
```c++
namespace gold {
  
  /// thread_status
  enum class thread_status;
  
  /// stop_source
  class stop_source;
  
  /// stop_token
  class stop_token;
  
  /// thread
  class thread {
   public:
    /// creation
    thread() noexcept = default;
    thread(const thread&) = delete;
    thread(thread&&) noexcept;
    thread& operator=(const thread&) = delete;
    thread& operator=(thread&&);
    ~thread();
    
    template <typename F, typename... Args>
    static thread spawn(F&&, Args&&...);
    
    template <typename F, typename... Args>
    static thread named_spawn(std::string_view, F&&, Args&&...);
    
    /// types
    class id;
    using native_handle_type = /* implementation defined */;
    
    /// observers
    static std::size_t hardware_concurrency() noexcept;
    id get_id() const noexcept;
    
    void name(std::string_view);
    std::string name() const noexcept;
    
    static thread& get_thread_by_name(std::string_view);
    
    thread_status status() const noexcept;
    
    /// current thread
    class current {
     public:
      static thread& get_thread();
      void yield_now() noexcept;
      void sleep_for(const gold::chrono::qualified_duration auto&);
      void sleep_until(const gold::chrono::qualified_time_point auto&);
      id get_id() noexcept;
      
      void name(std::string_view);
      std::string name() const noexcept;
    };
    
    /// operations
    void join();
    void detach();
    bool joinable() const noexcept;
    
    stop_source get_stop_source();
    stop_token get_stop_token();
    bool request_stop();
  };
  
  /// lazy_thread
  class lazy_thread;
  
}
```

### `[gold.preview.concurrent.sync]`
```c++
namespace gold {

  /// synced
  template <typename T>
  class synced;
  
  /// channel
  template <typename Ref, typename Value = /* ... */>
  class channel;

} // namespace gold
```

## `[gold.preview.data]`
+ `gold::image`
+ `gold::bitmap`
+ `gold::audio`

## `[gold.preview.system]`
+ `gold::sys::shell`
+ `gold::sys::process`
+ `gold::sys::power_status`
+ `gold::sys::global_memory_status`
+ `gold::sys::space_info`
+ `gold::sys::os_info`
+ `gold::sys::regkey`

## `[gold.preview.apps]`
+ `gold::app`
+ `gold::window_base`
+ `gold::default_window`

```c++
namespace gold {

  /// window_base
  template <typename Derived>
  class window_base {
   public:
    using native_handle_type = /* implementation defined */;
    using derived_type       = Derived;
   
    constexpr window_base();
    window_base(const window_base&) = delete;
    
    virtual ~window_base();
    
    native_handle_type native_handle() const noexcept;
    
   protected:
    virtual std::string_view do_get_class_name() const = 0;
    virtual void do_render() const = 0;
    virtual void do_exit() const = 0;
    virtual void do_input() const = 0;
    virtual void do_timer() const = 0;
    virtual void do_start() const = 0;
  };
  
  /// default_window
  class default_window : public window_base<default_window>;
  
  /// app
  template <typename Window = default_window>
  class app;

} // namespace gold
```
## `[gold.preview.canvas]`
+ `gold::canvas::color`
+ `gold::canvas::color_hex`
+ `gold::canvas::color_rgb`
+ `gold::canvas::color_cmyk`
+ `gold::canvas::color_hsl`

```c++
namespace gold::canvas {

  /// color_hex
  using color_hex = /* at least 3 bytes integer */;
  
  /// color_rgb
  struct color_rgb;
  
  /// color_cmyk
  struct color_cmyk;
  
  /// color_hsl
  struct color_hsl;
  
  /// color
  class color {
   public:
    static constexpr color from_hex(canvas::color_hex);
    static constexpr color from_rgb(canvas::color_rgb);
    static constexpr color from_cmyk(canvas::color_cmyk);
    static constexpr color from_hsl(canvas::color_hsl);
    
    constexpr canvas::color_hex hex() const noexcept;
    constexpr void hex(canvas::color_hex) noexcept;
    constexpr canvas::color_rgb rgb() const noexcept;
    constexpr void rgb(canvas::color_rgb) noexcept;
    constexpr canvas::color_cmyk cmyk() const noexcept;
    constexpr void cmyk(canvas::color_cmyk) noexcept;
    constexpr canvas::color_hsl hsl() const noexcept;
    constexpr void hsl(canvas::color_hsl) noexcept;
    
    enum class filter_mode {
      brightness, 
      contrast,
      hue_rotate,
      invert,
      opacity,
      saturate,
      sepia
    };
    
    static constexpr color mix(std::initializer_list<color>) noexcept;
    static constexpr color filter(color, filter_mode, /* unspecified */);
    static constexpr color color_filter(color, color);
  };
  
  namespace colors {
    inline static constexpr color 
      red = /* ... */,
      blue = /* ... */,
      yellow = /* ... */;
      
    /// ... more ...
  
  } // namespace colors

} // namespace gold::canvas
```

## `[gold.preview.graphics]`
+ `gold::graphics`
+ `gold::canvas_context`
+ `gold::canvas_stream`
+ `gold::canvas::basic_point`
+ `gold::canvas::basic_region`
+ `gold::canvas::font_style`
+ `gold::canvas::basic_font`
+ `gold::canvas::brush`
+ `gold::canvas::hatch_brush`
+ `gold::canvas::gradient_brush`
+ `gold::canvas::solid_brush`
+ `gold::canvas::texture_brush`
+ `gold::canvas::basic_pen`
+ `gold::canvas::basic_rectangle`
+ `gold::canvas::basic_circle`
+ `gold::canvas::basic_ellipse`
+ `gold::canvas::basic_quadrilateral`
+ `gold::canvas::basic_path`
+ `gold::canvas::basic_arc`
+ `gold::canvas::basic_pie`
+ `gold::canvas::basic_line`
+ `gold::canvas::basic_polygon`
+ `gold::canvas::bitmap`
+ `gold::canvas::image`
+ `gold::canvas::color`
