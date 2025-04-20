#include "eos_core.hpp"

namespace eos {

static constexpr std::size_t kMaxVars = 32, kMaxSvcs = 32, kMaxEvts = 32, kMaxTasks = 32;
static SharedVarEntry g_vars[kMaxVars];  static std::size_t g_nv;
static ServiceEntry  g_svcs[kMaxSvcs];  static std::size_t g_ns;
static EventEntry    g_evts[kMaxEvts];  static std::size_t g_ne;
static TaskEntry     g_tasks[kMaxTasks]; static std::size_t g_nt;

void Core::registerPeripheral(PeripheralDescriptor* d) noexcept {
    // Merge tables (compile‑time sizes validated in code‑gen)
    for (std::size_t i{}; i < d->nVars;  ++i) g_vars [g_nv++] = d->vars [i];
    for (std::size_t i{}; i < d->nSvcs;  ++i) g_svcs [g_ns++] = d->svcs [i];
    for (std::size_t i{}; i < d->nEvts;  ++i) g_evts [g_ne++] = d->evts [i];
    for (std::size_t i{}; i < d->nTasks; ++i) g_tasks[g_nt++] = d->tasks[i];
}

void Core::begin() noexcept {
    shared_.init(g_vars); svc_.init(g_svcs); bus_.init(g_evts); sch_.init(g_tasks);
}

void Core::tick() noexcept { sch_.runDue(); }

}  // namespace eos