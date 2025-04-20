// src/core/SystemManager.cpp

#include "SystemManager.hpp"

void SystemManager::addPeripheral(Peripheral* peripheral) {
    peripherals.push_back(peripheral);
}

void SystemManager::addProcess(Process* process) {
    processes.push_back(process);
}

Peripheral* SystemManager::getPeripheral(const std::string& name) {
    for (auto* peripheral : peripherals) {
        if (peripheral->getName() == name) {
            return peripheral;
        }
    }
    return nullptr;
}

Process* SystemManager::getProcess(const std::string& name) {
    for (auto* process : processes) {
        if (process->getName() == name) {
            return process;
        }
    }
    return nullptr;
}

void SystemManager::setup() {
    for (Peripheral* peripheral : peripherals) {
        peripheral->setup();
    }
    for (Process* process : processes) {
        process->setup();
    }
}

void SystemManager::loop() {
    for (Peripheral* peripheral : peripherals) {
        peripheral->loop();
    }
    for (Process* process : processes) {
        process->loop();
    }
}
