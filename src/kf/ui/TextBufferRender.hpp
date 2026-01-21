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
/// @tparam N text buffer capacity
/// @note Implements Render CRTP interface for character-based display
template<usize N> struct TextBufferRender : Render<TextBufferRender<N>> {
    friend struct Render<TextBufferRender<N>>;

    using Glyph = u8;///< Text interface measurement unit in glyphs

    /// @brief Text renderer configuration settings
    struct Config {
        using RenderHandler = Function<void(StringView)>;///< Render completion callback type

        Glyph row_max_length{16}; ///< Maximum characters per row
        Glyph rows_total{4};     ///< Total available rows in display
        RenderHandler on_render_finish{nullptr};///< Callback invoked when rendering completes
        Glyph float_places{2};
        Glyph double_places{4};

        Config(const Config &) = delete;
    };

    Config config{};///< Current renderer configuration

private:
    ArrayString<N> buffer{};
    Glyph cursor_row{0};  ///< Current row position in virtual display
    Glyph cursor_col{0};  ///< Current column position in current row
    bool contrast_mode{false};///< Current contrast mode state

    kf_nodiscard usize widgetsAvailableImpl() const {
        return config.rows_total - cursor_row;
    }

    void prepareImpl() {
        buffer.clear();
    }

    void finishImpl() {
        if (nullptr == buffer.data()) {
            return;
        }

        cursor_row = 0;
        cursor_col = 0;

        buffer.data()[buffer.size() - 1] = '\0';

        if (nullptr != config.on_render_finish) {
            config.on_render_finish(buffer.view());
        }
    }

    void titleImpl(StringView title) {
        print(title);
        write('\n');
    }

    void checkboxImpl(bool enabled) { print(enabled ? "==[ 1 ]" : "[ 0 ]--"); }

    void valueImpl(StringView str) { print(str); }

    void valueImpl(bool value) { print(value ? "true" : "false"); }

    void valueImpl(i32 integer) { print(integer); }

    void valueImpl(f32 real) { print(real, config.float_places); }

    void valueImpl(f64 real) { print(real, config.double_places); }

    void arrowImpl() {
        write('-');
        write('>');
        write(' ');
    }

    void colonImpl() {
        write(':');
        write(' ');
    }

    void beginContrastImpl() {
        write('\x81');
        contrast_mode = true;
    }

    void endContrastImpl() {
        write('\x80');
        contrast_mode = false;
    }

    void beginBlockImpl() { write('['); }

    void endBlockImpl() { write(']'); }

    void beginAltBlockImpl() { write('<'); }

    void endAltBlockImpl() { write('>'); }

    void beginWidgetImpl(usize) {}

    void endWidgetImpl() { write('\n'); }

protected:

    void print(StringView str) {
        const char *s = str.data();

        if (nullptr == s) {
            s = "nullptr";
        }

        while (*s != '\0') {
            write(*s);
            s += 1;
        }
    }

    /// @brief Print integer to buffer
    /// @param integer Integer value to print
    /// @return Number of characters written
    void print(i32 integer) {
        if (integer == 0) {
            write('0');
            return;
        }

        if (integer < 0) {
            integer = -integer;
            write('-');
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
            write(digits_buffer[i]);
        }
    }

    /// @brief Print floating-point number to buffer
    /// @param real Floating-point value to print
    /// @param rounding Number of decimal places to show
    /// @return Number of characters written
    void print(f64 real, u8 rounding) {
        if (isnan(real)) {
            print("nan");
            return;
        }

        if (isinf(real)) {
            print("inf");
            return;
        }

        if (real < 0) {
            real = -real;
            write('-');
        }

        print(i32(real));

        if (rounding > 0) {
            write('.');

            auto fractional = real - i32(real);

            for (auto i = 0; i < rounding; i += 1) {
                const auto base = 10;

                fractional *= base;
                const auto digit = u8(fractional);
                write('0' + digit);
                fractional -= digit;
            }
        }
    }

    /// @brief Write single character to buffer
    /// @param c Character to write
    /// @return 1 if character written, 0 otherwise
    /// @note Handles line wrapping, row limits, and contrast mode
    void write(char c) {
        if (buffer.full()) {
            return;
        }

        if (cursor_row >= config.rows_total) {
            return;
        }

        if ('\n' == c) {
            cursor_row += 1;
            cursor_col = 0;
        } else {
            if (cursor_col >= config.row_max_length) {
                if (contrast_mode and not buffer.full()) {
                    (void) buffer.push('\x80');
                    contrast_mode = false;
                }
                return;
            }
            cursor_col += 1;
        }
        (void) buffer.push(c);
    }
};

}// namespace ui
}// namespace kf