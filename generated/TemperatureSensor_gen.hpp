#pragma once
#include "eos_core.hpp"          // núcleo, macros y typedefs

/*─────────────────────────────────────────────────────────────────────*
 *  TemperatureSensor – user code skeleton                            *
 *  (todo el cuerpo de métodos fue copiado tal cual desde tu .eos)    *
 *────────────────────────────────────────────────────────────────────*/
peripheral TemperatureSensor {
    /* ── User‑defined fields ─────────────────────────────────────── */
    Pin int pin;
    SharedVar int temperature;
    SharedVar int humidity;
    SharedVar int pressure;

    EventEnum { TooHot };

    /* ── Enums AUTOGENERADOS por el parser ───────────────────────── */
    enum class Var { temperature, humidity, pressure, COUNT };
    enum class Svc { turnOn, turnOff, COUNT };

    /* ── Constructor ─────────────────────────────────────────────── */
    TemperatureSensor(int gpio)
        : pin{gpio}, temperature{25}, humidity{50}, pressure{1013}
    {
        pinMode(pin, INPUT);
    }

    /* ── Periodic task (cada 2 s) ────────────────────────────────── */
    Periodic(2000)
    void sample() {
        // TODO: lectura real del sensor
        if (temperature > 30)
            Emit(TooHot);
    }

    /* ── Services ────────────────────────────────────────────────── */
    Service void turnOn()  { digitalWrite(pin, HIGH); }
    Service void turnOff() { digitalWrite(pin, LOW);  }
};

/*─────────────────────────────────────────────────────────────────────*
 *  Código GENERADO: tablas estáticas + Descriptor                    *
 *  (el parser inserta esto justo debajo de la clase anterior)        *
 *────────────────────────────────────────────────────────────────────*/

namespace gen {

    /* --- 2. Service wrappers ------------------------------------- */
    namespace _svc {
        inline void turnOn (void* ctx) { static_cast<TemperatureSensor*>(ctx)->turnOn(); }
        inline void turnOff(void* ctx) { static_cast<TemperatureSensor*>(ctx)->turnOff(); }
    }

    /* --- 3. Periodic task wrapper -------------------------------- */
    inline void _task_sample(void* ctx) { static_cast<TemperatureSensor*>(ctx)->sample(); }


    /* --- 5. Static tables ---------------------------------------- */
    /*  Nota: los tamaños vienen dados por Var::COUNT, Svc::COUNT…  */
    inline void* vars[static_cast<size_t>(TemperatureSensor::Var::COUNT)] = {
        &reinterpret_cast<TemperatureSensor*>(0)->temperature,
        &reinterpret_cast<TemperatureSensor*>(0)->humidity,
        &reinterpret_cast<TemperatureSensor*>(0)->pressure
    };

    inline eos::ServiceRegistry<TemperatureSensor::Svc,
            static_cast<size_t>(TemperatureSensor::Svc::COUNT)>::Entry svcs[] = {
        { _svc::turnOn,  nullptr },   // se rellena el ctx en runtime
        { _svc::turnOff, nullptr }
    };

    inline eos::EventBus<TemperatureSensor::EventEnum, 1>::Entry evts[] = {
        { _evt::noop, nullptr }       // placeholder (TooHot no necesita callback interno)
    };

    inline eos::TaskEntry tasks[] = {
        { _task_sample, 2000, 0, nullptr }
    };

    /* --- 6. Descriptor + autoregistro ---------------------------- */
    inline eos::PeripheralDescriptor desc {
        "TemperatureSensor",
        vars,  std::size(vars),
        svcs,  std::size(svcs),
        evts,  std::size(evts),
        tasks, std::size(tasks)
    };

    tatic eos::AutoRegister _auto{ &desc };   // se ejecuta antes de setup()

} // namespace gen
