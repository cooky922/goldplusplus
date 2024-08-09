// <gold/bits/sequence/type_seq.hpp> - gold++ library

// Copyright (C) [ 2021 - 2024 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_SEQUENCE_TYPE_SEQ_HPP

#include <gold/bits/__util/uneval.hpp>
#include <gold/bits/type_traits/specialization_of.hpp>
#include <gold/bits/type_traits/conditional.hpp>
#include <gold/bits/sequence/value_seq.hpp>
#include <gold/bits/sequence/make_integer_seq.hpp>
#include <gold/bits/sequence/type_fwd.hpp>
#include <gold/bits/sequence/type_tokens.hpp>

namespace gold {

    namespace __type_seq {

        /// __type_seq::qualified_type_seq
        template <typename T>
        concept qualified_type_seq = gold::is_specialization_of_v<T, gold::type_seq_t>();

        /// __type_seq::extract_from_indices
        template <template <typename...> typename Temp, typename T, typename Indices>
        struct extract_from_indices {
            using type = decltype([]<std::size_t... Is>(gold::index_seq_t<Is...>) {
                return __util::uneval<Temp<typename T::template at<Is>...>>;
            }(Indices {})());
        };

        /// __type_seq::simple_type_info
        struct simple_type_info {
            const bool* id_ptr;
            friend constexpr bool operator==(simple_type_info, simple_type_info) noexcept = default;
        };

        /// __type_seq::simple_typeid
        template <typename T>
        consteval simple_type_info simple_typeid() noexcept {
            static constexpr bool dummy = true;
            return { .id_ptr = &dummy };
        }

        /// __type_seq::simple_type_info_array
        template <std::size_t N>
        struct simple_type_info_array {
            const simple_type_info elem[N];

            constexpr auto begin() const noexcept { return elem; }
            constexpr auto end() const noexcept { return elem + N; }
            static consteval std::size_t size() noexcept { return N; }
            consteval auto data() const noexcept { return elem; }
        };

        template <>
        struct simple_type_info_array<0> {
            static consteval const simple_type_info* begin() noexcept { return nullptr; }
            static consteval const simple_type_info* end() noexcept { return nullptr; }
            static consteval std::size_t size() noexcept { return 0; }
            static consteval const simple_type_info* data() noexcept { return nullptr; }
        };

        /// __type_seq::simple_type_info_subrange
        struct simple_type_info_subrange {
            const simple_type_info* iter;
            const simple_type_info* sent;

            consteval auto begin() const noexcept { return iter; }
            consteval auto end() const noexcept { return sent; }
            consteval std::size_t size() const noexcept { return sent - iter; }
            consteval bool empty() const noexcept { return iter == sent; }
        };

        /// __type_seq::get_type_ids
        template <qualified_type_seq Seq>
        consteval auto get_type_ids() noexcept {
            if constexpr (Seq::template empty<>)
                return simple_type_info_array<0>{};
            else
                return []<typename... Ts>(type_seq_t<Ts...>*) {
                    return simple_type_info_array<sizeof...(Ts)> {{
                        __type_seq::simple_typeid<Ts>() ...
                    }};
                }(static_cast<Seq*>(nullptr));
        }

        /// __type_seq::search_seq
        consteval simple_type_info_subrange search_seq(simple_type_info_subrange r1,
                                                       simple_type_info_subrange r2) {
            for (;; ++r1.iter) {
                auto it1 = r1.iter;
                for (auto it2 = r2.iter;; ++it1, ++it2) {
                    if (it2 == r2.sent)
                        return {.iter = r1.iter, .sent = it1};
                    if (it1 == r1.sent)
                        return {.iter = it1, .sent = it1};
                    if (*it1 != *it2)
                        break;
                }
            }
            __builtin_unreachable();
        }

        /// __type_seq::contains_subrange
        consteval bool contains_subrange(simple_type_info_subrange r1,
                                         simple_type_info_subrange r2) {
            return r2.empty() || !__type_seq::search_seq(r1, r2).empty();
        }

        /// __type_seq::mismatch_result
        struct mismatch_result {
            std::size_t n1;
            std::size_t n2;
        };

        /// __type_seq::mismatch_seq
        consteval mismatch_result mismatch_seq(simple_type_info_subrange r1,
                                               simple_type_info_subrange r2) {
            auto iter1 = r1.iter;
            auto iter2 = r2.iter;
            for (; iter1 != r1.sent && iter2 != r2.sent; ++iter1, ++iter2)
                if (*iter1 != *iter2)
                    break;
            return {.n1 = static_cast<std::size_t>(iter1 - r1.iter),
                    .n2 = static_cast<std::size_t>(iter2 - r2.iter)};
        }

        /// __type_seq::div_ceil
        consteval std::size_t div_ceil(std::size_t num, std::size_t denom) {
            std::size_t r = num / denom;
            if (num % denom)
                ++r;
            return r;
        }

    } // namespace __type_seq

    /// type_seq_t<>
    template <>
    struct type_seq_t<> {
      private:
        /// ::self [private]
        using self = type_seq_t;

        /// ::seq_size [private]
        inline static constexpr std::size_t seq_size = 0;

        /// ::seq_empty [private]
        inline static constexpr bool seq_empty = true;

      public:
        /// ctor
        consteval type_seq_t() noexcept = default;

        /// ::size
        template <typename... Unused>
            requires (sizeof...(Unused) == 0)
        inline static constexpr std::size_t size = self::seq_size;

        /// ::empty
        template <typename... Unused>
            requires (sizeof...(Unused) == 0)
        inline static constexpr bool empty = self::seq_empty;

        /// ::unpack
        template <template <typename...> typename Temp>
            requires requires { typename Temp<>; }
        using unpack = Temp<>;

        /// ::unpack_q
        template <typename Temp>
        using unpack_q = self::unpack<Temp::template Fn>;

        /// ::concat_with
        template <__type_seq::qualified_type_seq... Seqs>
        using concat_with = decltype((__util::uneval<self&&>() + ... + __util::uneval<Seqs&&>()));

        /// ::append
        template <typename... Us>
        using append = self::concat_with<type_seq_t<Us...>>;

        /// ::prepend
        template <typename... Us>
        using prepend = type_seq_t<Us...>::template concat_with<self>;

        /// ::push_front
        template <typename U>
        using push_front = type_seq_t<U>;

        /// ::push_back
        template <typename U>
        using push_back = type_seq_t<U>;

        /// ::reverse
        template <typename... Unused>
            requires (sizeof...(Unused) == 0)
        using reverse = self;

        /// ::take
        template <std::size_t>
        using take = self;

        /// ::take_last
        template <std::size_t>
        using take_last = self;

        /// ::drop
        template <std::size_t>
        using drop = self;

        /// ::insert_at
        template <std::size_t N, typename... Us>
            requires (N == 0)
        using insert_at = self::append<Us...>;

        /// ::equals
        template <__type_seq::qualified_type_seq Other>
        inline static constexpr bool equals = Other::seq_empty;

        /// ::repeat
        template <std::size_t>
        using repeat = self;

        /// ::all_of
        template <template <typename> typename>
        inline static constexpr bool all_of = true;

        /// ::all_of_q
        template <typename Pred>
        inline static constexpr bool all_of_q = self::all_of<Pred::template Fn>;

        /// ::none_of
        template <template <typename> typename>
        inline static constexpr bool none_of = true;

        /// ::none_of_q
        template <typename Pred>
        inline static constexpr bool none_of_q = self::none_of<Pred::template Fn>;

        /// ::any_of
        template <template <typename> typename>
        inline static constexpr bool any_of = false;

        /// ::any_of_q
        template <typename Pred>
        inline static constexpr bool any_of_q = self::any_of<Pred::template Fn>;

        /// ::contains_single
        template <typename>
        inline static constexpr bool contains_single = false;

        /// ::contains
        template <__type_seq::qualified_type_seq>
        inline static constexpr bool contains = false;

        /// ::starts_with_single
        template <typename>
        inline static constexpr bool starts_with_single = false;

        /// ::starts_with
        template <__type_seq::qualified_type_seq>
        inline static constexpr bool starts_with = false;

        /// ::ends_with_single
        template <typename>
        inline static constexpr bool ends_with_single = false;

        /// ::ends_with
        template <__type_seq::qualified_type_seq>
        inline static constexpr bool ends_with = false;

        /// ::flatten
        template <typename... Unused>
            requires (sizeof...(Unused) == 0)
        using flatten = self;

        /// ::operator+
        template <typename... Us>
        friend consteval auto operator+(type_seq_t, type_seq_t<Us...>) {
            return type_seq_t<Us...>{};
        }

        /// ::operator==
        friend consteval auto operator==(type_seq_t, type_seq_t) { return true; }
    };

    /// empty_type_seq_t
    using empty_type_seq_t = type_seq_t<>;

    /// empty_type_seq
    inline constexpr empty_type_seq_t empty_type_seq {};

    /// type_seq_t
    template <typename... Ts>
    struct type_seq_t {
      private:
        /// ::self [private]
        using self = type_seq_t;

        /// ::seq_size [private]
        inline static constexpr std::size_t seq_size = sizeof...(Ts);

        /// ::seq_empty [private]
        inline static constexpr bool seq_empty = false;

      public:
        /// ::size
        template <typename... Unused>
            requires (sizeof...(Unused) == 0)
        inline static constexpr std::size_t size = self::seq_size;

        /// ::empty
        template <typename... Unused>
            requires (sizeof...(Unused) == 0)
        inline static constexpr bool empty = self::seq_empty;

        /// ::at
        template <std::size_t I>
            requires (I < self::seq_size)
        using at = gold::pack_element_at<I, Ts...>;

        /// ::front
        template <typename... Unused>
            requires (sizeof...(Unused) == 0)
        using front = self::at<0>;

        /// ::back
        template <typename... Unused>
            requires (sizeof...(Unused) == 0)
        using back = self::at<self::seq_size - 1>;

      private:
        /// ::pop_front_impl [private]
        // note: doesn't work in alias form
        template <typename T, typename... Us>
        struct pop_front_impl {
            using type = type_seq_t<Us...>;
        };

        /// ::pop_back_impl [private]
        template <typename T, typename... Us>
        struct pop_back_impl {
            static constexpr auto sf_get_type_() {
                if constexpr (sizeof...(Us) == 0)
                    return __util::uneval<type_seq_t<>>;
                else
                    return __util::uneval<
                        typename pop_back_impl<Us...>::type::template push_front<T>
                    >;
            }

            using type = decltype(sf_get_type_()());
        };

        /// ::extract_from_indices_impl [private]
        template <typename Indices>
        struct extract_from_indices_impl {
            template <std::size_t... Is>
            static consteval auto sf_type_(gold::index_seq_t<Is...>) {
                return __util::uneval<
                    type_seq_t<typename self::template at<Is>...>
                >;
            }
            using type = decltype(sf_type_(Indices{})());
        };

        /// ::for_each_impl [private]
        template <typename F>
        static constexpr void for_each_impl(F&& f) {
            [&]<std::size_t... Is>(gold::index_seq_t<Is...>) {
                (f.template operator()<typename self::template at<Is>>(), ...);
            }(gold::make_index_seq<self::seq_size>);
        }

        /// ::find_if_impl [private]
        template <template <typename> typename Pred>
        struct find_if_impl {
            inline static constexpr int value = [] -> int {
                int  pos       = 0;
                bool has_found = false;

                self::for_each_impl([&pos, &has_found]<typename T>{
                    if constexpr (Pred<T>::value)
                        has_found = true;
                    if (!has_found)
                        ++pos;
                });

                if (!has_found)
                    return -1;

                return pos;
            }();

            using type = decltype([]{
                if constexpr (value == -1)
                    return gold::sentinel_token;
                else
                    return gold::index_token<value>;
            }());
        };

        /// ::find_if_not_impl [private]
        template <template <typename> typename Pred>
        struct find_if_not_impl {

            /// ::binded_temp
            template <typename T>
            using binded_temp = std::bool_constant<!Pred<T>::value>;

            /// ::result
            using result = self::find_if_impl<binded_temp>;

            /// ::value
            inline static constexpr int value = result::value;

            /// ::type
            using type = result::type;

        };

        /// ::find_impl [private]
        template <typename Query>
        struct find_impl {

            /// ::binded_temp
            template <typename T>
            using binded_temp = std::bool_constant<__is_same(T, Query)>;

            /// ::result
            using result = self::find_if_impl<binded_temp>;

            /// ::value
            inline static constexpr int value = result::value;

            /// ::type
            using type = result::type;

        };

        /// ::find_last_if_impl [private]
        template <template <typename> typename Pred>
        struct find_last_if_impl {

            /// ::value
            inline static constexpr int value = [] -> int {
                int  pos       = 0;
                bool has_found = false;

                self::for_each_impl([&pos, &has_found]<typename T> {
                    if constexpr (Pred<T>::value)
                        has_found = true;
                    if (!has_found)
                        pos++;
                });

                if (!has_found)
                    return -1;

                return self::seq_size - pos - 1;
            }();

            /// ::type
            using type = decltype([]{
                if constexpr (value == -1)
                    return gold::sentinel_token;
                else
                    return gold::index_token<value>;
            }());

        };

        /// ::find_last_if_not_impl [private]
        template <template <typename> typename Pred>
        struct find_last_if_not_impl {

            /// ::binded_temp
            template <typename T>
            using binded_temp = self::find_if_not_impl<Pred>::template binded_temp<T>;

            /// ::result
            using result = self::find_last_if_impl<binded_temp>;

            /// ::value
            inline static constexpr int value = result::value;

            /// ::type
            using type = result::value;

        };

        /// ::find_last_impl [private]
        template <typename Query>
        struct find_last_impl {

            /// ::binded_temp
            template <typename T>
            using binded_temp = self::find_impl<Query>::template binded_temp<T>;

            /// ::result
            using result = self::find_last_if_impl<binded_temp>;

            /// ::value
            inline static constexpr int value = result::value;

            /// ::type
            using type = result::value;

        };

        /// ::flatten_helper_impl [private]
        // recursive approach
        template <typename, typename>
        struct flatten_helper_impl {};

        template <typename... Us, typename... Heads, typename... Tails>
        struct flatten_helper_impl<type_seq_t<Us...>, type_seq_t<type_seq_t<Heads...>, Tails...>> {
            using type = flatten_helper_impl<type_seq_t<Us...>, type_seq_t<Heads..., Tails...>>::type;
        };

        template <typename... Us, typename Head, typename... Tails>
        struct flatten_helper_impl<type_seq_t<Us...>, type_seq_t<Head, Tails...>> {
            using type = flatten_helper_impl<type_seq_t<Us..., Head>, type_seq_t<Tails...>>::type;
        };

        template <__type_seq::qualified_type_seq Seq>
        struct flatten_helper_impl<Seq, type_seq_t<>> {
            using type = Seq;
        };

        /// ::flatten_impl [private]
        template <typename... Us>
        struct flatten_impl {
            using type = self::flatten_helper_impl<type_seq_t<>, type_seq_t<Us...>>::type;
        };

        /// ::zip_list_at_impl [private]
        template <std::size_t I, typename... Seqs>
        using zip_last_at_impl = type_seq_t<typename Seqs::template at<I>...>;

        /// ::zip_impl [private]
        template <typename, typename...>
        struct zip_impl {};

        template <std::size_t... Is, typename... Seqs>
        struct zip_impl<gold::index_seq_t<Is...>, Seqs...> {

            /// ::at_impl
            template <std::size_t I>
            using at_impl = type_seq_t<typename Seqs::template at<I>...>;

            /// ::type
            using type = type_seq_t<at_impl<Is>...>;
        };

        /// join_impl [private]
        template <typename... Us>
        struct join_impl {
            using type = type_seq_t<>::template concat_with<Us...>;
        };

        /// join_with_impl [private]
        // Args... are inserted between sequences
        template <typename... Args>
        struct join_with_impl {

            /// ::temp_type
            using temp_type = type_seq_t<>
                            ::template concat_with<typename Ts::template append<Args...>...>;

            /// ::type
            using type = gold::conditional_t<sizeof...(Args) == 0,
                temp_type, typename temp_type::template pop_back<>
            >;
        };

        /// ::select_while_impl [private]
        // returns the select count for those who make the Pred true until false
        template <template <typename> typename Pred>
        static consteval std::size_t select_while_impl() {
            std::size_t pos = 0;
            bool ongoing    = true;
            self::for_each_impl([&]<typename T> {
                if constexpr (!Pred<T>::value)
                    ongoing = false;
                if (ongoing)
                    ++pos;
            });
            return pos;
        }

        /// ::take_impl [private]
        template <std::size_t N>
        static consteval auto take_impl() {
            if constexpr (N > self::seq_size)
                return __util::uneval<self>;
            else
                return __util::uneval<
                    typename self::extract_from_indices_impl<gold::make_consecutive_index_seq_t<0, N>>::type
                >;
        }

        /// ::take_last_impl [private]
        template <std::size_t N>
        static consteval auto take_last_impl() {
            if constexpr (N > self::seq_size)
                return __util::uneval<self>;
            else
                return __util::uneval<
                    typename self::extract_from_indices_impl<gold::make_consecutive_index_seq_t<self::seq_size - N, N>>::type
                >;
        }

        /// ::drop_impl [private]
        template <std::size_t N>
        static consteval auto drop_impl() {
            if constexpr (N > self::seq_size)
                return __util::uneval<type_seq_t<>>;
            else
                return __util::uneval<
                    typename self::take_last<self::seq_size - N>
                >;
        }

        /// ::chunk_impl [private]
        template <std::size_t N>
        static consteval auto chunk_impl() {
            if constexpr (N == 1)
                return __util::uneval<type_seq_t<type_seq_t<Ts>...>>;
            else
                return []<std::size_t... Is>(gold::index_seq_t<Is...>) {
                    return __util::uneval<
                        type_seq_t<typename type_seq_t<Ts...>::template drop<Is * N>::template take<N>...>
                    >;
                }(gold::make_index_seq<__type_seq::div_ceil(self::seq_size, N)>);
        }

        /// ::repeat_impl [private]
        template <std::size_t N>
        static consteval auto repeat_impl() {
            if constexpr (N == 0)
                return __util::uneval<type_seq_t<>>;
            else if constexpr (N == 1)
                return __util::uneval<self>;
            else
                return []<std::size_t... Is>(gold::index_seq_t<Is...>) {
                    return __util::uneval<
                        decltype((__util::uneval<self&&>() + ... + (Is, __util::uneval<self&&>())))
                    >;
                }(gold::make_index_seq<N - 1>);
        }

      public:

        /// ctor
        consteval type_seq_t() noexcept = default;

        /// ::unpack
        template <template <typename...> typename Temp>
            requires requires { typename Temp<Ts...>; }
        using unpack = Temp<Ts...>;

        /// ::unpack_q
        template <typename Temp>
        using unpack_q = self::unpack<Temp::template Fn>;

        /// ::concat_with
        template <__type_seq::qualified_type_seq... Seqs>
        using concat_with = decltype((__util::uneval<self&&>() + ... + __util::uneval<Seqs&&>()));

        /// ::append
        template <typename... Us>
        using append = self::concat_with<type_seq_t<Us...>>;

        /// ::prepend
        template <typename... Us>
        using prepend = type_seq_t<Us...>::template concat_with<self>;

        /// ::push_front
        template <typename U>
        using push_front = type_seq_t<U, Ts...>;

        /// ::push_back
        template <typename U>
        using push_back = type_seq_t<Ts..., U>;

        /// ::pop_front
        template <typename... Unused>
            requires (sizeof...(Unused) == 0)
        using pop_front = self::unpack<self::pop_front_impl>::type;

        /// ::pop_back
        template <typename... Unused>
            requires (sizeof...(Unused) == 0)
        using pop_back = self::unpack<self::pop_back_impl>::type;

        /// ::reverse
        template <typename... Unused>
            requires (sizeof...(Unused) == 0)
        using reverse = self::extract_from_indices_impl<gold::make_reverse_index_seq_t<self::seq_empty>>::type;

        /// ::take
        template <std::size_t N = 1>
        using take = decltype(self::take_impl<N>()());

        /// ::take_last
        template <std::size_t N = 1>
        using take_last = decltype(self::take_last_impl<N>()());

        /// ::drop
        template <std::size_t N = 1>
        using drop = decltype(self::drop_impl<N>()());

        /// ::take_while
        template <template <typename> typename Pred>
        using take_while = self::template take<self::template select_while_impl<Pred>()>;

        /// ::take_while_q
        template <typename Pred>
        using take_while_q = self::take_while<Pred::template Fn>;

        /// ::drop_while
        template <template <typename> typename Pred>
        using drop_while = self::template drop<self::template select_while_impl<Pred>()>;

        /// ::drop_while_q
        template <typename Pred>
        using drop_while_q = self::drop_while<Pred::template Fn>;

        /// ::slice
        template <std::size_t B, std::size_t E>
            requires (B < E)
        using slice = self::extract_from_indices_impl<gold::make_consecutive_index_seq_t<B, E - B>>::type;

        /// ::stride
        template <std::size_t S>
            requires (S != 0)
        using stride = self::extract_from_indices_impl<
            gold::make_consecutive_index_seq_t<0, (self::seq_size + S - 1) / S, S>
        >::type;

        /// ::shift_left
        template <std::size_t N = 1>
            requires (N <= self::seq_size)
        using shift_left = self::drop<N>;

        /// ::shift_right
        template <std::size_t N = 1>
            requires (N <= self::seq_size)
        using shift_right = self::take<self::seq_size - N>;

        /// ::rotate_left
        template <std::size_t N = 1>
        using rotate_left = self::drop<N % self::seq_size>
                                ::template concat_with<self::take<N % self::seq_size>>;

        /// ::rotate_right
        template <std::size_t N = 1>
        using rotate_right = self::take_last<N % self::seq_size>
                                 ::template concat_with<self::shift_right<N % self::seq_size>>;

        /// ::insert_at
        template <std::size_t N, typename... Us>
            requires (N <= self::seq_size)
        using insert_at = decltype([]{
            if constexpr (N == 0)
                return __util::uneval<self::prepend<Us...>>;
            else if constexpr (N == self::seq_size)
                return __util::uneval<self::append<Us...>>;
            else
                return __util::uneval<
                    typename self::template take<N>
                                 ::template append<Us...>
                                 ::template concat_with<self::template drop<N>>
                >;
        }()());

        /// ::erase_at
        // erase at specific index
        template <std::size_t I>
            requires (I < self::seq_size)
        using erase_at = self::extract_from_indices_impl<
            gold::concat_value_seq_t<
                gold::make_index_seq_t<I>,
                typename gold::make_index_seq_t<self::seq_size - I - 1>::template offset<I + 1>
            >
        >::type;

        /// ::erase_interval
        // erase at specific interval
        template <std::size_t B, std::size_t E>
            requires (B <= self::seq_size && E <= self::seq_size && B < E)
        using erase_interval = self::extract_from_indices_impl<
            gold::concat_value_seq_t<
                gold::make_index_seq_t<B>,
                typename gold::make_index_seq_t<self::seq_size - E>::template offset<E>
            >
        >::type;

        /// ::repeat
        template <std::size_t N>
        using repeat = decltype(self::repeat_impl<N>()());

        /// ::find
        template <typename Query>
        using find = self::find_impl<Query>::type;

        /// ::find_v
        template <typename Query>
        inline static constexpr int find_v = self::find_impl<Query>::value;

        /// ::find_if
        template <template <typename> typename Pred>
        using find_if = self::find_if_impl<Pred>::type;

        /// ::find_if_q
        template <typename Pred>
        using find_if_q = self::find_if<Pred::template Fn>;

        /// ::find_if_v
        template <template <typename> typename Pred>
        inline static constexpr int find_if_v = self::find_if_impl<Pred>::value;

        /// ::find_if_q_v
        template <typename Pred>
        inline static constexpr int find_if_q_v = self::find_if_v<Pred::template Fn>;

        /// ::find_if_not
        template <template <typename> typename Pred>
        using find_if_not = self::find_if_not_impl<Pred>::type;

        /// ::find_if_not_q
        template <typename Pred>
        using find_if_not_q = self::find_if_not<Pred::template Fn>;

        /// ::find_if_not_v
        template <template <typename> typename Pred>
        inline static constexpr int find_if_not_v = self::find_if_not_impl<Pred>::value;

        /// ::find_if_not_q_v
        template <typename Pred>
        inline static constexpr int find_if_not_q_v = self::find_if_not_v<Pred::template Fn>;

        /// ::find_last
        template <typename Query>
        using find_last = self::find_last_impl<Query>::type;

        /// ::find_last_v
        template <typename Query>
        inline static constexpr int find_last_v = self::find_last_impl<Query>::value;

        /// ::find_last_if
        template <template <typename> typename Pred>
        using find_last_if = self::find_last_if_impl<Pred>::type;

        /// ::find_last_if_q
        template <typename Pred>
        using find_last_if_q = self::find_last_if<Pred::template Fn>;

        /// ::find_last_if_v
        template <template <typename> typename Pred>
        inline static constexpr int find_last_if_v = self::find_last_if_impl<Pred>::value;

        /// ::find_last_if_q_v
        template <typename Pred>
        inline static constexpr int find_last_if_q_v = self::find_last_if_v<Pred::template Fn>;

        /// ::find_last_if_not
        template <template <typename> typename Pred>
        using find_last_if_not = self::find_last_if_not_impl<Pred>::type;

        /// ::find_last_if_not_q
        template <typename Pred>
        using find_last_if_not_q = self::find_last_if_not<Pred::template Fn>;

        /// ::find_last_if_not_v
        template <template <typename> typename Pred>
        inline static constexpr int find_last_if_not_v = self::find_last_if_not_impl<Pred>::value;

        /// ::find_last_if_not_q_v
        template <typename Pred>
        inline static constexpr int find_last_if_not_q_v = self::find_last_if_not_v<Pred::template Fn>;

        /// ::all_of
        template <template <typename> typename Pred>
        inline static constexpr bool all_of = (self::find_if_not<Pred>{} == gold::sentinel_token);

        /// ::all_of_q
        template <typename Pred>
        inline static constexpr bool all_of_q = self::all_of<Pred::template Fn>;

        /// ::none_of
        template <template <typename> typename Pred>
        inline static constexpr bool none_of = (self::find_if<Pred>{} == gold::sentinel_token);

        /// ::none_of_q
        template <typename Pred>
        inline static constexpr bool none_of_q = self::none_of<Pred::template Fn>;

        /// ::any_of
        template <template <typename> typename Pred>
        inline static constexpr bool any_of = (self::find_if<Pred>{} != gold::sentinel_token);

        /// ::any_of_q
        template <typename Pred>
        inline static constexpr bool any_of_q = self::any_of<Pred::template Fn>;

        /// ::contains_single
        template <typename Query>
        inline static constexpr bool contains_single = (self::find<Query>{} != gold::sentinel_token);

        /// ::contains
        template <__type_seq::qualified_type_seq Seq>
        inline static constexpr bool contains = [] -> bool {
            constexpr auto this_ids  = __type_seq::get_type_ids<self>();
            constexpr auto other_ids = __type_seq::get_type_ids<Seq>();
            return __type_seq::contains_subrange(
                {.iter = this_ids.begin(), .sent = this_ids.end()},
                {.iter = other_ids.begin(), .sent = other_ids.end()}
            );
        }();

        /// ::starts_with_single
        template <typename T>
        inline static constexpr bool starts_with_single = __is_same(T, typename self::template front<>);

        /// ::starts_with
        template <__type_seq::qualified_type_seq Seq>
        inline static constexpr bool starts_with = [] -> bool {
            if constexpr (Seq::seq_size > self::seq_size || Seq::seq_size == 0)
                return false;
            else
                return Seq::template equals<self::template take<Seq::seq_size>>;
        }();

        /// ::ends_with_single
        template <typename T>
        inline static constexpr bool ends_with_single = __is_same(T, typename self::template back<>);

        /// ::ends_with
        template <__type_seq::qualified_type_seq Seq>
        inline static constexpr bool ends_with = [] -> bool {
            if constexpr (Seq::seq_size > self::seq_size || Seq::seq_size == 0)
                return false;
            else
                return Seq::template equals<self::template take_last<Seq::seq_size>>;
        }();

        /// ::transform
        template <template <typename> typename Transform>
        using transform = type_seq_t<typename Transform<Ts>::type...>;

        /// ::transform_q
        template <typename Transform>
        using transform_q = self::transform<Transform::template Fn>;

        /// ::count
        template <typename Query>
        inline static constexpr std::size_t count = ((__is_same(Ts, Query) ? 1 : 0) + ... + 0);

        /// ::count_if
        template <template <typename> typename Pred>
        inline static constexpr std::size_t count_if = ((Pred<Ts>::value ? 1 : 0) + ... + 0);

        /// ::count_if_q
        template <typename Pred>
        inline static constexpr std::size_t count_if_q = self::count_if<Pred::template Fn>;

        /// ::filter
        template <template <typename> typename Pred>
        using filter = type_seq_t<>::template concat_with<
            gold::conditional_t<Pred<Ts>::value, type_seq_t<Ts>, type_seq_t<>> ...
        >;

        /// ::filter_q
        template <typename Pred>
        using filter_q = self::filter<Pred::template Fn>;

        /// ::remove_if
        template <template <typename> typename Pred>
        using remove_if = type_seq_t<>::template concat_with<
            gold::conditional_t<Pred<Ts>::value, type_seq_t<>, type_seq_t<Ts>> ...
        >;

        /// ::remove_if_q
        template <typename Pred>
        using remove_if_q = self::remove_if<Pred::template Fn>;

        /// ::remove
        template <typename Query>
        using remove = type_seq_t<>::template concat_with<gold::conditional_t<
            __is_same(Ts, Query), type_seq_t<>, type_seq_t<Ts>
        >...>;

        /// ::replace_if
        template <template <typename> typename Pred, typename New>
        using replace_if = type_seq_t<
            gold::conditional_t<Pred<Ts>::value, New, Ts>...
        >;

        /// ::replace_if_q
        template <typename Pred, typename New>
        using replace_if_q = self::replace_if<Pred::template Fn, New>;

        /// ::replace
        template <typename Old, typename New>
        using replace = type_seq_t<gold::conditional_t<
            __is_same(Old, Ts), New, Ts
        >...>;

        /// ::fill
        template <typename T>
        using fill = type_seq_t<gold::conditional_t<true, T, Ts>...>;

        /// ::fill_first
        template <typename T, std::size_t N>
        using fill_first = self::template take<N>
                               ::template fill<T>
                               ::template concat_with<self::take_last<self::seq_size - N>>;

        /// ::mismatch
        template <__type_seq::qualified_type_seq Other>
        using mismatch = decltype([]{
            static constexpr auto this_ids  = __type_seq::get_type_ids<self>();
            static constexpr auto that_ids  = __type_seq::get_type_ids<Other>();
            static constexpr auto result    = __type_seq::mismatch_seq(
                { .iter = this_ids.begin(), .sent = this_ids.end() },
                { .iter = that_ids.begin(), .sent = that_ids.end() }
            );
            if constexpr (result.n1 == self::seq_size || result.n2 == Other::seq_size)
                return __util::uneval<type_seq_t<>>;
            else
                return __util::uneval<
                    type_seq_t<typename self::at<result.n1>,
                               typename Other::at<result.n2>>
                >;
        }()());

        /// ::split_when [TODO] [2nd batch]
        /// ::split [TODO] [2nd batch]

        /// ::join_with
        template <typename... Args>
            requires (__type_seq::qualified_type_seq<Ts> && ...)
        using join_with = self::join_with_impl<Args...>::type;

        /// ::flatten
        template <typename... Unused>
            requires (sizeof...(Unused) == 0)
        using flatten = self::unpack<self::flatten_impl>::type;

        /// ::zip_with
        template <__type_seq::qualified_type_seq... Seqs>
        using zip_with = self::zip_impl<
            gold::make_index_seq_t<__algo::min_element({self::seq_size, Seqs::seq_size...})>,
            self, Seqs...
        >::type;

        /// ::unzip [TODO] [2nd batch]

        /// ::chunk [TODO]
        template <std::size_t N>
            requires (N > 0)
        using chunk = decltype(self::chunk_impl<N>()());

        /// ::equals
        template <__type_seq::qualified_type_seq Other>
        inline static constexpr bool equals = __is_same(self, Other);

        /// ::unique [TODO] [2nd batch]

        /// ::operator+
        template <typename... Us>
        friend consteval auto operator+(type_seq_t, type_seq_t<Us...>) {
            return type_seq_t<Ts..., Us...>{};
        }

        /// ::operator==
        friend consteval bool operator==(type_seq_t, type_seq_t) { return true; }

        template <typename... Us>
        friend consteval bool operator==(type_seq_t, type_seq_t<Us...>) {
            return false;
        }
    };

    /// type_seq
    template <typename... Ts>
    inline constexpr type_seq_t<Ts...> type_seq {};

    /// concat_type_seq_t
    template <__type_seq::qualified_type_seq... Seqs>
    using concat_type_seq_t = empty_type_seq_t::template concat_with<Seqs...>;

    /// concat_type_seq
    template <__type_seq::qualified_type_seq... Seqs>
    inline constexpr concat_type_seq_t<Seqs...> concat_type_seq {};

    /// zip_type_seq_t
    template <__type_seq::qualified_type_seq... Seqs>
    using zip_type_seq_t = type_seq_t<Seqs...>
                         ::template take<1>
                         ::template zip_with<type_seq_t<Seqs...>::template drop<1>>;

    /// zip_type_seq
    template <__type_seq::qualified_type_seq... Seqs>
    inline constexpr zip_type_seq_t<Seqs...> zip_type_seq {};

} // namespace gold

#endif // __GOLD_BITS_SEQUENCE_TYPE_SEQ_HPP
