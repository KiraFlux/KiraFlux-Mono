// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

// for avr capability
#include <math.h>// NOLINT(*-deprecated-headers)

#include "kf/Function.hpp"
#include "kf/core/aliases.hpp"
#include "kf/core/attributes.hpp"
#include "kf/memory/Array.hpp"
#include "kf/memory/Slice.hpp"
#include "kf/memory/StringView.hpp"
#include "kf/ui/Render.hpp"


namespace kf {// NOLINT(*-concat-nested-namespaces) // for c++11 capability
namespace ui {

/// @brief Text-based UI rendering system for terminal/console output
/// @note Implements Render CRTP interface for character-based display
struct TextRender : Render<TextRender> {
    friend Base;

    using Glyph = u8;///< Text interface measurement unit in glyphs

    /// @brief Text renderer configuration settings
    struct Config {
        using RenderHandler = Function<void(StringView)>;///< Render completion callback type

        Glyph row_max_length{16}; ///< Maximum characters per row
        Glyph rows_total{4};     ///< Total available rows in display
        Slice<char> buffer{};                     ///< Output buffer for rendered text
        RenderHandler on_render_finish{nullptr};///< Callback invoked when rendering completes
        Glyph float_places{2};
        Glyph double_places{4};

        Config(const Config &) = delete;
    };

    Config config{};///< Current renderer configuration

private:
    usize buffer_cursor{0};   ///< Current position in output buffer
    Glyph cursor_row{0};  ///< Current row position in virtual display
    Glyph cursor_col{0};  ///< Current column position in current row
    bool contrast_mode{false};///< Current contrast mode state

    kf_nodiscard usize widgetsAvailableImpl() const {
        return config.rows_total - cursor_row;
    }

    void prepareImpl() {
        buffer_cursor = 0;
    }

    void finishImpl() {
        if (nullptr == config.buffer.data()) {
            return;
        }

        cursor_row = 0;
        cursor_col = 0;
        config.buffer.data()[buffer_cursor - 1] = '\0';

        if (nullptr != config.on_render_finish) {
            config.on_render_finish({config.buffer.data(), buffer_cursor});
        }
    }

    void titleImpl(StringView title) {
        (void) print(title);
        (void) write('\n');
    }

    void checkboxImpl(bool enabled) {
        (void) print(enabled ? "==[ 1 ]" : "[ 0 ]--");
    }

    void valueImpl(StringView str) {
        (void) print(str);
    }

    void valueImpl(bool value) {
        (void) print(value ? "true" : "false");
    }

    void valueImpl(i32 integer) {
        (void) print(integer);
    }

    void valueImpl(f32 real) {
        (void) print(real, config.float_places);
    }

    void valueImpl(f64 real) {
        (void) print(real, config.double_places);
    }

    void arrowImpl() {
        (void) write('-');
        (void) write('>');
        (void) write(' ');
    }

    void colonImpl() {
        (void) write(':');
        (void) write(' ');
    }

    void beginContrastImpl() {
        (void) write('\x81');
        contrast_mode = true;
    }

    void endContrastImpl() {
        (void) write('\x80');
        contrast_mode = false;
    }

    void beginBlockImpl() {
        (void) write('[');
    }

    void endBlockImpl() {
        (void) write(']');
    }

    void beginAltBlockImpl() {
        (void) write('<');
    }

    void endAltBlockImpl() {
        (void) write('>');
    }

    void beginWidgetImpl(usize) {}

    void endWidgetImpl() {
        (void) write('\n');
    }

protected:

    kf_nodiscard usize print(StringView str) {
        const char *s = str.data();

        if (nullptr == s) {
            s = "nullptr";
        }

        usize written{0};

        while (*s != '\x00') {
            written += write(*s);
            s += 1;
        }

        return written;
    }

    /// @brief Print integer to buffer
    /// @param integer Integer value to print
    /// @return Number of characters written
    kf_nodiscard usize print(i32 integer) {
        if (integer == 0) {
            return write('0');
        }

        usize written{0};

        if (integer < 0) {
            integer = -integer;
            written += write('-');
        }

        char digits_buffer[12];

        auto digits_total{0};
        while (integer > 0) {
            const auto base = 10;

            digits_buffer[digits_total] = static_cast<char>(integer % base + '0');
            digits_total += 1;
            integer /= base;
        }

        for (auto i = digits_total - 1; i >= 0; i -= 1) {
            written += write(digits_buffer[i]);
        }

        return written;
    }

    /// @brief Print floating-point number to buffer
    /// @param real Floating-point value to print
    /// @param rounding Number of decimal places to show
    /// @return Number of characters written
    kf_nodiscard usize print(f64 real, u8 rounding) {
        if (isnan(real)) {
            return print("nan");
        }

        if (isinf(real)) {
            return print("inf");
        }

        usize written{0};

        if (real < 0) {
            real = -real;
            written += write('-');
        }

        written += print(i32(real));

        if (rounding > 0) {
            written += write('.');

            auto fractional = real - i32(real);

            for (auto i = 0; i < rounding; i += 1) {
                const auto base = 10;

                fractional *= base;
                const auto digit = u8(fractional);
                written += write('0' + digit);
                fractional -= digit;
            }
        }

        return written;
    }

    /// @brief Write single character to buffer
    /// @param c Character to write
    /// @return 1 if character written, 0 otherwise
    /// @note Handles line wrapping, row limits, and contrast mode
    kf_nodiscard usize write(char c) {
        if (buffer_cursor >= config.buffer.size()) {
            return 0;
        }

        if (cursor_row >= config.rows_total) {
            return 0;
        }

        if ('\n' == c) {
            cursor_row += 1;
            cursor_col = 0;
        } else {
            if (cursor_col >= config.row_max_length) {
                if (contrast_mode and buffer_cursor < config.buffer.size()) {
                    config.buffer.data()[buffer_cursor] = '\x80';
                    buffer_cursor += 1;
                    contrast_mode = false;
                }
                return 0;
            }
            cursor_col += 1;
        }
        config.buffer.data()[buffer_cursor] = c;
        buffer_cursor += 1;
        return 1;
    }
};

}// namespace ui
}// namespace kf