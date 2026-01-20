// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include "kf/core/aliases.hpp"
#include "kf/core/attributes.hpp"

namespace kf {

/// @brief Низкочастотный фильтры
/// @tparam T Скаляр
template<typename T> struct LowFrequencyFilter {

private:
    const f32 alpha;
    const f32 one_minus_alpha{1.0f - alpha};
    T filtered{};
    bool first_step{false};

public:
    explicit LowFrequencyFilter(f32 alpha) noexcept :
        alpha{alpha} {}

    kf_nodiscard const T &calc(const T &x) noexcept {
        if (first_step) {
            first_step = false;
            filtered = x;
            return filtered;
        }

        if (alpha == 1.0) {
            filtered = x;
            return filtered;
        }

        filtered = filtered * one_minus_alpha + x * alpha;
        return filtered;
    }

    /// @brief Сбросить значение фильтра
    void reset() {
        first_step = true;
    }
};

}// namespace kf
