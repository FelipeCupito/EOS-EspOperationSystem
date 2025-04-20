// src/core/SystemManager.hpp

#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include <vector>
#include <string>
#include "Process.hpp"
#include "Peripheral.hpp"
#include "./Access/ServiceInvoker.hpp"


class ComponentManager {
    //TODO: implemenatr scheduler con prioridad para procesos
    //TODO: implementar que los preocesos y los perifericos sea no bloqueantes
private:
    // Usamos vectores en lugar de mapas para eficiencia en pequeñas colecciones
    std::vector<Peripheral*> peripherals;
    std::vector<Process*> processes;

    // Constructor privado para Singleton
    ComponentManager() = default;

public:
    // Patrón Singleton
    static ComponentManager& getInstance() {
        static ComponentManager instance;
        return instance;
    }

    // Prohibir copia y asignación
    ComponentManager(const ComponentManager&) = delete;
    ComponentManager& operator=(const ComponentManager&) = delete;

    void setup();
    void loop();

    template <typename Derived>
    ServiceInvoker<Derived> getServiceInvoker(const char* Name){
        for (Peripheral* peripheral : peripherals) {
            if (peripheral->getName() == Name) {
                return peripheral->getServiceInvoker();
            }
        }
        return nullptr;
    }
    
};

#endif // SYSTEM_MANAGER_HPP