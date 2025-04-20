#ifndef EOS_DSL_MACROS_HPP
#define EOS_DSL_MACROS_HPP

#include <Arduino.h>

#define peripheral         class [[eos::peripheral]]
#define SharedVar          [[eos::shared]]          // marks a field
#define SharedVarRO       [[eos::shared_ro]]       // marks a field
#define Service            [[eos::service]]         // marks a method
#define Events          [[eos::event_enum]] enum // inside peripheral
#define Periodic(ms)       [[eos::periodic(ms)]]

// Extras already present
#define Pin                [[eos::pin]]
#define Route              [[eos::route]]
#define Board              [[eos::board]]
#define Include(x)         [[eos::include(x)]]
#define Emit(x)            [[eos::emit(x)]]

#endif // EOS_DSL_MACROS_HPP