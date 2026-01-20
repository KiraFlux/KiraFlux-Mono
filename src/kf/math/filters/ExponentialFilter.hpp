// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include "kf/core/aliases.hpp"
#include "kf/core/attributes.hpp"

namespace kf {

/// Экспоненциальный фильтр
template<typename T> struct ExponentialFilter {
    f32 k;
    T filtered;

    constexpr explicit ExponentialFilter(f32 k, T init_value = T{}) :
        k{k}, filtered{init_value} {}

    kf_nodiscard const T &calc(const T &value) {
        filtered += (value - filtered) * k;
        return filtered;
    }
};

}// namespace kf
