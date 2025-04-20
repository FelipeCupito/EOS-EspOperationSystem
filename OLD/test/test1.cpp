#include <iostream>
#include <string>
#include <vector>
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

// TemperatureSensor: Tiene ExposedVars y MethodRegistry
class TemperatureSensor : public Peripheral<TemperatureSensor, MethodRegistry<3>, ExposedVars<2>> {
public:
    // Enum to identify the shared variables
    enum class VariableName {
        CurrentTemperature,
        MaxTemperature,
        COUNT
    };

    // Enum to identify the services (methods)
    enum class ServiceName {
        ReadTemperature,
        ResetTemperature,
        CheckAndTriggerActuator,
        COUNT
    };

    TemperatureSensor(Actuator& actuator) 
    : currentTemperature_(25), maxTemperature_(100), linkedActuator_(actuator) {
        // Register shared variables
        registerVariable(VariableName::CurrentTemperature, &currentTemperature_);
        registerVariable(VariableName::MaxTemperature, &maxTemperature_);

        // Register services (methods)
        registerService(ServiceName::ReadTemperature, &TemperatureSensor::readTemperature);
        registerService(ServiceName::ResetTemperature, &TemperatureSensor::resetTemperature);
        registerService(ServiceName::CheckAndTriggerActuator, &TemperatureSensor::checkAndTriggerActuator);
    }

    // Method to read temperature
    void readTemperature() {
        std::cout << "Reading temperature..." << std::endl;
        currentTemperature_ += 5;  // Simulate temperature increase
        std::cout << "Current temperature: " << currentTemperature_ << std::endl;
    }

    // Method to reset the temperature
    void resetTemperature() {
        std::cout << "Resetting temperature to 0..." << std::endl;
        currentTemperature_ = 0;
    }

    // Check temperature and trigger the actuator if it exceeds maxTemperature_
    void checkAndTriggerActuator() {
        if (currentTemperature_ > maxTemperature_) {
            std::cout << "Temperature exceeds the maximum! Triggering actuator..." << std::endl;
            linkedActuator_.getServiceInvoker()(Actuator::ServiceName::TurnOn);
        } else {
            std::cout << "Temperature is within safe limits." << std::endl;
            linkedActuator_.getServiceInvoker()(Actuator::ServiceName::TurnOff);
        }
    }

private:
    int currentTemperature_;  // Stores the current temperature
    int maxTemperature_;      // Maximum temperature recorded
    Actuator& linkedActuator_; // Reference to the Actuator to trigger actions
};

// PressureSensor: Solo tiene ExposedVars
class PressureSensor : public Peripheral<PressureSensor, ExposedVars<3>> {
public:
    // Enum to identify the shared variables
    enum class VariableName {
        CurrentPressure,
        MaxPressure,
        StatusMessage,
        COUNT
    };

    PressureSensor() : currentPressure_(1.01), maxPressure_(2.5), statusMessage_("OK") {
        // Register shared variables
        registerVariable(VariableName::CurrentPressure, &currentPressure_);
        registerVariable(VariableName::MaxPressure, &maxPressure_);
        registerVariable(VariableName::StatusMessage, &statusMessage_);
    }

    // Method to simulate pressure changes
    void simulatePressureChange(double newPressure) {
        currentPressure_ = newPressure;
        if (currentPressure_ > maxPressure_) {
            statusMessage_ = "Pressure too high!";
        } else {
            statusMessage_ = "Pressure normal.";
        }
    }

private:
    double currentPressure_;  // Stores the current pressure
    double maxPressure_;      // Maximum pressure recorded
    std::string statusMessage_; // Pressure status
};

int main() {
    /****************************
     * Test TemperatureSensor and Actuator Interaction
     ****************************/
    std::cout << "=== Testing TemperatureSensor and Actuator ===" << std::endl;
    Actuator actuator;
    TemperatureSensor tempSensor(actuator);

    // Test Shared Variables in TemperatureSensor
    auto tempViewer = tempSensor.getVarViewer();
    std::cout << "Initial temperature: " 
              << tempViewer.getVariable<int>(TemperatureSensor::VariableName::CurrentTemperature) << std::endl;

    // Test MethodRegistry in TemperatureSensor
    auto tempInvoker = tempSensor.getServiceInvoker();
    tempInvoker(TemperatureSensor::ServiceName::ReadTemperature);
    tempInvoker(TemperatureSensor::ServiceName::CheckAndTriggerActuator);

    // Check if actuator was triggered
    std::cout << "Actuator state: " << actuator.getState() << std::endl;

    tempInvoker(TemperatureSensor::ServiceName::ResetTemperature);

    // Check if reset worked
    std::cout << "Temperature after reset: " 
              << tempViewer.getVariable<int>(TemperatureSensor::VariableName::CurrentTemperature) << std::endl;

    /****************************
     * Test PressureSensor with Complex Data
     ****************************/
    std::cout << "\n=== Testing PressureSensor ===" << std::endl;
    PressureSensor pressureSensor;

    // Test Shared Variables in PressureSensor
    auto pressureViewer = pressureSensor.getVarViewer();
    std::cout << "Current pressure: " 
              << pressureViewer.getVariable<double>(PressureSensor::VariableName::CurrentPressure) << std::endl;
    std::cout << "Max pressure: " 
              << pressureViewer.getVariable<double>(PressureSensor::VariableName::MaxPressure) << std::endl;

    // Simulate a dangerous pressure
    pressureSensor.simulatePressureChange(3.0);
    std::cout << "Status message after high pressure: " 
              << pressureViewer.getVariable<std::string>(PressureSensor::VariableName::StatusMessage) << std::endl;

    /****************************
     * Test Multiple Instances and Complex Interactions
     ****************************/
    std::cout << "\n=== Testing Multiple Instances and Complex Interactions ===" << std::endl;

    // Create multiple sensors and interact with them
    TemperatureSensor tempSensor2(actuator);
    PressureSensor pressureSensor2;

    auto tempViewer2 = tempSensor2.getVarViewer();
    auto pressureViewer2 = pressureSensor2.getVarViewer();

    std::cout << "Sensor 2 - Initial temperature: " 
              << tempViewer2.getVariable<int>(TemperatureSensor::VariableName::CurrentTemperature) << std::endl;
    std::cout << "Sensor 2 - Initial pressure: " 
              << pressureViewer2.getVariable<double>(PressureSensor::VariableName::CurrentPressure) << std::endl;

    // Simulate interaction with other sensor
    tempSensor2.readTemperature();
    tempSensor2.checkAndTriggerActuator();

    return 0;
}
