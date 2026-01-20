// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once
#include "kf/core/config.hpp"

#if kf_port_has_functional
#include <functional>

namespace kf {
template<typename F> using Function = std::function<F>;
}

#else

#include "stdlib.h"// NOLINT(*-deprecated-headers)

#include "kf/core/utility.hpp"

namespace kf {
template<typename T> class Fn;

template<typename R, typename... Args> class Function<R(Args...)> {
private:
    using func_ptr_t = R (*)(Args...);
    func_ptr_t func_ptr = nullptr;

public:
    Function() noexcept = default;

    Function(nullptr_t) noexcept {}// NOLINT(*-explicit-constructor)

    Function(func_ptr_t f) noexcept :// NOLINT(*-explicit-constructor)
        func_ptr(f) {}

    template<typename F> Function(F f) noexcept :// NOLINT(*-explicit-constructor)
        func_ptr(static_cast<func_ptr_t>(f)) {}

    Function(const Function &other) noexcept = default;

    Function(Function &&other) noexcept = default;

    Function &operator=(nullptr_t) noexcept {
        func_ptr = nullptr;
        return *this;
    }

    Function &operator=(func_ptr_t f) noexcept {
        func_ptr = f;
        return *this;
    }

    Function &operator=(const Function &other) noexcept = default;

    Function &operator=(Function &&other) noexcept = default;

    template<typename F> Function &operator=(F f) noexcept {
        func_ptr = static_cast<func_ptr_t>(f);
        return *this;
    }

    R operator()(Args... args) const {
        if (func_ptr) {
            return func_ptr(args...);
        } else {
            return R();
        }
    }

    explicit operator bool() const noexcept {
        return func_ptr != nullptr;
    }

    bool operator==(nullptr_t) const noexcept {
        return func_ptr == nullptr;
    }

    bool operator!=(nullptr_t) const noexcept {
        return func_ptr != nullptr;
    }

    bool operator==(func_ptr_t f) const noexcept {
        return func_ptr == f;
    }

    bool operator!=(func_ptr_t f) const noexcept {
        return func_ptr != f;
    }
};

}// namespace kf

#endif