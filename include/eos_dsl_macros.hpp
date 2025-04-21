#pragma once

// Macros que marcan el DSL dentro de C++.
// Se traducen a __attribute__((annotate("eos::<tag>"))).
#define peripheral   class __attribute__((annotate("eos::peripheral")))
#define SharedVar    __attribute__((annotate("eos::shared")))
#define SharedVarRO  __attribute__((annotate("eos::shared_ro")))
#define Service      __attribute__((annotate("eos::service")))
#define Periodic(ms) __attribute__((annotate("eos::periodic=" #ms)))
