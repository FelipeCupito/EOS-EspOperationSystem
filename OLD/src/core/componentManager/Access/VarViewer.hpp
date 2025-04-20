#ifndef VAR_VIEWER_HPP
#define VAR_VIEWER_HPP

#include <iostream>
#include <array>
#include <functional>
#include <stdexcept>
#include <tuple>
#include <type_traits>

#include "../interfaces/IVarViewer.hpp"
//#include "Concepts.h"

template <typename Derived, std::size_t NumVariables>
class VarViewer: public IVarViewer{
public:
    VarViewer(const ExposedVars<NumVariables>& ExposedVarsInterface) 
        : ExposedVarsInterface_(ExposedVarsInterface) {}

    // Method to access a shared variable by enum and explicit type
    template <typename VarType, typename VariableEnum>
    VarType getVariable(VariableEnum varEnum) const {
        static_assert(HasVariableNameEnum<Derived>, "Error: Derived class must have an enum named 'VariableName'");
        
        return ExposedVarsInterface_.template getVariable<VarType>(static_cast<std::size_t>(varEnum));
    }

private:
    const ExposedVars<NumVariables>& ExposedVarsInterface_;  // Reference to the shared memory interface
};

#endif // VAR_VIEWER_HPP