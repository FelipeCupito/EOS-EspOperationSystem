#ifndef IVAR_VIEWER_HPP
#define IVAR_VIEWER_HPP

class IVarViewer {
public:
    virtual ~IVarViewer() = default;

    // Método virtual para obtener una variable genérica por su enum
    virtual void* getVariable(void* varEnum) const = 0;
};

#endif // IVAR_VIEWER_HPP
