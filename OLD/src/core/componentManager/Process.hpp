// src/core/interfaces/Process.hpp

#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "Component.hpp"
//#include "../Core.hpp"


class Process : public Component {
protected:
    int priority;

public:
    Process(const char* name, int priority) : Component(name), priority(priority) {
        //Core::getInstance().registerProcess(this);
    }

    virtual void setup() = 0;
    virtual void loop() = 0;

    int getPriority() const {
        return priority;
    }
};

#endif // PROCESS_HPP