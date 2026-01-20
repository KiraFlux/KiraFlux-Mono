# KiraFlux Toolkit

Кроссплатформенная библиотека для встраиваемых систем на C++. Объединяет несколько ранее разделённых модулей в единый репозиторий.

## Основные компоненты

- **Ядро** - кроссплатформенные утилиты, аллокаторы, логгирование
- **Графика** - Canvas, драйверы дисплеев (SSD1306, ST7735), шрифты
- **Ввод** - джойстики, кнопки, энкодеры
- **Пользовательский интерфейс** - виджеты, страницы, навигация
- **Сеть** - ESP-NOW, структурированная коммуникация
- **Драйверы** - моторы, сервоприводы, датчики
- **Математика** - векторы, фильтры, PID, таймеры

## Поддерживаемые платформы

- AVR (Arduino Nano, Uno, Mega)
- ESP32
- Минимальный стандарт C++11

## Быстрый старт

### PlatformIO

```ini
lib_deps = https://github.com/KiraFlux/KiraFlux-Toolkit.git
```

### Пример

```cpp
#include <kf/Logger.hpp>

void setup() {
    kf_Logger_setWriter([](auto msg) { Serial.write(msg.data(), msg.size()); });
    kf_Logger_info("System started");
}
```

## Документация

- [Графика и дисплеи](./src/kf/gfx/README.md)
- [Драйверы ввода](./src/kf/drivers/input/README.md)
- [Драйверы вывода](./src/kf/drivers/display/README.md)
- [Пользовательский интерфейс](./src/kf/ui/README.md)
- [EspNow](./src/kf/network/EspNow.md)
- [Последовательный протокол / структурированная коммуникация](./src/kf/network/bytelang/README.md)

## Лицензия

MIT License