// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include <cmath>

#include "kf/Option.hpp"
#include "kf/core/aliases.hpp"
#include "kf/core/attributes.hpp"

namespace kf {

/// Vector 2D
template<typename T> struct vec2 final {

    /// Scalar type
    using Scalar = T;

    T x, y;

    /// Default constructor
    vec2() :
        x{0}, y{0} {}

    /// Constructor with x and y
    vec2(T x, T y) :
        x{x}, y{y} {}

    /// Addition operator
    kf_nodiscard vec2 operator+(const vec2 &other) const noexcept {
        return {x + other.x, y + other.y};
    }

    /// Subtraction operator
    kf_nodiscard vec2 operator-(const vec2 &other) const noexcept {
        return {x - other.x, y - other.y};
    }

    /// Scalar multiplication operator
    kf_nodiscard vec2 operator*(T scalar) const noexcept {
        return {x * scalar, y * scalar};
    }

    /// Safe Scalar division operator with option
    kf_nodiscard Option<vec2> divChecked(T scalar) const noexcept {
        if (scalar == 0) {
            return {};
        }

        return {vec2{x / scalar, y / scalar}};
    }

    /// Scalar division operator
    kf_nodiscard vec2 operator/(T scalar) const noexcept {
        return vec2{x / scalar, y / scalar};
    }

    /// Addition assignment operator
    vec2 &operator+=(const vec2 &other) noexcept {
        x += other.x;
        y += other.y;
        return *this;
    }

    /// Subtraction assignment operator
    vec2 &operator-=(const vec2 &other) noexcept {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    /// Length of the vector
    kf_nodiscard T length() const noexcept {
        return std::hypot(x, y);
    }

    /// Normalized vector with option
    kf_nodiscard Option<vec2> normalized() const noexcept {
        const T len = length();

        if (len == 0) {
            return {};
        }

        return {vec2{x / len, y / len}};
    }

    /// Dot product with another vector
    kf_nodiscard T dot(const vec2 &other) const noexcept {
        return x * other.x + y * other.y;
    }

    /// Check if vector is zero
    kf_nodiscard inline bool isZero() const noexcept {
        return x == 0 and y == 0;
    }
};

// Type aliases for common types

using vec2f = vec2<f32>;/// Float vector type
using vec2i = vec2<i32>;/// Integer vector type

}// namespace kf
