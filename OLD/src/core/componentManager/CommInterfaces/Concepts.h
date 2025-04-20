#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP

#include <type_traits>

/**
 * @file Concepts.hpp
 * @brief Defines several C++20 concepts to enforce compile-time checks on types.
 * 
 * This file includes concepts that help verify if certain types or enums are present in derived classes,
 * and ensure that communication interfaces are not duplicated. These checks allow compile-time safety
 * and prevent logical errors from occurring at runtime.
 **/

/**
 * @concept IsEnum
 * Verifies if a type is an enum.
 */
template<typename T>
concept IsEnum = std::is_enum_v<T>;

/**
 * @concept HasEventTypeEnum
 * Verifies that a class contains an enum called EventType.
 */
template<typename T>
concept HasEventTypeEnum = requires {
    typename T::EventType;
    std::is_enum_v<typename T::EventType>;  
};

/**
 * @concept HasVariableNameEnum
 * Verifies that a class contains an enum called VariableName.
 */
template<typename T>
concept HasVariableNameEnum = requires {
    typename T::VariableName;
    std::is_enum_v<typename T::VariableName>;  
};

/**
 * @concept HasServiceNameEnum
 * Verifies that a class contains an enum called ServiceName.
 */
template<typename T>
concept HasServiceNameEnum = requires {
    typename T::ServiceName;
    std::is_enum_v<typename T::ServiceName>;  
};

/**
 * @concept HasCommInterfaces
 * Checks if MethodRegistry or ExposedVars is present in the communication interfaces.
 */
template<typename T, typename... CommInterfaces>
concept HasCommInterfaces = (std::is_same_v<T, CommInterfaces> || ...);

/**
 * @concept unique_types
 * Ensures that there are no repeated types in the communication interfaces.
 */
template <typename...>
struct unique_types;

template <>
struct unique_types<> : std::true_type {};

template <typename T, typename... Rest>
struct unique_types<T, Rest...> : std::conjunction<
    std::negation<std::is_same<T, Rest>>..., unique_types<Rest...>> {};

#endif // CONCEPTS_HPP
