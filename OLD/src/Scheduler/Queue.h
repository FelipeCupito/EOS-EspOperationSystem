/**
 * @file Queue.h
 * @version 1.0
 *
 * @brief Clase plantilla `Queue` para implementar una cola circular de elementos de datos.
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "Scheduler.h"

/**
 * @brief Clase plantilla para una cola circular (ring buffer) de elementos de datos.
 *
 * @tparam T Tipo de los elementos almacenados en la cola.
 * @tparam NMEMB Número de elementos en la cola (debe ser una potencia de 2).
 */
template <class T, uint8_t NMEMB>
class Queue {
    // Asegura que NMEMB es una potencia de 2
    static_assert(NMEMB && !(NMEMB & (NMEMB - 1)), "Queue::NMEMB debe ser una potencia de 2");

public:
    /**
     * @brief Constructor de la clase `Queue`.
     *        Inicializa los índices de inserción y extracción.
     */
    Queue(): m_put(0), m_get(0){}

    /**
     * @brief Obtiene el número de elementos disponibles en la cola.
     * @return Número de elementos disponibles.
     */
    unsigned int available() const {
      return ((NMEMB + m_put - m_get) & MASK);
    }

    /**
     * @brief Obtiene el espacio disponible en la cola para nuevos elementos.
     * @return Número de espacios disponibles.
     */
    unsigned int room() const {
      return ((NMEMB - m_put + m_get - 1) & MASK);
    }

    /**
     * @brief Inserta un elemento en la cola. Espera hasta que haya espacio disponible.
     * @param[in] data Puntero al elemento a insertar.
     */
    void push(const T* data) {
      await(room()); // Espera hasta que haya espacio
      unsigned int next = (m_put + 1) & MASK;
      m_buffer[next] = *data;
      m_put = next;
    }

    /**
     * @brief Inserta un elemento almacenado en la memoria de programa en la cola.
     *        Espera hasta que haya espacio disponible.
     * @param[in] data Puntero al elemento en la memoria de programa.
     */
    void push_P(const T* data){
      await(room()); // Espera hasta que haya espacio
      unsigned int next = (m_put + 1) & MASK;
      memcpy_P(&m_buffer[next], data, sizeof(T));
      m_put = next;
    }

    /**
     * @brief Extrae un elemento de la cola. Espera hasta que haya elementos disponibles.
     * @param[out] data Puntero al buffer donde se almacenará el elemento extraído.
     */
    void pull(T* data){
      await(available()); // Espera hasta que haya elementos disponibles
      unsigned int next = (m_get + 1) & MASK;
      m_get = next;
      *data = m_buffer[next];
    }

private:
    static const unsigned int MASK = (NMEMB - 1); //!< Máscara para índices circulares
    volatile unsigned int m_put;                  //!< Índice de inserción
    volatile unsigned int m_get;                  //!< Índice de extracción
    T m_buffer[NMEMB];                            //!< Buffer circular que almacena los elementos
};

#endif // QUEUE_H