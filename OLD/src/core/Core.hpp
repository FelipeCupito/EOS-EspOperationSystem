// // src/core/Core.hpp
#pragma once
#include <string>
#include <functional>
#include "./componentManager/SystemManager.hpp"

// Forward declarations
// class Peripheral;
// class Process;
// class EventBus;
// class SystemManager;
// template<typename Derived>
// class EventEmitter;


class Core {
private:
    // Constructor privado para Singleton
    Core() = default;

public:
    // Singleton
    static Core& getInstance() {
        static Core instance;
        return instance;
    }

    // Prohibido el uso de copia o asignación
    Core(const Core&) = delete;
    Core& operator=(const Core&) = delete;

    void setup();
    void loop();

    // Registra un periférico en el sistema
    void registerPeripheral(Peripheral* peripheral);
    void registerProcess(Process* process);

    // Trait para asegurar que el tipo tiene un enum ServiceName con COUNT
    template<typename T, typename = void>
    struct has_valid_service_name : std::false_type {};

    template<typename T>
    struct has_valid_service_name<T, std::void_t<decltype(T::ServiceName::COUNT)>> : std::true_type {};

    // template <typename ServiceType>
    // typename ServiceType::ServiceApi* getServiceApi(const std::string& peripheralName) {
    //     // Aseguramos en tiempo de compilación que ServiceType tiene un enum llamado ServiceName con COUNT
    //     static_assert(has_valid_service_name<ServiceType>::value, "ServiceType must define an enum ServiceName with a COUNT value");

    //     // Obtenemos el periférico registrado con el nombre dado
    //     Peripheral* peripheral = SystemManager::getInstance().getPeripheral(peripheralName);

    //     // Obtenemos el número de servicios a través de ServiceName::COUNT
    //     constexpr std::size_t NumServices = static_cast<std::size_t>(ServiceType::ServiceName::COUNT);

    //     // Intentamos hacer un static_cast a ServiceProvider<ServiceType>
    //     ServiceProvider<ServiceType, NumServices>* serviceProvider = static_cast<ServiceProvider<ServiceType, NumServices>*>(peripheral);

    //     if (!serviceProvider) {
    //         // Manejo de error: el periférico no es un ServiceProvider válido
    //         return nullptr;
    //     }

    //     // Retornamos el API de servicio
    //     return serviceProvider->getServiceApi();
    // }
    template <typename ServiceType>
    typename ServiceType::ServiceApi* getServiceApi(const std::string& peripheralName) {
        // // Obtenemos el periférico registrado
        // Peripheral* peripheral = SystemManager::getInstance().getPeripheral(peripheralName);

        // if (!peripheral) {
        //     return nullptr; // No se encontró el periférico
        // }

        // // Si el periférico es un ServiceProvider, hacemos un cast
        // if constexpr (is_service_provider<ServiceType>::value) {
        //     // Cast a ServiceProvider si es válido
        //     auto* serviceProvider = static_cast<ServiceProvider<ServiceType, static_cast<std::size_t>(ServiceType::ServiceName::COUNT)>*>(peripheral);

        //     if (serviceProvider) {
        //         return serviceProvider->getServiceApi();
        //     }
        // }

        return nullptr; // No es un ServiceProvider
    }

    // Event bus
    template<typename Derived>
    void publishEvent(EventEmitter<Derived>* emitter, typename Derived::EventType event);

    template<typename Derived>
    void subscribeToEvent(EventEmitter<Derived>* emitter, typename Derived::EventType event, std::function<void()> callback);
};

// class Core {
// private:
//     SystemManager& systemManager = SystemManager::getInstance();
//     EventBus eventBus = EventBus::getInstance();

// public:
//     static Core& getInstance() {
//         static Core instance;
//         return instance;
//     }

//     void setup() {
//         systemManager.setup();
//     }

//     void loop() {
//         systemManager.loop();
//     }

//     /** SysteManager **/
//     void registerPeripheral(Peripheral* peripheral) {
//         systemManager.addPeripheral(peripheral);
//     }

//     void registerProcess(Process* process) {
//         systemManager.addProcess(process);
//     }


//     /** Service proxy. **/
//     // Obtener un proxy de servicio con manejo de errores
//     template <typename Derived>
//     typename Derived::ServiceAccessor* getServiceProxy(const std::string& peripheralName) {
//         auto* peripheral = getPeripheral(peripheralName);
//         auto* serviceCapable = dynamic_cast<ServiceProvider<Derived>*>(peripheral);
        
//         if (!serviceCapable) {
//             //TODO, manejo de error, deberia ser un erro de compilacion pero no se puede hacer con dynamic_cast
//         }

//         return dynamic_cast<typename Derived::ServiceAccessor*>(serviceCapable->getServiceProxy());
//     }

//     /** event bus **/
//     template<typename Derived>
//     void publishEvent(EventEmitter<Derived>* emitter, typename Derived::EventType event) {
//         eventBus.publishEvent(emitter, event);
//     }

//     template<typename Derived>
//     void subscribeToEvent(EventEmitter<Derived>* emitter, typename Derived::EventType event, std::function<void()> callback) {
//         eventBus.subscribeToEvent(emitter, event, callback);
//     }
// };
