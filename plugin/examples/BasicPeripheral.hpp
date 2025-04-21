// plugin/examples/BasicPeripheral.hpp
#pragma once
#include "../../include/eos_dsl_macros.hpp"

peripheral BasicPeripheral {
    SharedVarRO int foo;
    SharedVar   uint8_t bar;

    Service void baz(uint8_t x);
};
