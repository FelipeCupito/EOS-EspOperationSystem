/**
 * @file Task.h
 * @brief Clase base para las tareas del scheduler.
 *
 * Esta clase define la interfaz que todas las tareas deben implementar.
 */

#ifndef TASK_H
#define TASK_H

class Task {
public:
    virtual ~Task() {}

    /**
     * @brief Método de inicialización de la tarea.
     *
     * Este método se llama una vez al inicio de la tarea.
     */
    virtual void setup() = 0;

    /**
     * @brief Método que se ejecuta en cada ciclo de la tarea.
     *
     * Este método se llama repetidamente mientras la tarea esté activa.
     */
    virtual void loop() = 0;
};

#endif // TASK_H
