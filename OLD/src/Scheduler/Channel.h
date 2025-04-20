#ifndef CHANNEL_H
#define CHANNEL_H

#include "Scheduler.h"

class Channel {
  public:
    /**
     * @brief Constructor de la clase Channel.
     * Inicializa el canal para el paso de mensajes entre tareas.
     */
    Channel() :
      m_buf(nullptr),
      m_max(0),
      m_size(0),
      m_ticket(0),
      m_serving(0)
    {}

    /**
     * @brief Enviar un mensaje a la tarea receptora.
     *        Espera hasta que la tarea receptora esté lista antes de copiar el mensaje.
     *        Múltiples remitentes se pondrán en cola en orden.
     * @param[in] buf Puntero al buffer del mensaje.
     * @param[in] size Tamaño del mensaje en bytes.
     * @return El tamaño del mensaje enviado si es exitoso, de lo contrario un código de error negativo.
     */
    int send(const void* buf, size_t size) {
      // Tomar un ticket y esperar el turno
      uint8_t ticket = m_ticket++;
      await((ticket == m_serving) && (m_buf != nullptr));

      // Verificar si el buffer receptor puede contener el mensaje
      if (size > m_max) {
          m_serving += 1;
          return -1; // Error: el tamaño del mensaje excede la capacidad del buffer receptor
      }

      // Copiar el mensaje al buffer receptor
      memcpy(m_buf, buf, size);
      m_size = size;
      return static_cast<int>(size);
    }

    /**
     * @brief Recibir un mensaje del canal.
     *        Espera a que un remitente copie el mensaje en el buffer proporcionado.
     * @param[out] buf Puntero al buffer donde se almacenará el mensaje.
     * @param[in] size Tamaño máximo del buffer en bytes.
     * @return El tamaño del mensaje recibido.
     */
    int recv(void* buf, size_t size){
      // Esperar hasta que el canal esté libre para recibir
      await(m_buf == nullptr);

      // Configurar el buffer receptor y el tamaño máximo
      m_buf = buf;
      m_max = size;
      m_size = 0;

      // Esperar el mensaje del remitente
      await(m_size != 0);

      int receivedSize = static_cast<int>(m_size);

      // Restablecer el estado del canal
      m_buf = nullptr;
      m_max = 0;
      m_size = 0;
      m_serving += 1;

      // Devolver el tamaño del mensaje recibido
      return receivedSize;
    }

  private:
    void* m_buf;                // Puntero al buffer del mensaje
    volatile size_t m_max;      // Tamaño máximo del mensaje que se puede recibir
    volatile size_t m_size;     // Tamaño actual del mensaje recibido
    volatile uint8_t m_ticket;  // Número de ticket para las tareas remitentes
    volatile uint8_t m_serving; // Número de ticket que se está sirviendo
};

#endif // CHANNEL_H