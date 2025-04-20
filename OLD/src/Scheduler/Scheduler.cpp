#include "Scheduler.h"
#include <Arduino.h>

// ESP8266-specific includes and definitions
extern "C" {
  #include "os_type.h"
  #include "user_interface.h"
}
#include "Semaphore.h"

// Configuración específica para el ESP8266
#define LOOP_TASK_PRIORITY 1
#define LOOP_QUEUE_SIZE    1

static uint32_t taskStartMicros = 0;

extern "C" void espYield() { yield(); }
extern "C" void optimisticYield(uint32_t intervalUs) { yield(); }

// Implementación de la función de delay con soporte para el watchdog
extern "C" void delay(unsigned long intervalMs) {
    uint32_t startTime = system_get_time();
    while ((system_get_time() - startTime) < intervalMs * 1000) {
        system_soft_wdt_feed(); // Alimente el watchdog
        yield();
    }
}

// Implementación de la función delayUntil
extern "C" void delayUntil(uint32_t targetClock) {
    uint32_t currentClock;
    while ((currentClock = system_get_time()) < targetClock) {
        system_soft_wdt_feed(); // Alimente el watchdog
        yield();
    }

    if ((currentClock - targetClock) > 100) {
        // Imprime un mensaje de advertencia si se pierde el deadline
        #ifdef DEBUG_SCHEDULER
        Serial.println("Deadline missed");
        #endif
    }
}

// Devuelve el tiempo actual del sistema
extern "C" unsigned int getClock() {
    return system_get_time();
}

// Tarea principal del loop
extern "C" void mainLoopTask(os_event_t *events) {
    static bool firstRun = true;
    taskStartMicros = system_get_time();

    if (firstRun) {
        firstRun = false;
        preLoopUpdateFrequency(); // Actualiza la frecuencia antes del loop
        Scheduler.initialize(0x0);
        setup(); // Llama a la función de setup del usuario
        if (!Scheduler.startTask(NULL, loop, 1024)) {
            panic(); // Manejo de error si no se puede iniciar el task principal
        }
    }
    yield();
    ets_post(LOOP_TASK_PRIORITY, 0, 0);
}

// Semaphore para el manejo de SPIFFS en el ESP8266
static Semaphore spiffsSemaphore(1);

extern "C" void lockSPIFFS(uint32_t* fs) {
    spiffsSemaphore.wait(1);
}

extern "C" void unlockSPIFFS(uint32_t* fs) {
    spiffsSemaphore.signal(1);
}

// Inicialización de variables estáticas
TaskScheduler::Task mainTask = { &mainTask, &mainTask, { 0 }, NULL };
TaskScheduler::Task* TaskScheduler::currentTask = &mainTask;
size_t TaskScheduler::taskStackTop = TaskScheduler::DEFAULT_STACK_SIZE;
size_t TaskScheduler::taskCount = 0;

// Inicializa el scheduler con un tamaño de stack especificado
bool TaskScheduler::initialize(size_t stackSize) {
    static bool initialized = false; // Estado de inicialización

    if (!initialized) {
        initialized = true; // Marcar como inicializado
        taskStackTop = stackSize;

        // Inicializa la memoria del stack con un patrón para debugging
        uint8_t *stackStart = (uint8_t*)RAMEND - STACK_MAX;
        memset(stackStart, 0x08, STACK_MAX - stackSize);
    }

    return true;
}

// Inicia una nueva tarea con funciones de setup y loop
bool TaskScheduler::startTask(TaskFunction setupFunc, TaskFunction loopFunc, size_t stackSize) {
    if ((currentTask != &mainTask) || (loopFunc == NULL)) {
        return false; // Validación de condiciones iniciales
    }

    // Ajusta el tamaño del stack para incluir el contexto de la tarea
    stackSize += sizeof(Task);
    size_t frameSize = RAMEND - (size_t)&frameSize;

    // Reserva memoria para el stack de la tarea
    uint8_t taskStack[taskStackTop - frameSize];
    if (mainTask.stack == NULL) {
        mainTask.stack = taskStack;
    }

    // Verifica que el stack no exceda el tamaño máximo
    if (taskStackTop + stackSize > STACK_MAX) {
        return false; // No hay suficiente espacio en el stack
    }

    taskStackTop += stackSize; // Actualiza el tope del stack
    taskCount++; // Incrementa el contador de tareas

    // Inicializa la tarea con las funciones y el stack asignado
    initializeTask(setupFunc, loopFunc, taskStack - stackSize);
    return true;
}

// Realiza un cambio de contexto a la siguiente tarea
void TaskScheduler::yield() {
    if (setjmp(currentTask->context)) {
        return; // Guarda el contexto actual y retorna si fue llamado por longjmp
    }

    // Pasa al siguiente task en la cola
    currentTask = currentTask->next;
    longjmp(currentTask->context, true); // Salta al contexto de la siguiente tarea
}

// Devuelve el uso actual del stack de la tarea en ejecución
size_t TaskScheduler::getStackUsage() {
    unsigned char stackMarker;
    return (&stackMarker - currentTask->stack);
}

// Inicializa una tarea con funciones de setup y loop, y su stack
void TaskScheduler::initializeTask(TaskFunction setupFunc, TaskFunction loopFunc, const uint8_t* stackTop) {
    Task newTask;
    newTask.next = &mainTask; // Apunta al main task como el siguiente en la cola
    newTask.previous = mainTask.previous; // Apunta al anterior de la cola
    mainTask.previous->next = &newTask; // Ajusta los punteros en la lista
    mainTask.previous = &newTask;
    newTask.stack = stackTop;

    // Crea el contexto de la nueva tarea
    if (setjmp(newTask.context)) {
        if (setupFunc != NULL) setupFunc(); // Ejecuta la función de setup si está definida
        while (1) loopFunc(); // Ejecuta la función de loop indefinidamente
    }
}

// Función de yield global para el ESP8266
extern "C" void yield() {
    Scheduler.yield();
}
