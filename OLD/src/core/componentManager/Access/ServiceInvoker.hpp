// ServiceInvoker.hpp
#ifndef SERVICE_INVOKER_HPP
#define SERVICE_INVOKER_HPP

template<typename Derived>
class ServiceInvoker {
public:
    ServiceInvoker(Derived* instance) : instance_(instance) {}

    // Invocar un servicio con argumento específico
    template<typename ArgType>
    int invoke(typename Derived::ServiceName serviceEnum, ArgType& arg) {
        return instance_->invokeService(
            static_cast<std::size_t>(serviceEnum),
            instance_,
            &arg
        );
    }

    // Invocar un servicio sin argumento
    int invoke(typename Derived::ServiceName serviceEnum) {
        return instance_->invokeService(
            static_cast<std::size_t>(serviceEnum),
            instance_,
            nullptr
        );
    }

    // Invocar un servicio con argumento void*
    int invoke(typename Derived::ServiceName serviceEnum, void* arg) {
        return instance_->invokeService(
            static_cast<std::size_t>(serviceEnum),
            instance_,
            arg
        );
    }

private:
    Derived* instance_;
};

#endif // SERVICE_INVOKER_HPP