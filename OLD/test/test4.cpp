#include <iostream>
#include <string>
#include <functional>
#include <array>
#include "Peripheral.hpp"

struct Config {
    int mode;
    double sensitivity;
};

// Definición de la clase Sensor que extiende Peripheral
class Sensor : public Peripheral<Sensor, MethodRegistry<5>> {
public:
    enum class ServiceName {
        SetThreshold,
        UpdateReading,
        SetLabel,
        IncrementCounter,
        Configure,  // Método con void*
        COUNT
    };

    Sensor() : threshold_(0.0f), reading_(0.0f), label_(""), counter_(0), config_{0, 0.0} {
        // Registrar los servicios en el MethodRegistry
        registerService<float>(ServiceName::SetThreshold, &Sensor::setThreshold);
        registerService(ServiceName::UpdateReading, &Sensor::updateReading);
        registerService<const char*>(ServiceName::SetLabel, &Sensor::setLabel);
        registerService<int>(ServiceName::IncrementCounter, &Sensor::incrementCounter);
        
        // Registrar método que utiliza void*
        registerService(ServiceName::Configure, &Sensor::configureVoidPtr);
    }

    // Método para establecer el umbral (threshold)
    int setThreshold(float threshold) {
        threshold_ = threshold;
        std::cout << "Threshold set to: " << threshold_ << std::endl;
        return 0;
    }

    // Método para actualizar la lectura
    int updateReading() {
        reading_ = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100));
        std::cout << "Reading updated to: " << reading_ << std::endl;
        return 0;
    }

    // Método para establecer una etiqueta
    int setLabel(const char* label) {
        label_ = label;
        std::cout << "Label set to: " << label_ << std::endl;
        return 0;
    }

    // Método para configurar el sensor (versión con void*)
    int configureVoidPtr(void* cfgPtr) {
        //IMPORTANTE: Realizar la conversión de void* a Config*
        Config* cfg = static_cast<Config*>(cfgPtr);
        config_ = *cfg;
        std::cout << "Sensor configured. Mode: " << config_.mode << ", Sensitivity: " << config_.sensitivity << std::endl;
        return 0;
    }

    // Método para incrementar un contador interno
    int incrementCounter(int increment) {
        counter_ += increment;
        std::cout << "Counter incremented by " << increment << ". New value: " << counter_ << std::endl;
        return 0;
    }

    // Métodos para verificar el estado interno (para pruebas)
    float getThreshold() const { return threshold_; }
    float getReading() const { return reading_; }
    std::string getLabel() const { return label_; }
    int getCounter() const { return counter_; }
    Config getConfig() const { return config_; }

private:
    float threshold_;
    float reading_;
    std::string label_;
    int counter_;
    Config config_;
};






int main() {
    Sensor mySensor;

    // Obtenemos el invocador de servicios
    auto serviceInvoker = mySensor.getServiceInvoker();

    // Probar establecer el umbral
    std::cout << "\n--- Setting Threshold ---" << std::endl;
    float threshold = 75.5f;
    serviceInvoker.invoke(Sensor::ServiceName::SetThreshold, threshold);
    assert(mySensor.getThreshold() == 75.5f);

    // Probar actualizar la lectura
    std::cout << "\n--- Updating Reading ---" << std::endl;
    serviceInvoker.invoke(Sensor::ServiceName::UpdateReading);
    float reading = mySensor.getReading();
    assert(reading >= 0.0f && reading <= 100.0f);

    // Probar establecer una etiqueta
    std::cout << "\n--- Setting Label ---" << std::endl;
    const char* label = "Temperature Sensor";
    serviceInvoker.invoke(Sensor::ServiceName::SetLabel, label);
    assert(mySensor.getLabel() == label);

        // Probar configurar el sensor (usando void*)
    std::cout << "\n--- Configuring Sensor ---" << std::endl;
    Config cfg = {1, 0.85};
    serviceInvoker.invoke(Sensor::ServiceName::Configure, &cfg);  // Usamos void*
    Config currentCfg = mySensor.getConfig();
    assert(currentCfg.mode == cfg.mode && currentCfg.sensitivity == cfg.sensitivity);


    // Probar incrementar el contador
    std::cout << "\n--- Incrementing Counter ---" << std::endl;
    int increment = 5;
    serviceInvoker.invoke(Sensor::ServiceName::IncrementCounter, increment);
    assert(mySensor.getCounter() == 5);
    increment = 3;
    serviceInvoker.invoke(Sensor::ServiceName::IncrementCounter, increment);
    assert(mySensor.getCounter() == 8);

    // Confirmar que las instancias originales se modificaron correctamente
    std::cout << "\nAll tests passed successfully." << std::endl;

    return 0;
}