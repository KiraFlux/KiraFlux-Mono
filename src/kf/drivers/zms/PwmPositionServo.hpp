// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include <Arduino.h>

#include "kf/core/attributes.hpp"
#include "kf/math/units.hpp"
#include "kf/validation.hpp"

namespace kf {

struct PwmPositionServo {

    struct PwmSettings : Validable<PwmSettings> {
        u32 ledc_frequency_hz;
        u8 ledc_resolution_bits;

        kf_nodiscard inline u32 maxDuty() const {
            return (1u << ledc_resolution_bits) - 1u;
        }

        kf_nodiscard u16 dutyFromPulseWidth(Milliseconds pulse_width) const {
            const auto t = u64(pulse_width) * ledc_frequency_hz * maxDuty();
            return u16(t / 1000000u);
        }

        void check(Validator &validator) const {
            kf_Validator_check(validator, ledc_frequency_hz > 0);
            kf_Validator_check(validator, ledc_resolution_bits >= 8);
            kf_Validator_check(validator, ledc_resolution_bits <= 16);
        }
    };

    struct DriverSettings : Validable<DriverSettings> {
        u8 signal_pin;
        u8 ledc_channel;

        /// @brief Минимальный угол оси
        Degrees min_angle;

        /// @brief Максимальный угол оси
        Degrees max_angle;

        void check(Validator &validator) const {
            kf_Validator_check(validator, ledc_channel <= 15);
            kf_Validator_check(validator, min_angle < max_angle);
        }
    };

    struct PulseSettings : Validable<PulseSettings> {
        struct Pulse {
            Microseconds pulse;
            Degrees angle;
        };

        Pulse min_position;
        Pulse max_position;

        kf_nodiscard Microseconds pulseWidthFromAngle(Degrees angle) const {
            return map(
                constrain(angle, min_position.angle, max_position.angle),
                min_position.angle,
                max_position.angle,
                static_cast<long>(min_position.pulse),
                static_cast<long>(max_position.pulse));
        }

        void check(Validator &validator) const {
            kf_Validator_check(validator, min_position.pulse < max_position.pulse);
            kf_Validator_check(validator, min_position.angle < max_position.angle);
        }
    };

private:
    const PwmSettings &pwm_settings;
    const DriverSettings &driver_settings;
    const PulseSettings &pulse_settings;

public:
    explicit constexpr PwmPositionServo(
        const PwmSettings &pwm_settings,
        const DriverSettings &driver_settings,
        const PulseSettings &pulse_settings) :
        driver_settings{driver_settings}, pwm_settings(pwm_settings), pulse_settings(pulse_settings) {}

    kf_nodiscard bool init() const {
        const auto freq = ledcSetup(
            driver_settings.ledc_channel,
            pwm_settings.ledc_frequency_hz,
            pwm_settings.ledc_resolution_bits);

        if (freq == 0) {
            kf_Logger_error("LEDC setup failed");
            return false;
        }

        ledcAttachPin(driver_settings.signal_pin, driver_settings.ledc_channel);

        return true;
    }

    void set(Degrees angle) {
        write(pwm_settings.dutyFromPulseWidth(pulse_settings.pulseWidthFromAngle(angle)));
    }

    void disable() {
        write(0);
    }

private:
    void write(u16 duty) const {
        ledcWrite(driver_settings.ledc_channel, duty);
    }
};

}// namespace kf