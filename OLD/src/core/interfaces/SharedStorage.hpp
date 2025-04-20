// // src/core/interfaces/SharedStorage.hpp

#pragma once
#include <array>
#include <type_traits>
#include <any>
#include <iostream>

// Definir el macro para registrar variables de manera más limpia
#define REGISTER_SHARED_VAR(var, access) registerVariable(static_cast<size_t>(VariableName::var), &var, access)
#define REGISTER_SHARED_VAR_READ(var) registerVariable(static_cast<size_t>(VariableName::var), &var, SharedStorage::ACCESSTYPE::READ)



// Clase base sin dependencia directa del enum en Derived
template<typename Derived, std::size_t Size>
#define COUNT_SH_VAR(var) static_cast<std::size_t>(Peripheral::VariableName::var)
class SharedStorage {
protected:
    // El array para almacenar las variables
    std::array<std::any, Size> variableArray;

public:
    // Tipos de acceso
    enum class ACCESSTYPE {
        READ,
        WRITE,
        READWRITE
    };

     SharedStorage(){
        // Verificamos que Derived tiene un enum VariableName
        static_assert(std::is_enum<typename Derived::VariableName>::value, "Derived class must define an enum VariableName");
        static_assert(Size > 0, "Size of SharedStorage must be greater than 0.");
        
    }

    template<typename T>
    constexpr void registerVariable(std::size_t variableIndex, T* variablePtr, ACCESSTYPE access) {
        // Chequeo en tiempo de compilación para asegurarnos que el índice está dentro de los límites
        static_assert(std::is_integral<std::size_t>::value, "Index must be an unsigned integer.");
        static_assert(!std::is_pointer<T>::value, "Registered variable must not be a pointer to a pointer.");
         if (variableIndex >= Size) {
            return; //TODO: Manejo correcto del errores
        }

        variableArray[variableIndex] = SharedVariable<T>(variablePtr, access);
    }

    // Clase para almacenar variables
    template<typename T>
    class SharedVariable {
    public:
        T* valuePtr;
        ACCESSTYPE access;

        SharedVariable(T* ptr, ACCESSTYPE accessType)
            : valuePtr(ptr), access(accessType) {}

        // Leer y escribir
        T read() const {
            return *valuePtr;
        }

        void write(T newValue) {
            *valuePtr = newValue;
        }
    };

    // Proveedor de acceso para leer/escribir
    class StorageProvider {
    private:
        std::array<std::any, Size>& variableArray;

    public:
        StorageProvider(std::array<std::any, Size>& array) : variableArray(array) {}

        // Leer usando el índice
        template<typename T>
        T read(size_t index) {
            if (index >= Size) {
                return T(); //TODO: Manejo correcto del errores
            }
            auto& sharedVar = std::any_cast<SharedVariable<T>&>(variableArray[index]);
            return sharedVar.read();
        }

        // Escribir usando el índice
        template<typename T>
        void write(size_t index, T newValue) {
            if (index >= Size) {
               return; //TODO: Manejo correcto del errores
            }
            auto& sharedVar = std::any_cast<SharedVariable<T>&>(variableArray[index]);
            sharedVar.write(newValue);
        }
    };

    // Proporciona un acceso seguro a las variables
    StorageProvider getProvider() {
        return StorageProvider(variableArray);
    }
};
