#pragma once
#include <type_traits>
#include <utility>

// Base para manejar SharedVar… accesibles por .shared().get<Var>()
template<typename Peripheral, typename VarEnum>
class SharedBase {
    Peripheral& ref_;
public:
    explicit constexpr SharedBase(Peripheral& ref) noexcept : ref_(ref) {}

    template<VarEnum v>
    decltype(auto) get() {
        //static_assert(v != VarEnum::COUNT, "VarEnum::COUNT no es válido");
        return Peripheral::template access_shared<v>(ref_);
    }
};

// Base para invocar servicios type‑safe por .services().call<Svc>(…)
template<typename Peripheral, typename SvcEnum>
class ServicesBase {
    Peripheral& ref_;
public:
    explicit constexpr ServicesBase(Peripheral& ref) noexcept : ref_(ref) {}

    template<SvcEnum s, typename... Args>
    void call(Args&&... args) {
        //static_assert(s != SvcEnum::COUNT, "SvcEnum::COUNT no es válido");
        Peripheral::template invoke_service<s>(ref_, std::forward<Args>(args)...);
    }
};
