#ifndef IPERIPHERAL_INTERFACE_H
#define IPERIPHERAL_INTERFACE_H

#include <Arduino.h>
#include "../access/ServiceInvoker.hpp"
#include "../Component.hpp"

class IPeripheralInterface: public Component {

public:
    IPeripheralInterface(const char* name) : Component(name) {}

    virtual ~IPeripheralInterface() = default;

    // Método virtual puro para invocar servicios
    template <typename Derived>
    ServiceInvoker<Derived> getServiceInvoker();

    virtual auto getVarViewer() = 0;

    // Método virtual puro para obtener el VarViewer
    // virtual IVarViewer getVarViewer() = 0;

    // //TODO evetos
};

#endif // IPERIPHERAL_INTERFACE_H
