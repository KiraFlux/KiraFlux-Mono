// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

namespace kf {

/// @brief Полная реализация шаблона проектирования "Одиночка"
template<typename T> struct Singleton {

    /// @brief Получить ссылку на единственный экземпляр
    static T &instance() {
        static T instance{};// NOLINT(*-dynamic-static-initializers)
        return instance;
    }

    /// @brief Запрет копирования
    Singleton(const Singleton &) = delete;

    /// @brief Запрет присваивания
    Singleton &operator=(const Singleton &) = delete;

protected:
    /// @brief Скрытие конструктора по умолчанию (Гарантия единственного экземпляра)
    Singleton() = default;

    /// @brief Скрытие деструктора (Гарантия валидности экземпляра всегда)
    ~Singleton() = default;
};

}// namespace kf
