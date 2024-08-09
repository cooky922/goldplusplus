// <gold/bits/numbers/fp_parts.hpp> - gold++ library

// Copyright (C) [ 2021 - 2023 ] - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_NUMBERS_FP_PARTS_HPP
#define __GOLD_BITS_NUMBERS_FP_PARTS_HPP

#include <type_traits>
#include <gold/basic_types>

namespace gold::__num {

    /// __num::fp_parts_base
    template <typename T>
    struct fp_parts_base {
        bool sign;
        T exponent; // exponent is unbiased by default
        T mantissa;
    };

    /// __num::fp_parts
    template <typename>
    struct fp_parts {};

    /// __num::fp_parts<float>
    template <>
    struct fp_parts<float> : fp_parts_base<gold::uint32_t> {
      private:
        using base_type = fp_parts_base<gold::uint32_t>;
        using repr_type = gold::uint32_t;

      public:
        using storage_type = gold::uint32_t;
        using representation_type = gold::uint32_t;
        inline static constexpr auto mantissa_bits = 23uz;
        inline static constexpr auto exponent_bits = 8uz;
        inline static constexpr auto exponent_bias = 127uz; // 2 ^ (exp-bits - 1) - 1
        // max_precision in base 10 form: round(log10(2 ^ bits) + 1)
        inline static constexpr auto max_precision = 8uz;

        static constexpr fp_parts from_parts(base_type op) noexcept {
            return fp_parts { op };
        }

        static constexpr fp_parts from_parts(bool sign, repr_type exponent, repr_type mantissa) noexcept {
            return from_parts(base_type{sign, exponent, mantissa});
        }

        static constexpr fp_parts from_float(float op) noexcept {
            repr_type bits = __builtin_bit_cast(repr_type, op);
            return fp_parts {
                base_type {
                    .sign     = bool(bits & (1u << 31)),
                    .exponent = (bits & (0xFFu << 23)) >> 23,
                    .mantissa = bits & 0x7FFFFFu
                }
            };
        }

        constexpr float to_float() const noexcept {
            repr_type bits = (repr_type(this->sign) << 31)
                           | (this->exponent << 23)
                           | (this->mantissa);
            return __builtin_bit_cast(float, bits);
        }

        constexpr std::make_signed_t<repr_type> biased_exponent() const noexcept {
            return this->exponent - exponent_bias;
        }

        constexpr void biased_exponent(std::make_signed_t<repr_type> op) noexcept {
            this->exponent = repr_type(exponent_bias + op);
        }
    };

    /// __num::fp_parts<double>
    template <>
    struct fp_parts<double> : fp_parts_base<gold::uint64_t> {
      private:
        using base_type = fp_parts_base<gold::uint64_t>;
        using repr_type = gold::uint64_t;

      public:
        using storage_type = gold::uint64_t;
        using representation_type = gold::uint64_t;
        inline static constexpr auto mantissa_bits = 52uz;
        inline static constexpr auto exponent_bits = 11uz;
        inline static constexpr auto exponent_bias = 1023uz; // 2 ^ (exp-bits - 1) - 1
        inline static constexpr auto max_precision = 17uz;

        static constexpr fp_parts from_parts(base_type op) noexcept {
            return fp_parts { op };
        }

        static constexpr fp_parts from_parts(bool sign, repr_type exponent, repr_type mantissa) noexcept {
            return from_parts(base_type{sign, exponent, mantissa});
        }

        static constexpr fp_parts from_float(double op) noexcept {
            repr_type bits = __builtin_bit_cast(repr_type, op);
            return fp_parts {
                base_type {
                    .sign     = bool(bits & (1ull << 63)),
                    .exponent = (bits & (0x7FFull << 52)) >> 52,
                    .mantissa = bits & 0xFFFFFFFFFFFFFull
                }
            };
        }

        constexpr double to_float() const noexcept {
            repr_type bits = (repr_type(this->sign) << 63)
                           | (this->exponent << 52)
                           | (this->mantissa);
            return __builtin_bit_cast(double, bits);
        }

        constexpr std::make_signed_t<repr_type> biased_exponent() const noexcept {
            return this->exponent - exponent_bias;
        }

        constexpr void biased_exponent(std::make_signed_t<repr_type> op) noexcept {
            this->exponent = repr_type(exponent_bias + op);
        }
    };

    /// __num::fp_parts<long double>
    template <>
    struct fp_parts<long double> : fp_parts_base<gold::uint64_t> {
      private:
        using base_type = fp_parts_base<gold::uint64_t>;
        using repr_type = gold::uint64_t;

      public:
        using storage_type = gold::uint128_t;
        using representation_type = gold::uint64_t;
        inline static constexpr auto mantissa_bits = 64uz;
        inline static constexpr auto exponent_bits = 15uz;
        inline static constexpr auto exponent_bias = 16383uz; // 2 ^ (exp-bits - 1) - 1
        inline static constexpr auto max_precision = 21uz;

        static constexpr fp_parts from_parts(base_type op) noexcept {
            return fp_parts { op };
        }

        static constexpr fp_parts from_parts(bool sign, repr_type exponent, repr_type mantissa) noexcept {
            return from_parts(base_type{sign, exponent, mantissa});
        }

        static constexpr fp_parts from_float(long double op) noexcept {
            gold::uint128_t bits = __builtin_bit_cast(gold::uint128_t, op);
            return fp_parts {
                base_type {
                    .sign     = bool(bits & (gold::uint128_t(1ull) << 79)),
                    .exponent = repr_type((bits & (gold::uint128_t(0x7FFFull) << 64)) >> 64),
                    .mantissa = repr_type(bits & 0xFFFFFFFFFFFFFFFFull)
                }
            };
        }

        constexpr long double to_float() const noexcept {
            gold::uint128_t bits = (gold::uint128_t(this->sign) << 79)
                                 | (gold::uint128_t(this->exponent) << 64)
                                 | (gold::uint128_t(this->mantissa));
            return __builtin_bit_cast(long double, bits);
        }

        constexpr std::make_signed_t<repr_type> biased_exponent() const noexcept {
            return this->exponent - exponent_bias;
        }

        constexpr void biased_exponent(std::make_signed_t<repr_type> op) noexcept {
            this->exponent = repr_type(exponent_bias + op);
        }
    };

    /// __num::fp_parts<std::float16_t>
    template <>
    struct fp_parts<gold::float16x_t> : fp_parts_base<gold::uint16_t> {
      private:
        using base_type = fp_parts_base<gold::uint16_t>;
        using repr_type = gold::uint16_t;

      public:
        using storage_type = gold::uint16_t;
        using representation_type = gold::uint16_t;
        inline static constexpr auto mantissa_bits = 10uz;
        inline static constexpr auto exponent_bits = 5uz;
        inline static constexpr auto exponent_bias = 15uz; // 2 ^ (exp-bits - 1) - 1
        inline static constexpr auto max_precision = 4uz;

        static constexpr fp_parts from_parts(base_type op) noexcept {
            return fp_parts { op };
        }

        static constexpr fp_parts from_parts(bool sign, repr_type exponent, repr_type mantissa) noexcept {
            return from_parts(base_type{sign, exponent, mantissa});
        }

        static constexpr fp_parts from_float(gold::float16x_t op) noexcept {
            repr_type bits = __builtin_bit_cast(repr_type, op);
            return fp_parts {
                base_type {
                    .sign     = bool(bits & (1u << 15)),
                    .exponent = repr_type((bits & (0x1Fu << 10)) >> 10),
                    .mantissa = repr_type(bits & 0x3FFu)
                }
            };
        }

        constexpr gold::float16x_t to_float() const noexcept {
            repr_type bits = (repr_type(this->sign) << 15)
                           | (this->exponent << 10)
                           | (this->mantissa);
            return __builtin_bit_cast(gold::float16x_t, bits);
        }

        constexpr std::make_signed_t<repr_type> biased_exponent() const noexcept {
            return this->exponent - exponent_bias;
        }

        constexpr void biased_exponent(std::make_signed_t<repr_type> op) noexcept {
            this->exponent = repr_type(exponent_bias + op);
        }
    };

    /// __num::fp_parts<std::float32_t>
    template <>
    struct fp_parts<gold::float32x_t> : fp_parts_base<gold::uint32_t> {
      private:
        using base_type = fp_parts_base<gold::uint32_t>;
        using repr_type = gold::uint32_t;

      public:
        using storage_type = gold::uint32_t;
        using representation_type = gold::uint32_t;
        inline static constexpr auto mantissa_bits = 23uz;
        inline static constexpr auto exponent_bits = 8uz;
        inline static constexpr auto exponent_bias = 127uz; // 2 ^ (exp-bits - 1) - 1
        inline static constexpr auto max_precision = 8uz;

        static constexpr fp_parts from_parts(base_type op) noexcept {
            return fp_parts { op };
        }

        static constexpr fp_parts from_parts(bool sign, repr_type exponent, repr_type mantissa) noexcept {
            return from_parts(base_type{sign, exponent, mantissa});
        }

        static constexpr fp_parts from_float(gold::float32x_t op) noexcept {
            repr_type bits = __builtin_bit_cast(repr_type, op);
            return fp_parts {
                base_type {
                    .sign     = bool(bits & (1u << 31)),
                    .exponent = (bits & (0xFFu << 23)) >> 23,
                    .mantissa = bits & 0x7FFFFFu
                }
            };
        }

        constexpr gold::float32x_t to_float() const noexcept {
            repr_type bits = (repr_type(this->sign) << 31)
                           | (this->exponent << 23)
                           | (this->mantissa);
            return __builtin_bit_cast(gold::float32x_t, bits);
        }

        constexpr std::make_signed_t<repr_type> biased_exponent() const noexcept {
            return this->exponent - exponent_bias;
        }

        constexpr void biased_exponent(std::make_signed_t<repr_type> op) noexcept {
            this->exponent = repr_type(exponent_bias + op);
        }
    };

    /// __num::fp_parts<std::float64_t>
    template <>
    struct fp_parts<gold::float64x_t> : fp_parts_base<gold::uint64_t> {
      private:
        using base_type = fp_parts_base<gold::uint64_t>;
        using repr_type = gold::uint64_t;

      public:
        using storage_type = gold::uint64_t;
        using representation_type = gold::uint64_t;
        inline static constexpr auto mantissa_bits = 52uz;
        inline static constexpr auto exponent_bits = 11uz;
        inline static constexpr auto exponent_bias = 1023uz; // 2 ^ (exp-bits - 1) - 1
        inline static constexpr auto max_precision = 17uz;

        static constexpr fp_parts from_parts(base_type op) noexcept {
            return fp_parts { op };
        }

        static constexpr fp_parts from_parts(bool sign, repr_type exponent, repr_type mantissa) noexcept {
            return from_parts(base_type{sign, exponent, mantissa});
        }

        static constexpr fp_parts from_float(gold::float64x_t op) noexcept {
            repr_type bits = __builtin_bit_cast(repr_type, op);
            return fp_parts {
                base_type {
                    .sign     = bool(bits & (1ull << 63)),
                    .exponent = (bits & (0x7FFull << 52)) >> 52,
                    .mantissa = bits & 0xFFFFFFFFFFFFFull
                }
            };
        }

        constexpr gold::float64x_t to_float() const noexcept {
            repr_type bits = (repr_type(this->sign) << 63)
                           | (this->exponent << 52)
                           | (this->mantissa);
            return __builtin_bit_cast(gold::float64x_t, bits);
        }

        constexpr std::make_signed_t<repr_type> biased_exponent() const noexcept {
            return this->exponent - exponent_bias;
        }

        constexpr void biased_exponent(std::make_signed_t<repr_type> op) noexcept {
            this->exponent = repr_type(exponent_bias + op);
        }
    };

    /// __num::fp_parts<__float128>
    template <>
    struct fp_parts<gold::float128_t> : fp_parts_base<gold::uint128_t> {
      private:
        using base_type = fp_parts_base<gold::uint128_t>;
        using repr_type = gold::uint128_t;

      public:
        using storage_type = gold::uint128_t;
        using representation_type = gold::uint128_t;
        inline static constexpr auto mantissa_bits = 112uz;
        inline static constexpr auto exponent_bits = 15uz;
        inline static constexpr auto exponent_bias = 16383uz; // 2 ^ (exp-bits - 1) - 1
        inline static constexpr auto max_precision = 35uz;

        static constexpr fp_parts from_parts(base_type op) noexcept {
            return fp_parts { op };
        }

        static constexpr fp_parts from_parts(bool sign, repr_type exponent, repr_type mantissa) noexcept {
            return from_parts(base_type{sign, exponent, mantissa});
        }

        static constexpr fp_parts from_float(gold::float128_t op) noexcept {
            repr_type bits = __builtin_bit_cast(repr_type, op);
            return fp_parts {
                base_type {
                    .sign     = bool(bits & (repr_type(1ull) << 127)),
                    .exponent = (bits & (repr_type(0x7FFFull) << 112)) >> 112,
                    .mantissa = bits & ((gold::uint128_t(1) << 112) - 1)
                }
            };
        }

        constexpr gold::float128_t to_float() const noexcept {
            repr_type bits = (repr_type(this->sign) << 127)
                           | (this->exponent << 112)
                           | (this->mantissa);
            return __builtin_bit_cast(gold::float128_t, bits);
        }

        constexpr std::make_signed_t<repr_type> biased_exponent() const noexcept {
            return this->exponent - exponent_bias;
        }

        constexpr void biased_exponent(std::make_signed_t<repr_type> op) noexcept {
            this->exponent = repr_type(exponent_bias + op);
        }
    };

    /// __num::fp_parts<std::float128_t>
    template <>
    struct fp_parts<gold::float128x_t> : fp_parts_base<gold::uint128_t> {
      private:
        using base_type = fp_parts_base<gold::uint128_t>;
        using repr_type = gold::uint128_t;

      public:
        using storage_type = gold::uint128_t;
        using representation_type = gold::uint128_t;
        inline static constexpr auto mantissa_bits = 112uz;
        inline static constexpr auto exponent_bits = 15uz;
        inline static constexpr auto exponent_bias = 16383uz; // 2 ^ (exp-bits - 1) - 1
        inline static constexpr auto max_precision = 35uz;

        static constexpr fp_parts from_parts(base_type op) noexcept {
            return fp_parts { op };
        }

        static constexpr fp_parts from_parts(bool sign, repr_type exponent, repr_type mantissa) noexcept {
            return from_parts(base_type{sign, exponent, mantissa});
        }

        static constexpr fp_parts from_float(gold::float128x_t op) noexcept {
            repr_type bits = __builtin_bit_cast(repr_type, op);
            return fp_parts {
                base_type {
                    .sign     = bool(bits & (repr_type(1ull) << 127)),
                    .exponent = (bits & (repr_type(0x7FFFull) << 112)) >> 112,
                    .mantissa = bits & ((gold::uint128_t(1) << 112) - 1)
                }
            };
        }

        constexpr gold::float128x_t to_float() const noexcept {
            repr_type bits = (repr_type(this->sign) << 127)
                           | (this->exponent << 112)
                           | (this->mantissa);
            return __builtin_bit_cast(gold::float128x_t, bits);
        }

        constexpr std::make_signed_t<repr_type> biased_exponent() const noexcept {
            return this->exponent - exponent_bias;
        }

        constexpr void biased_exponent(std::make_signed_t<repr_type> op) noexcept {
            this->exponent = repr_type(exponent_bias + op);
        }
    };

    /// __num::fp_parts<std::bfloat16_t>
    template <>
    struct fp_parts<gold::bfloat16x_t> : fp_parts_base<gold::uint8_t> {
      private:
        using base_type = fp_parts_base<gold::uint8_t>;
        using repr_type = gold::uint8_t;

      public:
        using storage_type = gold::uint16_t;
        using representation_type = gold::uint8_t;
        inline static constexpr auto mantissa_bits = 7uz;
        inline static constexpr auto exponent_bits = 8uz;
        inline static constexpr auto exponent_bias = 127uz; // 2 ^ (exp-bits - 1) - 1
        inline static constexpr auto max_precision = 3uz;

        static constexpr fp_parts from_parts(base_type op) noexcept {
            return fp_parts { op };
        }

        static constexpr fp_parts from_parts(bool sign, repr_type exponent, repr_type mantissa) noexcept {
            return from_parts(base_type{sign, exponent, mantissa});
        }

        static constexpr fp_parts from_float(gold::bfloat16x_t op) noexcept {
            gold::uint16_t bits = __builtin_bit_cast(gold::uint16_t, op);
            return fp_parts {
                base_type {
                    .sign     = bool(bits & (1u << 15)),
                    .exponent = repr_type((bits & (0xFFu << 7)) >> 7),
                    .mantissa = repr_type(bits & 0x7Fu)
                }
            };
        }

        constexpr gold::bfloat16x_t to_float() const noexcept {
            gold::uint16_t bits = (gold::uint16_t(this->sign) << 15)
                                | (gold::uint16_t(this->exponent) << 7)
                                | (gold::uint16_t(this->mantissa));
            return __builtin_bit_cast(gold::bfloat16x_t, bits);
        }

        constexpr std::make_signed_t<repr_type> biased_exponent() const noexcept {
            return this->exponent - exponent_bias;
        }

        constexpr void biased_exponent(std::make_signed_t<repr_type> op) noexcept {
            this->exponent = repr_type(exponent_bias + op);
        }
    };

} // namespace gold::__num

#endif // __GOLD_BITS_NUMBERS_FP_PARTS_HPP
