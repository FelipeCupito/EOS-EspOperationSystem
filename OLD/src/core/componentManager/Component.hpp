// src/core/interfaces/Component.hpp

#pragma once
#include <Arduino.h>

class Component {
protected:
    String name;
    bool active = true;

public:
    Component(const char* name) : name(name) {}

    virtual void setup() = 0;
    virtual void loop() = 0;

    String getName() const {
        return name;
    }

    bool isActive() const {
        return active;
    }

    void setActive(bool isActive) {
        active = isActive;
    }
};

