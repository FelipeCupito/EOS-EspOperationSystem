#ifndef EOS_CORE_HPP
#define EOS_CORE_HPP

#include <cstdint>
#include <array>
#include <type_traits>
#include <functional>
#include <Arduino.h>
#include "eos_dsl_macros.hpp" 

namespace eos {
    
//─────────── 2. Data‑store templates indexed by enum ───────────//

template<typename Enum, std::size_t N>
class SharedVarStore {
    public:
        constexpr void init(void* (&vars)[N]) noexcept { 
            vars_ = vars; 
        }
        
        template<Enum E, typename T>
        [[nodiscard]] T& get() const noexcept {
            return *static_cast<T*>(vars_[static_cast<std::size_t>(E)]);
        }
    private:
        void** vars_{}; // pointers to actual vars
};

template<typename Enum, std::size_t N>
class ServiceRegistry {
    public:
        struct Entry { 
            void (*fn)(void*); 
            void* ctx; 
        };
        
        constexpr void init(Entry (&sv)[N]) noexcept { 
            svcs_ = sv; 
        }

        template<Enum E>
        void call() const noexcept {
            auto& e = svcs_[static_cast<std::size_t>(E)];
            e.fn(e.ctx);
        }
    private:
        Entry* svcs_{};
};

// EventBus kept hash‑free by using compiler‑generated enum index too
template<typename Enum, std::size_t N>
class EventBus {
    public:
        struct Entry { 
            void (*cb)(void*); 
            void* ctx; 
        };
        
        constexpr void init(Entry (&ev)[N]) noexcept { 
            evs_ = ev; 
        }

        template<Enum E>
        void publish() const noexcept {
            evs_[static_cast<std::size_t>(E)].cb(evs_[static_cast<std::size_t>(E)].ctx);
        }
        
    private:
        Entry* evs_{};
};


struct TaskEntry { 
    void (*fn)(void*) noexcept; 
    uint32_t period; 
    uint32_t last{0}; 
    void* ctx; 
};

/** Scheduler – cooperative tick‑based periodic tasks. */
class Scheduler {
    public:
        template<std::size_t N>
        constexpr void init(TaskEntry (&t)[N]) noexcept {
            tasks_ = t; size_ = N;
        }

        void runDue() noexcept {
            const uint32_t now = millis();
            for (std::size_t i{}; i < size_; ++i)
                if (static_cast<uint32_t>(now - tasks_[i].last) >= tasks_[i].period) {
                    tasks_[i].last = now;
                    tasks_[i].fn(tasks_[i].ctx);
                }
        }

    private:
        TaskEntry* tasks_{}; std::size_t size_{};
};

//─────────── 3. Descriptor generated per peripheral ───────────//
struct PeripheralDescriptor {
    //const char* name;
    
    // Shared Var
    void** vars;  
    std::size_t nVars;

    // Service
    ServiceRegistry<int,1>::Entry* svcs; 
    std::size_t nSvcs;
    
    // Event
    EventBus<int,1>::Entry* evts; 
    std::size_t nEvts;

    // Task
    TaskEntry* tasks; 
    std::size_t nTasks;
};

// ────────────────────────────────────────────────────────────────────
//  Core façade (singleton)
// ────────────────────────────────────────────────────────────────────
class Core {
public:
    static Core& instance() noexcept { static Core c; return c; }
    void registerPeripheral(PeripheralDescriptor* d) noexcept;
    void begin() noexcept;        ///< call in Arduino setup()
    void tick() noexcept;         ///< call in Arduino loop()
private:
    Core() = default;
};

// Auto‑register helper (used in generated code)
struct AutoRegister {
    explicit constexpr AutoRegister(PeripheralDescriptor* d) noexcept {
        Core::instance().registerPeripheral(d);
    }
};



}  // namespace eos
#endif  // EOS_CORE_HPP



