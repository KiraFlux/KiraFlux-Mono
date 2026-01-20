// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include "kf/core/attributes.hpp"
#include "kf/drivers/zms/PwmPositionServo.hpp"
#include "kf/math/units.hpp"
#include "kf/validation.hpp"

namespace kf {

/// @brief Двухосевой манипулятор
struct Manipulator2DOF {

    /// @brief Настройки двухосного манипулятора
    struct Settings : Validable<Settings> {

        /// @brief Настройки ШИМ сервопривода
        PwmPositionServo::PwmSettings servo_pwm;

        /// @brief Настройки Pulse сервопривода
        PwmPositionServo::PulseSettings servo_generic_pulse_settings;

        /// @brief Настройки оси захвата
        PwmPositionServo::DriverSettings claw_axis;

        /// @brief Настройки оси звена
        PwmPositionServo::DriverSettings arm_axis;

        void check(Validator &validator) const {
            kf_Validator_check(validator, arm_axis.isValid());
            kf_Validator_check(validator, claw_axis.isValid());
            kf_Validator_check(validator, servo_pwm.isValid());
            kf_Validator_check(validator, servo_generic_pulse_settings.isValid());
        }
    };

private:
    /// @brief Настройки манипулятора
    const Settings &settings;

    /// @brief Привод оси звена
    PwmPositionServo arm_axis;

    /// @brief Привод оси захвата
    PwmPositionServo claw_axis;

public:
    explicit Manipulator2DOF(const Settings &settings) :
        settings{settings},
        arm_axis{settings.servo_pwm, settings.arm_axis, settings.servo_generic_pulse_settings},
        claw_axis{settings.servo_pwm, settings.claw_axis, settings.servo_generic_pulse_settings} {}

    /// @brief Инициализировать захват
    kf_nodiscard bool init() {
        if (not arm_axis.init()) {
            kf_Logger_error("arm axis fail");
            return false;
        }

        if (not claw_axis.init()) {
            kf_Logger_error("claw axis fail");
            return false;
        }

        return true;
    }

    inline void setArm(Degrees angle) { arm_axis.set(angle); }

    inline void setClaw(Degrees angle) { claw_axis.set(angle); }

    inline void disableArm() { arm_axis.disable(); }

    inline void disableClaw() { claw_axis.disable(); }
};

}// namespace kf
