#include <iostream>
#include "Peripheral.cpp"

// PressureSensor: Solo tiene ExposedVars
class PressureSensor : public Peripheral<PressureSensor, ExposedVars<2>> {
public:
    //Enum to identify the shared variables
    enum class VariableName {
        CurrentPressure,
        MaxPressure,
        COUNT
    };

    PressureSensor() : currentPressure_(1.01), maxPressure_(2.5) {
        // Register shared variables (asegurándonos de que se registre correctamente)
        registerVariable(VariableName::CurrentPressure, &currentPressure_);
        registerVariable(VariableName::MaxPressure, &maxPressure_);
    }

private:
    double currentPressure_;  // Stores the current pressure
    double maxPressure_;      // Maximum pressure recorded
};

int main() {
    std::cout << "=== Testing PressureSensor ===" << std::endl;

    PressureSensor pressureSensor;

    // Obtener el visor para las variables del sensor de presión
    auto pressureViewer = pressureSensor.getVarViewer();

    // Acceder a las variables especificando el tipo explícitamente
    std::cout << "Current pressure: " 
              << pressureViewer.getVariable<double>(PressureSensor::VariableName::CurrentPressure) << std::endl;
    std::cout << "Max pressure: " 
              << pressureViewer.getVariable<double>(PressureSensor::VariableName::MaxPressure) << std::endl;

    return 0;
}
