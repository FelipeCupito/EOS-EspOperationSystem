// src/core/EventBus.hpp
#pragma once
#include <functional>
#include <unordered_map>
#include <vector>

// Forward declaration de EventEmitter
template<typename Derived>
class EventEmitter;

class EventBus {
private:
    std::unordered_map<void*, std::unordered_map<int, std::vector<std::function<void()>>>> eventSubscriptions;

    EventBus() = default;  // Constructor privado para Singleton

public:
    static EventBus& getInstance() {
        static EventBus instance;
        return instance;
    }

    // Prohibir copia y asignación
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    // Publicar y suscribirse a eventos
    template<typename Derived>
    void publishEvent(EventEmitter<Derived>* emitter, typename Derived::EventType event);

    template<typename Derived>
    void subscribeToEvent(EventEmitter<Derived>* emitter, typename Derived::EventType event, std::function<void()> callback);

};


// #pragma once
// #include <functional>
// #include <unordered_map>
// #include <string>
// #include <vector>
// #include "EventEmitter.hpp" 

// class EventBus {
// private:
//     std::unordered_map<void*, std::unordered_map<int, std::vector<std::function<void()>>>> eventSubscriptions;

// public:

//     static EventBus& getInstance() {
//         static EventBus instance;
//         return instance;
//     }

//     template<typename Derived>
//     void publishEvent(EventEmitter<Derived>* emitter, typename Derived::EventType event) {
//         int eventCode = static_cast<int>(event);
//         auto instanceSubscriptions = eventSubscriptions[emitter];
//         if (instanceSubscriptions.find(eventCode) != instanceSubscriptions.end()) {
//             for (auto& subscription : instanceSubscriptions[eventCode]) {
//                 subscription();  // Ejecutar las funciones suscritas
//             }
//         }
//     }

//     template<typename Derived>
//     void subscribeToEvent(EventEmitter<Derived>* emitter, typename Derived::EventType event, std::function<void()> callback) {
//         int eventCode = static_cast<int>(event);
//         eventSubscriptions[emitter][eventCode].push_back(callback);
//     }
// };
