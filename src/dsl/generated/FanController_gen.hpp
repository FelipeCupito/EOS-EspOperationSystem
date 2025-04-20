
 #pragma once
 #include <Arduino.h>          // pinMode, analogWrite, etc.
 #include <utility>            // std::forward
 #include "eos_shared_services.hpp"
 
 class FanController {
 public:
     
 /* ===== Variables compartidas ===== */
     enum class Var :uint16_t {         fanPin,         currentRpm,         powerLevel     };
 
     /* ===== Servicios expuestos ===== */
     enum class Svc : uint8_t {         setPower,         emergencyStop     };
 
     using Shared   = SharedBase<FanController, Var>;
     using Services = ServicesBase<FanController, Svc>;
 
     [[nodiscard]] Shared   shared()   noexcept { return Shared{*this}; }
     [[nodiscard]] Services services() noexcept { return Services{*this}; }
 
 private:     int fanPin;     int currentRpm;     uint8_t powerLevel; 
 public:
     /* ----- ctor ----- */
     explicit FanController(int fanPin, int currentRpm, uint8_t powerLevel)
         : fanPin(fanPin), currentRpm(currentRpm), powerLevel(powerLevel)
     {
         // inicialización específica (si fuese necesaria)
     }
 
     /* ----- implementación de servicios ----- */   
    void setPower(                 uint8_t power     ) {
         // TODO: cuerpo de setPower
     }     void emergencyStop(     ) {
         // TODO: cuerpo de emergencyStop
     } 
     /* ----- API para SharedBase / ServicesBase ----- */
     template<Var v>
     static decltype(auto) access_shared(FanController& p);
 
     template<Svc s, typename... Args>
     static void invoke_service(FanController& p, Args&&... args);
 };
 
 /* ===== Especializaciones de access_shared ===== */ 
 template<>
 inline decltype(auto)
 FanController::access_shared<FanController::Var::fanPin>(FanController& p)
 {     return p.fanPin; } 

 template<>
 inline decltype(auto)
 FanController::access_shared<FanController::Var::currentRpm>(FanController& p)
 {     return static_cast<const int&>(p.currentRpm); } 

 template<>
 inline decltype(auto)
 FanController::access_shared<FanController::Var::powerLevel>(FanController& p)
 {     return p.powerLevel; } 
 
 /* ===== Especializaciones de invoke_service ===== */ 
 template<>
 inline void
 FanController::invoke_service<FanController::Svc::setPower>(
     FanController& p, uint8_t&& power )
 {
     p.setPower(        std::forward<uint8_t>(power)     );
 } 
 template<>
 inline void
 FanController::invoke_service<FanController::Svc::emergencyStop>(
     FanController& p )
 {
     p.emergencyStop(     );
 } 