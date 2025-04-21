#include "../include/eos_dsl_macros.hpp"
#include <cstdint>


peripheral FanController {
    int fanPin;
    SharedVarRO int currentRpm;     // Solo lectura para el usuario
    SharedVar uint8_t powerLevel;   // Lectura y escritura

    FanController(int pin)
        : fanPin(pin), currentRpm(0), powerLevel(0) {
        //pinMode(fanPin, OUTPUT);
    }

    void internalUpdateRpm(int rpm) { 
        currentRpm = rpm; 
    }

    Service void setPower(uint8_t power) {
        powerLevel = power;
        //analogWrite(fanPin, powerLevel);
    }

    Service void emergencyStop() {
        powerLevel = 0;
        //analogWrite(fanPin, 0);
    }
};