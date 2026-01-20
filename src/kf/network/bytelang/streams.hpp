#pragma once

#include <Stream.h>

#include "kf/Option.hpp"
#include "kf/core/aliases.hpp"
#include "kf/core/attributes.hpp"

namespace kf::bytelang {

/// Входной поток (чтение данных)
struct InputStream {

private:
    Stream &stream;

public:
    explicit InputStream(Stream &s) :
        stream{s} {}

    void clean() {
        while (stream.available()) {
            (void) stream.read();
        }
    }

    /// Сколько байт доступно для чтения
    kf_nodiscard usize available() {
        return stream.available();
    }

    /// Прочитать один байт
    kf_nodiscard Option<u8> readByte() {
        const auto result = stream.read();

        if (result == -1) {
            return {};
        } else {
            return {static_cast<u8>(result)};
        }
    }

    /// Прочитать объект типа T
    template<typename T> kf_nodiscard Option<T> read() {
        T value;

        const usize bytes_read = stream.readBytes(
            reinterpret_cast<u8 *>(&value),
            sizeof(T));

        if (bytes_read == sizeof(T)) {
            return {value};
        } else {
            return {};
        }
    }
};

/// Выходной поток (запись данных)
struct OutputStream {

private:
    Stream &stream;

public:
    explicit OutputStream(Stream &s) :
        stream{s} {}

    /// Записать один байт
    kf_nodiscard bool writeByte(u8 byte) {
        return stream.write(byte) == 1;
    }

    /// Записать буфер
    kf_nodiscard bool write(const void *data, usize length) {
        return stream.write(static_cast<const u8 *>(data), length) == length;
    }

    /// Записать объект типа T
    template<typename T> kf_nodiscard inline bool write(const T &value) {
        return write(static_cast<const void *>(&value), sizeof(T));
    }
};

}// namespace kf::bytelang
