// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include "kf/core/attributes.hpp"
#include "kf/math/units.hpp"

namespace kf {

template<typename T> struct ComplementaryFilter {

private:
    const f32 alpha;
    const f32 one_minus_alpha{1.0f - alpha};
    T filtered{};
    bool first_step{true};

public:
    explicit ComplementaryFilter(f32 alpha) :
        alpha{alpha} {}

    kf_nodiscard const T &calc(T x, T dx, Seconds dt) {
        if (first_step) {
            first_step = false;
            filtered = x;
        } else {
            T prediction = filtered + dx * dt;
            filtered = alpha * prediction + one_minus_alpha * x;
        }

        return filtered;
    }

    void reset() {
        first_step = true;
    }
};

}// namespace kf
