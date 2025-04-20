#include "Core.hpp"
#include "SystemManager.hpp"
#include "EventBus.hpp"
#include "./interfaces/Peripheral.hpp"
#include "./interfaces/Process.hpp"
#include "./interfaces/ServiceProvider.hpp"

void Core::setup() {
    SystemManager::getInstance().setup();
}

void Core::loop() {
    SystemManager::getInstance().loop();
}

void Core::registerPeripheral(Peripheral* peripheral) {
    SystemManager::getInstance().addPeripheral(peripheral);
}

void Core::registerProcess(Process* process) {
    SystemManager::getInstance().addProcess(process);
}

// // Service proxy
// template <typename Derived>
// typename Derived::ServiceAccessor* Core::getServiceProxy(const std::string& peripheralName) {
//     // Obtenemos el periférico registrado con el nombre dado
//     auto* peripheral = SystemManager::getInstance().getPeripheral(peripheralName);

//     // Intentamos hacer un dynamic_cast a un ServiceProvider
//     auto* serviceCapable = dynamic_cast<ServiceProvider<Derived>*>(peripheral);

//     if (!serviceCapable) {
//         // Manejo de error
//         return nullptr;
//     }

//     // Retornamos el proxy de servicio sin que el proceso tenga que especificar el número de servicios
//     return serviceCapable->getServiceProxy();
// }


template<typename Derived>
void Core::publishEvent(EventEmitter<Derived>* emitter, typename Derived::EventType event) {
    EventBus::getInstance().publishEvent(emitter, event);
}

template<typename Derived>
void Core::subscribeToEvent(EventEmitter<Derived>* emitter, typename Derived::EventType event, std::function<void()> callback) {
    EventBus::getInstance().subscribeToEvent(emitter, event, callback);
}

