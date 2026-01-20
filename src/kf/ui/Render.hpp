// Copyright (c) 2026 KiraFlux
// SPDX-License-Identifier: MIT

#pragma once

#include "kf/core/aliases.hpp"
#include "kf/core/attributes.hpp"

namespace kf {// NOLINT(*-concat-nested-namespaces) // for c++11 capability

/// @brief отдельное пространство имён для внешних компонентов UI
namespace ui {

/// @brief Система рендера
template<typename Impl> struct Render {
    friend Impl;

    // Управление

    /// @brief Подготовить буфер отрисовки
    void prepare() { impl().prepareImpl(); }

    /// @brief После рендера кадра
    void finish() { impl().finishImpl(); }

    /// @brief Начало отрисовки виджета
    void beginWidget(usize index) { impl().beginWidgetImpl(index); }

    /// @brief Завершение отрисовки виджета
    void endWidget() { impl().endWidgetImpl(); }

    /// @brief Количество виджетов, которые ещё возможно отобразить
    kf_nodiscard usize widgetsAvailable() { return impl().widgetsAvailableImpl(); }

    // Значения

    /// @brief Заголовок страницы
    void title(const char *title) { impl().titleImpl(title); }

    /// @brief Отобразить строку
    void string(const char *str) { impl().stringImpl(str); }

    /// @brief Отобразить целое число
    void number(i32 integer) { impl().numberImpl(integer); }

    /// @brief Отобразить вещественное число
    void number(f64 real, u8 rounding) { impl().numberImpl(real, rounding); }

    // Оформление

    /// @brief Отобразить стрелку от края к виджету
    void arrow() { impl().arrowImpl(); }

    /// @brief Колонка (Разделитель)
    void colon() { impl().colonImpl(); }

    /// @brief Контрастный текст
    void beginContrast() { impl().beginContrastImpl(); }

    /// @brief Контрастный текст
    void endContrast() { impl().endContrastImpl(); }

    /// @brief Блок
    void beginBlock() { impl().beginBlockImpl(); }

    /// @brief Блок
    void endBlock() { impl().endBlockImpl(); }

    /// @brief Альтернативный блок
    void beginAltBlock() { impl().beginAltBlockImpl(); }

    /// @brief Альтернативный блок
    void endAltBlock() { impl().endAltBlockImpl(); }

private:
    inline Impl &impl() { return *static_cast<Impl *>(this); }
};

}// namespace ui
}// namespace kf
