// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once
#include "kf/core/config.hpp"

#if kf_port_has_array
#include <array>

namespace kf {
template<typename T, size_t N> using Array = std::array<T, N>;
}

#else

#include "stddef.h"// NOLINT(*-deprecated-headers)

#include "kf/core/attributes.hpp"
#include "kf/core/type_traits.hpp"

namespace kf {

// Реализация array для AVR
template<typename T, size_t N> struct Array {
    static_assert(N > 0, "Array size (N) must be >= 0");

    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = pointer;            // simplified
    using const_reverse_iterator = const_pointer;// simplified

private:
    value_type data_[N];

public:
    reference at(size_type pos) {
        // AVR does not support exceptions
        return data_[pos];
    }

    const_reference at(size_type pos) const {
        return data_[pos];
    }

    reference operator[](size_type pos) {
        return data_[pos];
    }

    const_reference operator[](size_type pos) const {
        return data_[pos];
    }

    reference front() {
        return data_[0];
    }

    const_reference front() const {
        return data_[0];
    }

    reference back() {
        return data_[N - 1];
    }

    const_reference back() const {
        return data_[N - 1];
    }

    pointer data() noexcept {
        return data_;
    }

    const_pointer data() const noexcept {
        return data_;
    }

    iterator begin() noexcept {
        return data_;
    }

    const_iterator begin() const noexcept {
        return data_;
    }

    const_iterator cbegin() const noexcept {
        return data_;
    }

    iterator end() noexcept {
        return data_ + N;
    }

    const_iterator end() const noexcept {
        return data_ + N;
    }

    const_iterator cend() const noexcept {
        return data_ + N;
    }

    // Упрощенные reverse iterators для AVR
    iterator rbegin() noexcept {
        return data_ + N - 1;
    }

    const_iterator rbegin() const noexcept {
        return data_ + N - 1;
    }

    iterator rend() noexcept {
        return data_ - 1;
    }

    const_iterator rend() const noexcept {
        return data_ - 1;
    }

    kf_nodiscard constexpr bool empty() const noexcept {
        return N == 0;
    }

    kf_nodiscard constexpr size_type size() const noexcept {
        return N;
    }

    kf_nodiscard constexpr size_type max_size() const noexcept {
        return N;
    }

    void fill(const T &value) {
        for (size_type i = 0; i < N; ++i) {
            data_[i] = value;
        }
    }

    void swap(Array &other) {
        for (size_type i = 0; i < N; ++i) {
            swap(data_[i], other.data_[i]);
        }
    }

    // Операторы сравнения
    bool operator==(const Array &other) const {
        for (size_type i = 0; i < N; ++i) {
            if (data_[i] == other.data_[i]) continue;
            return false;
        }
        return true;
    }

    bool operator!=(const Array &other) const {
        return !(*this == other);// NOLINT(*-simplify)
    }

    bool operator<(const Array &other) const {
        for (size_type i = 0; i < N; ++i) {
            if (data_[i] < other.data_[i]) return true;
            if (other.data_[i] < data_[i]) return false;
        }
        return false;
    }

    bool operator<=(const Array &other) const {
        return !(other < *this);// NOLINT(*-simplify)
    }

    bool operator>(const Array &other) const {
        return other < *this;
    }

    bool operator>=(const Array &other) const {
        return !(*this < other);// NOLINT(*-simplify)
    }
};

}// namespace kf

#endif