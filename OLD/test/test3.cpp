#include <iostream>
#include <string>
#include <functional>
#include "Peripheral.cpp"

// Actuator: Solo tiene MethodRegistry y responde a órdenes desde otros sensores
class Actuator : public Peripheral<Actuator, MethodRegistry<2>> {
public:
    // Enum to identify the services (methods)
    enum class ServiceName {
        TurnOn,
        TurnOff,
        COUNT
    };

    Actuator() : state_("OFF") {
        // Register services (methods)
        registerService(ServiceName::TurnOn, &Actuator::turnOn);
        registerService(ServiceName::TurnOff, &Actuator::turnOff);
    }

    void turnOn() {
        std::cout << "Actuator is now ON." << std::endl;
        state_ = "ON";
    }

    void turnOff() {
        std::cout << "Actuator is now OFF." << std::endl;
        state_ = "OFF";
    }

    const std::string& getState() const {
        return state_;
    }

private:
    std::string state_;  // Actuator state (ON or OFF)
};

int main() {
    Actuator actuator;

    // Invoke the actuator services
    auto actuatorInvoker = actuator.getServiceInvoker();
    actuatorInvoker(Actuator::ServiceName::TurnOn);
    actuatorInvoker(Actuator::ServiceName::TurnOff);

    return 0;
}
