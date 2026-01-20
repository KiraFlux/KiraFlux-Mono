// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include <Arduino.h>

#include "kf/core/attributes.hpp"
#include "kf/math/units.hpp"

namespace kf {
// todo Перенести в Timer

/// @brief Служба для отслеживания момента истечения допустимого таймаута
struct TimeoutManager final {

private:
    /// @brief Таймаут
    Milliseconds timeout;

    /// @brief Момент следующего таймаута
    Milliseconds next_timeout{0};

public:
    explicit TimeoutManager(Milliseconds timeout_duration) :
        timeout{timeout_duration} {}

    /// @brief Обновление таймаута
    /// @param now Текущее время
    void update(Milliseconds now) {
        next_timeout = now + timeout;
    }

    /// @brief Проверка истечения таймаута
    /// @param now Текущее время
    /// @returns true - таймаут просрочен
    kf_nodiscard inline bool expired(Milliseconds now) const { return now >= next_timeout; }
};

}// namespace kf
