#pragma once
#include <string>

// Интерфейс плагина
class Plugin {
public:
    virtual ~Plugin() = default;
    // Возвращает имя плагина
    virtual std::string name() const = 0;
    // Возвращает версию плагина
    virtual std::string version() const = 0;
    // Метод для подписки на события
    virtual void initialize(class EventManager& eventManager) = 0;
    // Основной метод плагина, выполняющий некоторую задачу
    virtual void execute() = 0;

    //... hooks
    virtual std::string overrideStartupURL(bool firstRun) {
        return std::string();
    }
};

// Фабричная функция для создания экземпляра плагина.
// Обратите внимание на extern "C" для упрощения поиска символа при динамической загрузке.
extern "C" {
    typedef Plugin* (*CreatePluginFunc)();
}
