#ifndef METHODREGISTRY_H
#define METHODREGISTRY_H

#include <array>
#include <functional>
#include <cassert>

#define REGISTER_SERVICE(ServiceEnum, Method) \
    registerService(ServiceEnum, &std::remove_reference<decltype(*this)>::type::Method)

/**
 * @file MethodRegistry.cpp
 * @brief Registers and manages service methods for a Peripheral.
 * 
 * This class allows the registration of service methods, and provides functionality to invoke them by index.
 * 
 * @tparam NumServices The number of services that can be registered.
 */
template<std::size_t NumServices>
class MethodRegistry {
public:

    /**
    * @brief Constructor that initializes the service array to nullptr.
    */
    constexpr MethodRegistry() : services_{} {
        static_assert(NumServices > 0, "The number of services must be greater than 0");
    }

    /**
     * @brief Registers a service method.
     * 
     * @param idx The index at which to register the service.
     * @param func Pointer to the service method.
    */
    void registerService(std::size_t idx, std::function<int(void*, void*)> func) {
        if (idx < NumServices) {
            services_[idx] = std::move(func);
        }
    }

    /**
     * @brief Invokes a service by index.
     * 
     * @param idx The index of the service to invoke.
     * @param instance Pointer to the instance that will call the method.
    */
    int invoke(std::size_t idx, void* instance, void* arg) {
        if (idx >= NumServices || !services_[idx]) {
            return -1; // Error: servicio no registrado o índice fuera de rango
        }
        return services_[idx](instance, arg);
    }

private:
    // Type definition for the function that stores pointers to member functions.
    using ServiceFunction = std::function<int(void*, void*)>;

    // Array of service functions.
    std::array<ServiceFunction, NumServices> services_;
};

#endif // METHODREGISTRY_H