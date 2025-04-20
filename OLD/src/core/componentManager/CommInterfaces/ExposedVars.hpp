
#ifndef EXPOSED_VARS_HPP
#define EXPOSED_VARS_HPP

#include <array>

/**
 * @file ExposedVars.cpp
 * @brief Manages shared variables that are exposed by a Peripheral.
 * 
 * This class allows registering variables as shared memory within a peripheral
 * and provides a way to retrieve them safely by copying.
 * 
 * @tparam NumVariables The number of shared variables to manage.
 */
template<std::size_t NumVariables>
class ExposedVars {
public:
     /**
     * @struct Variable
     * Struct to store a pointer to the data and the size of the data type.
     */
    struct Variable {
        const void* ptr;
        std::size_t size;
    };

    /**
     * @brief Constructor that initializes the variable array with null pointers and size 0.
     */
    constexpr ExposedVars() : variables_{} {
        static_assert(NumVariables > 0, "The number of shared variables must be greater than 0");
    }

    /**
     * @brief Registers a shared variable.
     * 
     * @tparam VarType The type of the variable being shared.
     * @param idx The index at which to register the variable.
     * @param varPtr Pointer to the variable to be shared.
     */
    template <typename VarType>
    constexpr void registerVariable(std::size_t idx, const VarType* varPtr) const {
        if (idx >= NumVariables) {
            //throw std::out_of_range("Index out of range for registering the shared variable");
            //TODO: Manejo de errores
        }
        variables_[idx] = { static_cast<const void*>(varPtr), sizeof(VarType) };
    }

    /**
     * @brief Retrieves a shared variable by index and returns a copy.
     * 
     * @tparam VarType The type of the variable being retrieved.
     * @param idx The index of the variable to retrieve.
     * @return A copy of the variable.
     */
    template <typename VarType>
    VarType getVariable(std::size_t idx) const {
        if (idx >= NumVariables || !variables_[idx].ptr) {
            //throw std::out_of_range("Index out of range or variable not registered");
            //TODO: Manejo de errores
        }

        // Cast the pointer back to the correct type and return the value
        return *static_cast<const VarType*>(variables_[idx].ptr);
    }

private:
    // Array storing shared variables (void* and size)
    mutable std::array<Variable, NumVariables> variables_;
};

#endif // EXPOSED_VARS_HPP