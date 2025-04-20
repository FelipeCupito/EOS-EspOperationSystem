// src/core/interfaces/Peripheral.hpp

#pragma once
#include "Component.hpp"
#include "../Core.hpp"

class Core;

class Peripheral : public Component {
protected:
    bool active = true;

public:

    Peripheral(const std::string& name) : Component(name) {
        Core::getInstance().registerPeripheral(this);
    }

    virtual void setup() = 0;
    virtual void loop() = 0;

    bool isActive() const {
        return active;
    }

    void setActive(bool isActive) {
        active = isActive;
    }
};
