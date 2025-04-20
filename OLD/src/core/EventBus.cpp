#include "EventBus.hpp"

template<typename Derived>
void EventBus::publishEvent(EventEmitter<Derived>* emitter, typename Derived::EventType event) {
    int eventCode = static_cast<int>(event);
    auto instanceSubscriptions = eventSubscriptions[emitter];
    if (instanceSubscriptions.find(eventCode) != instanceSubscriptions.end()) {
        for (auto& subscription : instanceSubscriptions[eventCode]) {
            subscription();  // Ejecutar las funciones suscritas
        }
    }
}

template<typename Derived>
void EventBus::subscribeToEvent(EventEmitter<Derived>* emitter, typename Derived::EventType event, std::function<void()> callback) {
    int eventCode = static_cast<int>(event);
    eventSubscriptions[emitter][eventCode].push_back(callback);
}
