// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

namespace kf {// NOLINT(*-concat-nested-namespaces) // for c++11 capability
namespace ui {// NOLINT(*-concat-nested-namespaces)
namespace detail {// NOLINT(*-concat-nested-namespaces)

template<typename T> struct step_adjuster_traits;

template<> struct step_adjuster_traits<int> {
    static constexpr int min_step{1};
    static constexpr int default_step{1};
};

template<> struct step_adjuster_traits<float> {
    static constexpr float min_step{1e-3f};
    static constexpr float default_step{0.1f};
};

template<> struct step_adjuster_traits<double> {
    static constexpr double min_step{1e-6};
    static constexpr double default_step{0.01};
};


/// @brief Step adjustment with type-specific protection
template<typename T> struct StepAdjuster {
    static constexpr T step_multiplier{static_cast<T>(10)};
    static constexpr T min_step{step_adjuster_traits<T>::min_step};
    static constexpr T default_step{step_adjuster_traits<T>::default_step};

    static void adjust(T &step, int direction) noexcept {
        if (direction > 0) {
            step *= step_multiplier;
        } else {
            step /= step_multiplier;
            // Protection for integral types
            if (step < min_step) { step = min_step; }
        }
    }
};

}
}
}