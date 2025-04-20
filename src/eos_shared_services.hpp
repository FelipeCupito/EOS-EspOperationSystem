#pragma once
#include <type_traits>
#include <utility>

template<typename Peripheral, typename VarEnum>
class SharedBase {
    Peripheral& ref_;
public:
    explicit constexpr SharedBase(Peripheral& ref) noexcept : ref_(ref) {}

    // Acceso seguro a la variable v
    template<VarEnum v>
    decltype(auto) get() {        
        return Peripheral::template access_shared<v>(ref_);
    }
};

template<typename Peripheral, typename SvcEnum>
class ServicesBase {
    Peripheral& ref_;
public:
    explicit constexpr ServicesBase(Peripheral& ref) noexcept : ref_(ref) {}

    template<SvcEnum s, typename... Args>
    void call(Args&&... args) {
        Peripheral::template invoke_service<s>(ref_, std::forward<Args>(args)...);
    }
};
