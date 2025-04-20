// src/core/interfaces/EventEmitter.hpp

#pragma once
#include "../EventBus.hpp"
// Forward declaration de EventBus
class EventBus;

template<typename Derived>
class EventEmitter {
public:
    EventEmitter() {
        // Verificamos que la clase derivada defina un enum EventType
        static_assert(std::is_enum<typename Derived::EventType>::value,
                      "Derived class must define an EventType enum");
    }

    void emit(typename Derived::EventType event) {
        EventBus::getInstance().publishEvent(this, event);
    }

    /** Creo que no es necesario */
    // void subscribe(typename Derived::EventType event, std::function<void()> callback) {
    //     EventBus::getInstance().subscribeToEvent(this, event, callback);
    // }
};