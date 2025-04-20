// IServiceInvoker.hpp
#ifndef ISERVICE_INVOKER_HPP
#define ISERVICE_INVOKER_HPP

class IServiceInvoker {
public:
    virtual ~IServiceInvoker() = default;

    // Métodos plantilla no virtuales que aceptan enums directamente
    template<typename EnumType>
    int invoke(EnumType serviceEnum) {
        return invokeImpl(static_cast<std::size_t>(serviceEnum), nullptr);
    }

    template<typename EnumType, typename ArgType>
    int invoke(EnumType serviceEnum, ArgType& arg) {
        return invokeImpl(static_cast<std::size_t>(serviceEnum), &arg);
    }

protected:
    // Método virtual protegido que implementa la lógica interna
    virtual int invokeImpl(std::size_t serviceEnum, void* arg) = 0;
};

#endif // ISERVICE_INVOKER_HPP