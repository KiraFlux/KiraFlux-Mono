// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include <stdarg.h>

#include "kf/core/aliases.hpp"
#include "kf/memory/Array.hpp"

namespace kf {

struct String {};

template<usize N> using ArrayString = kf::Array<char, N>;

/// @brief Форматировать Массив-строку через <code>vsnprintf</code>
template<usize N> void formatTo(ArrayString<N> &destination, const char *format, ...) {
    if (N == 0) { return; }

    va_list args;
    va_start(args, format);

    vsnprintf(destination.data(), N, format, args);
    destination[N - 1] = '\0';

    va_end(args);
}

}// namespace kf
