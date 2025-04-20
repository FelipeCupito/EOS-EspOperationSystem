#include "eos_core.hpp"

void setup() {
    Serial.begin(115200);
    eos::Core::instance().begin();
}

void loop() {
    eos::Core::instance().tick();
}