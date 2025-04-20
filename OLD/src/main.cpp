#include <Arduino.h>

// #include "./peripherals/Led.hpp"
// #include "./peripherals/TemperatureSensor.hpp"
// #include "./peripherals/sensorWithService.hpp"
// #include "./processes/CoolingSystemProcess.hpp"

// void setup() {

//   TemperatureSensor tp = TemperatureSensor("temp_sensor_1");

//   TemperatureSensorService tpService = TemperatureSensorService("temp_sensor_1");

//   CoolingSystemProcess coolingSystem = CoolingSystemProcess();

  
//   Led led = Led("led", 13);
// }

// void loop() {
// }
#include <iostream>
#include <tuple>
#include <type_traits>

// Estrategias de comunicación
class ComunicacionEventos {
public:
    void comunicar() const {
        std::cout << "Comunicando vía Eventos" << std::endl;
    }
};

class ComunicacionMemoriaCompartida {
public:
    int comunicar(int data) const {
        std::cout << "Comunicando vía Memoria Compartida: Data = " << data << std::endl;
        return data + 1;  // Simulación de procesamiento de datos
    }
};

class ComunicacionProxyApi {
public:
    std::string comunicar(const std::string& mensaje) const {
        std::cout << "Comunicando vía Proxy API: Mensaje = " << mensaje << std::endl;
        return mensaje + " [enviado]";
    }
};

// Clase Periferico que acepta un número arbitrario de estrategias de comunicación
template <typename... Estrategias>
class Periferico {
public:
    // Constructor por defecto: crea instancias por defecto de cada estrategia
    Periferico() : estrategias_{Estrategias()...} {}

    // Llamar a la función comunicar de la estrategia por tipo
    template <typename Estrategia, typename... Args>
    auto comunicar(Args&&... args) const -> decltype(auto) {
        return getEstrategia<Estrategia>().comunicar(std::forward<Args>(args)...);
    }

private:
    // Tuple para almacenar las estrategias
    std::tuple<Estrategias...> estrategias_;

    // Helper para obtener la estrategia por tipo
    template <typename Estrategia>
    const Estrategia& getEstrategia() const {
        return std::get<Estrategia>(estrategias_);
    }
};

// Ejemplo de uso
void setup() {
    // Crear un Periferico que usa Eventos, Memoria Compartida y Proxy API
    Periferico<ComunicacionEventos, ComunicacionMemoriaCompartida, ComunicacionProxyApi> perifericoCompleto;

    // Usar la comunicación por tipo y pasar argumentos necesarios
    std::cout << "Ejecutar comunicacion via Proxy API:" << std::endl;
    auto resultProxy = perifericoCompleto.comunicar<ComunicacionProxyApi>("Mensaje importante");
    std::cout << "Resultado Proxy API: " << resultProxy << std::endl;

    std::cout << "\nEjecutar comunicacion via Memoria Compartida:" << std::endl;
    int resultMemoria = perifericoCompleto.comunicar<ComunicacionMemoriaCompartida>(42);
    std::cout << "Resultado Memoria Compartida: " << resultMemoria << std::endl;

    std::cout << "\nEjecutar comunicacion via Eventos:" << std::endl;
    perifericoCompleto.comunicar<ComunicacionEventos>();  // No tiene argumentos ni retorno

}

void loop() {
  
}