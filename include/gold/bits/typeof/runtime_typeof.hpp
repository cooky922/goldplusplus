// <gold/bits/typeof/runtime_typeof.hpp> - gold++ library

// Copyright (C) 2021 - present Desmond Gold

// note: internal header

#pragma once
#ifndef __GOLD_BITS_RUNTIME_TYPEOF_HPP
#define __GOLD_BITS_RUNTIME_TYPEOF_HPP

#include <string>
#include <cstdlib>
#include <typeinfo>
#include <memory>
#include <cxxabi.h>

namespace gold {

    namespace __detail {

        std::string demangle_(const char* name_) {
            std::size_t len_ { 0 };
            int status_ { 0 };

            std::unique_ptr<char, decltype(&std::free)> ptr_ (
                __cxxabiv1::__cxa_demangle(
                    name_, nullptr, &len_, &status_
                ), &std::free
            );

            return ptr_.get();
        }

        /// type_of_
        template <typename T>
        std::string type_of_(const T& arg_) {
            return __detail::demangle_(typeid(arg_).name());
        }

        template <typename T>
        std::string type_of_() {
            return __detail::demangle_(typeid(T).name());
        }


    } // namespace __detail

    /// demangle
    std::string demangle(const std::string& mangled_name_) {
        int status_;
        std::unique_ptr<char, decltype(&std::free)> ptr_ (
            __cxxabiv1::__cxa_demangle(
                mangled_name_.c_str(), nullptr, nullptr, &status_
            ), &std::free
        );
        if (status_ != 0) {
            return mangled_name_;
        } else {
            return ptr_.get();
        }
    }

} // namespace gold

#endif // __GOLD_BITS_RUNTIME_TYPEOF_HPP
