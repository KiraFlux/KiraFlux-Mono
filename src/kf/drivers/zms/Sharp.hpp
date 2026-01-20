// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include <Arduino.h>

#include "kf/core/attributes.hpp"
#include "kf/math/units.hpp"
#include "kf/validation.hpp"

namespace kf {

/// @brief ИК Датчик расстояния Sharp
struct Sharp {

    /// @brief Значение выхода АЦП
    using AnalogValue = u16;

    /// @brief Настройки Sharp
    struct Settings : Validable<Settings> {

        /// @brief Аналоговый пин сенсора
        u8 pin;

        /// @brief Разрешение АЦП
        u8 resolution;

        kf_nodiscard inline AnalogValue maxValue() const {
            return static_cast<AnalogValue>((1u << resolution) - 1u);
        }

        void check(Validator &validator) const {
            kf_Validator_check(validator, resolution > 0);
            kf_Validator_check(validator, resolution <= 16);
        }
    };

    const Settings &settings;

private:
    AnalogValue max_value{0};

public:
    explicit Sharp(const Settings &settings) :
        settings{settings} {}

    kf_nodiscard bool init() {
        max_value = settings.maxValue();

        pinMode(settings.pin, INPUT);
        analogReadResolution(settings.resolution);

        return true;
    }

    /// @brief Считать значения датчика в величине АЦП
    kf_nodiscard inline AnalogValue readRaw() const {
        return analogRead(settings.pin);
    }

    /// @brief Считать расстояние в миллиметрах
    kf_nodiscard Millimeters read() const {
        // 65535 / analogRead(a)
        long sum = 0;

        const auto n = 4;

        for (int i = 0; i < n; i += 1) {
            sum += readRaw();
            delay(1);
        }

        return (65535.0F * n) / Millimeters(sum);
    }
};

}// namespace kf
