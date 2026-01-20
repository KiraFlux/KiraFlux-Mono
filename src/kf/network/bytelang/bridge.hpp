// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include "kf/Function.hpp"
#include "kf/Result.hpp"
#include "kf/core/utility.hpp"
#include "kf/memory/Array.hpp"
#include "kf/network/bytelang/streams.hpp"

namespace kf::bytelang::bridge {

/// Ошибки обработки инструкций
enum class Error : u8 {

    /// Полученный код инструкции не соответствует ни одной инструкции.
    UnknownInstruction,

    /// Не удалось считать код инструкции
    InstructionCodeReadFail,

    /// Не удалось отправить код инструкции
    InstructionCodeWriteFail,

    /// Обработчик исполнения инструкции отправки не указан
    InstructionSendHandlerIsNull,

    // для применения в пользовательских инструкциях

    /// Не удалось прочесть аргумент
    InstructionArgumentReadFail,

    /// Не удалось записать аргумент
    InstructionArgumentWriteFail
};

/// Слушатель инструкций (Принимает код и аргументы)
template<

    /// Тип кода принимаемой инструкции
    typename T,

    /// Количество инструкций
    usize N
    //
    >
struct Receiver {

    /// Код инструкции на приём
    using Code = T;

    /// Количество инструкций
    static constexpr auto instruction_count = N;

    /// Контейнер для таблицы инструкций
    using InstructionTable = Array<Function<Result<void, Error>(InputStream &)>, instruction_count>;

    /// Сериализатор
    InputStream in;

    /// Обработчики на приём
    InstructionTable instructions;

    /// Обновление (Пул проверки)
    Result<void, Error> poll() {
        if (in.available() < sizeof(Code)) { return {}; }

        auto code_option = in.read<Code>();

        if (not code_option.hasValue()) {
            return {Error::InstructionCodeReadFail};
        }

        const auto code = code_option.value();

        if (code >= instruction_count) {
            in.clean();
            return {Error::UnknownInstruction};
        }

        return instructions[code](in);
    }

    // Запрет неявного создания
    Receiver() = delete;
};

/// Инструкция отправки
template<

    /// Примитив кода инструкции
    typename T,

    /// Сигнатура аргументов
    typename... Args
    //
    >
struct Instruction {

    /// Код инструкции на отправку
    using Code = T;

    /// Обработчик вызова инструкции
    using Handler = Function<Result<void, Error>(OutputStream &, Args...)>;

private:
    /// Сериализатор
    OutputStream &out;

    /// Обработчик вызова
    const Handler handler;

    /// Код инструкции
    const Code code;

public:
    Instruction(OutputStream &output_stream, Code code, Handler call_handler) :
        out{output_stream}, handler{kf::move(call_handler)}, code{code} {}

    Instruction(Instruction &&other) noexcept :
        out{other.out}, handler{kf::move(other.handler)}, code{other.code} {}

    /// Вызвать инструкцию
    Result<void, Error> operator()(Args... args) {
        if (nullptr == handler) {
            return {Error::InstructionSendHandlerIsNull};
        }

        if (not out.write(code)) {
            return {Error::InstructionCodeWriteFail};
        }

        return handler(out, args...);
    }

    Instruction() = delete;
};

/// Протокол отправки
template<

    /// Тип кода отправляемой инструкции
    typename T
    //
    >
struct Sender {

    /// Тип кода отправляемой инструкции
    using Code = T;

private:
    /// Сериализатор
    OutputStream out;

    /// Счётчик инструкций
    Code next_code{0};

public:
    explicit Sender(OutputStream &&output_stream) :
        out{output_stream} {}

    /// Создать инструкцию отправки
    template<typename... Args> Instruction<Code, Args...> createInstruction(typename Instruction<Code, Args...>::Handler handler) {
        return Instruction<Code, Args...>{out, next_code++, kf::move(handler)};
    }
};

}// namespace kf::bytelang::bridge
