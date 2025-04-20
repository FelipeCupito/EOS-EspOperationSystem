#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include <tuple>
#include <type_traits>

//#include "../Core.hpp"
#include "Component.hpp"
#include "./CommInterfaces/ExposedVars.hpp"
#include "./CommInterfaces/Concepts.h"
#include "./CommInterfaces/EventEmitter.hpp"
#include "./Access/VarViewer.hpp"
#include "./Access/ServiceInvoker.hpp"


/**
 * @file Peripheral.cpp
 * @brief A base class that handles different communication strategies for peripherals.
 * 
 * This class combines various communication interfaces, such as ExposedVars, MethodRegistry, 
 * and EventEmitter, enabling peripherals to expose variables, register methods, and emit events.
 * 
 * @tparam Derived The derived class using this base Peripheral class.
 * @tparam CommInterfaces A variadic list of communication interfaces.
 */
template <typename Derived, typename... CommInterfaces>
class Peripheral: public Component, public IPeripheralInterface {
    static_assert(unique_types<CommInterfaces...>::value, 
                  "Error: Duplicate communication interfaces detected.");

public:
    Peripheral(const char* name) : interfaces_{CommInterfaces()...}, Component(name) {
        //Core::getComponentManager().registerPeripheral(this);
    };

    virtual void setup() = 0;
    virtual void loop() = 0;
    
    /******************** Event Emission ***********************************/
    /**
     * @brief Emits an event from the derived class.
     * This method is only available if the derived class has an enum called EventName.
     * 
     * @tparam EventEnum The enum type representing the event.
     * @param event The enum value representing the event to emit.
     */
    template <typename EventEnum>
    requires HasCommInterfaces<EventEmitter, CommInterfaces...>
    void emit(EventEnum event) const {
        static_assert(HasEventTypeEnum<Derived>, "Error: Derived class must have an enum named 'EventName'");
        
        //TODO: Implementar
    }
    
    /******************** ExposedVars ***********************************/
    /**
     * @brief Registers a shared variable for a given Derived instance.
     * 
     * @tparam VariableEnum The enum representing the variable.
     * @tparam VarType The type of the variable to register.
     * @param varEnum The enum value representing the variable.
     * @param varPtr A pointer to the variable.
     */
    template <typename VariableEnum, typename VarType>
    requires HasCommInterfaces<ExposedVars<static_cast<std::size_t>(VariableEnum::COUNT)>, CommInterfaces...>
    constexpr void registerVariable(VariableEnum varEnum, const VarType* varPtr) {

        auto& exposedVars = getExposedVars();
        exposedVars.template registerVariable(static_cast<std::size_t>(varEnum), varPtr);
    }

    /**
     * @brief Returns a VarViewer that encapsulates access to all shared variables.
     */
    auto getVarViewer() const
    requires HasCommInterfaces<ExposedVars<static_cast<std::size_t>(Derived::VariableName::COUNT)>, CommInterfaces...>
    {
        return VarViewer<Derived, static_cast<std::size_t>(Derived::VariableName::COUNT)>(getExposedVars());
    }

    /******************** MethodRegistry ***********************************/
    /**
     * @brief Registers a service method with a specific argument type.
     * 
     * @tparam ArgType The type of the argument for the service method.
     * @tparam ServiceName The enum representing the service.
     * @param service The enum value representing the service.
     * @param func A pointer to the service method.
     */
    template <typename ArgType, typename ServiceName>
    requires HasCommInterfaces<MethodRegistry<static_cast<std::size_t>(Derived::ServiceName::COUNT)>, CommInterfaces...>
    void registerService(ServiceName service, int (Derived::*func)(ArgType)) {
        std::size_t idx = static_cast<std::size_t>(service);
        auto& provider = getMethodRegistry();

        // Convierte el método en uno genérico que utiliza void*
        provider.registerService(idx, [func](void* instance, void* arg) -> int {
            return (static_cast<Derived*>(instance)->*func)(*static_cast<ArgType*>(arg));
        });
    }

    /**
     * @brief Registers a service method with a void* argument.
     * 
     * @tparam ServiceName The enum representing the service.
     * @param service The enum value representing the service.
     * @param func A pointer to the service method.
     */
    template <typename ServiceName>
    requires HasCommInterfaces<MethodRegistry<static_cast<std::size_t>(Derived::ServiceName::COUNT)>, CommInterfaces...>
    void registerService(ServiceName service, int (Derived::*func)(void*)) {
        std::size_t idx = static_cast<std::size_t>(service);
        auto& provider = getMethodRegistry();

        // Simplemente registra el método con void*
        provider.registerService(idx, [func](void* instance, void* arg) -> int {
            return (static_cast<Derived*>(instance)->*func)(arg);
        });
    }

    /**
     * @brief Registers a service method with no arguments.
     * 
     * @tparam ServiceName The enum representing the service.
     * @param service The enum value representing the service.
     * @param func A pointer to the service method.
     */
    template <typename ServiceName>
    requires HasCommInterfaces<MethodRegistry<static_cast<std::size_t>(Derived::ServiceName::COUNT)>, CommInterfaces...>
    void registerService(ServiceName service, int (Derived::*func)()) {
        std::size_t idx = static_cast<std::size_t>(service);
        auto& provider = getMethodRegistry();

        // Convierte el método en uno que no requiere argumentos.
        provider.registerService(idx, [func](void* instance, void*) -> int {
            return (static_cast<Derived*>(instance)->*func)();
        });
    }

    /**
     * @brief Returns a ServiceInvoker to call services without exposing the entire Peripheral.
     * @return A ServiceInvoker object.
     */
    //template <typename Derived>
    //ServiceInvoker<Derived> getServiceInvoker() 
    auto getServiceInvoker()
    requires HasCommInterfaces<MethodRegistry<static_cast<std::size_t>(Derived::ServiceName::COUNT)>, CommInterfaces...>
    {
        return ServiceInvoker<Derived>(static_cast<Derived*>(this));
        //return ServiceInvoker(static_cast<Derived*>(this));
    }

    // Proporcionar acceso público al MethodRegistry (usado por ServiceInvoker)
    int invokeService(std::size_t idx, void* instance, void* arg) 
    requires HasCommInterfaces<MethodRegistry<static_cast<std::size_t>(Derived::ServiceName::COUNT)>, CommInterfaces...>
    {
        return getMethodRegistry().invoke(idx, instance, arg);
    }

private:
    /**
     * @brief Tuple to store the communication interfaces.
     */
    std::tuple<CommInterfaces...> interfaces_;

    /**
     * @brief Retrieves the MethodRegistry from the communication interfaces.
     */
    constexpr auto& getMethodRegistry()
    requires HasCommInterfaces<MethodRegistry<static_cast<std::size_t>(Derived::ServiceName::COUNT)>, CommInterfaces...>
    {
        return std::get<MethodRegistry<static_cast<std::size_t>(Derived::ServiceName::COUNT)>>(interfaces_);
    }

    /**
     * @brief Retrieves the ExposedVars from the communication interfaces.
     */
    constexpr auto& getExposedVars() const
    requires HasCommInterfaces<ExposedVars<static_cast<std::size_t>(Derived::VariableName::COUNT)>, CommInterfaces...>
    {
        return std::get<ExposedVars<static_cast<std::size_t>(Derived::VariableName::COUNT)>>(interfaces_);
    }
};

#endif // PERIPHERAL_H