// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include "kf/core/aliases.hpp"

namespace kf {

// Угол

/// @brief Угол в градусах (deg)
using Degrees = u16;

// Расстояние

/// @brief Описывает позицию пикселях
using Pixel = i16;

/// @brief Псевдоним типа выражения физической величины в миллиметрах (mm)
using Millimeters = f64;

// Время

/// @brief Выражает физическую величину отсчёта времени в секундах (s)
using Seconds = f32;

/// @brief Выражает физическую величину в Герцах (Гц) (Hz = 1 / s)
using Hertz = u16;

/// @brief Выражает физическую величину отсчёта времени в миллисекундах (ms)
using Milliseconds = u32;

/// @brief Выражает физическую величину отсчёта времени в микросекундах (us)
using Microseconds = u32;

}// namespace kf
