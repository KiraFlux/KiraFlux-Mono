// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include <cmath>

#include "kf/Option.hpp"
#include "kf/core/aliases.hpp"
#include "kf/core/attributes.hpp"

namespace kf {

/// Vector 3D
template<typename T> struct vec3 final {

    /// Scalar type
    using Scalar = T;

    T x, y, z;

    /// Default constructor
    vec3() :
        x{0}, y{0}, z{0} {}

    /// Constructor with x, y, z
    vec3(T x, T y, T z) :
        x{x}, y{y}, z{z} {}

    /// Addition operator
    kf_nodiscard vec3 operator+(const vec3 &other) const noexcept {
        return {x + other.x, y + other.y, z + other.z};
    }

    /// Subtraction operator
    kf_nodiscard vec3 operator-(const vec3 &other) const noexcept {
        return {x - other.x, y - other.y, z - other.z};
    }

    /// Scalar multiplication operator
    kf_nodiscard vec3 operator*(T scalar) const noexcept {
        return {x * scalar, y * scalar, z * scalar};
    }

    /// Safe Scalar division operator with option
    kf_nodiscard Option<vec3> divChecked(T scalar) const noexcept {
        if (scalar == 0) {
            return {};
        }

        return {vec3{x / scalar, y / scalar, z / scalar}};
    }

    /// Scalar division operator
    kf_nodiscard vec3 operator/(T scalar) const noexcept {
        return vec3{x / scalar, y / scalar, z / scalar};
    }

    /// Addition assignment operator
    vec3 &operator+=(const vec3 &other) noexcept {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    /// Subtraction assignment operator
    vec3 &operator-=(const vec3 &other) noexcept {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    /// Length of the vector
    kf_nodiscard T length() const noexcept {
        return std::sqrt(x * x + y * y + z * z);
    }

    /// Normalized vector with option
    kf_nodiscard Option<vec3> normalized() const noexcept {
        const T len = length();

        if (len == 0) {
            return {};
        }

        return {vec3{x / len, y / len, z / len}};
    }

    /// Dot product with another vector
    kf_nodiscard T dot(const vec3 &other) const noexcept {
        return x * other.x + y * other.y + z * other.z;
    }

    /// Cross product with another vector
    kf_nodiscard vec3 cross(const vec3 &other) const noexcept {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x};
    }

    /// Check if vector is zero
    kf_nodiscard inline bool isZero() const noexcept {
        return x == 0 and y == 0 and z == 0;
    }
};

// Type aliases for common types

using vec3f = vec3<f32>;/// Float vector type
using vec3i = vec3<i32>;/// Integer vector type

}// namespace kf
