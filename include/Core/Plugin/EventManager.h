#pragma once
#include <functional>
#include <map>
#include <vector>
#include <string>
#include <iostream>
class EventManager {
public:
    // Определим тип обработчика событий
    using Callback = std::function<void(const std::string& eventData)>;

    // Подписка на событие с именем eventName
    void subscribe(const std::string& eventName, Callback callback) {
        subscribers[eventName].push_back(callback);
    }

    // Публикация (рассылка) события с именем eventName и данными eventData
    void publish(const std::string& eventName, const std::string& eventData) {
        if (subscribers.contains(eventName)) {
            for (auto& cb : subscribers[eventName]) {
                cb(eventData);
            }
        } else {
            std::cout << "Нет подписчиков на событие: " << eventName << std::endl;
        }
    }
private:
    std::map<std::string, std::vector<Callback>> subscribers;
};