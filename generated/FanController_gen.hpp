
 #pragma once
 #include <Arduino.h>          // pinMode, analogWrite, etc.
 #include <utility>            // std::forward
 #include "eos_shared_services.hpp"
 
 class FanController {
public:
    enum class Var : uint16_t {currentRpm, powerLevel };

    enum class Svc : uint8_t {setPower, emergencyStop };

    using Shared   = SharedBase<FanController, Var>;
    using Services = ServicesBase<FanController, Svc>;

    [[nodiscard]] Shared   shared()   noexcept { return Shared{*this}; }
    [[nodiscard]] Services services() noexcept { return Services{*this}; }

    /* === API interna preservada por el usuario === */
};
 
 /* ===== Especializaciones de access_shared ===== */ 
 /* ===== Especializaciones de invoke_service ===== */ 