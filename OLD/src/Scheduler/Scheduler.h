#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <setjmp.h>
#include <stddef.h>
#include <stdint.h>

//#define DEBUG_SCHEDULER 1 // Uncomment to enable debug messages

// Platform-specific configurations
extern "C" unsigned int getClock();
extern "C" void delayUntil(uint32_t clockMs);

class TaskScheduler {
public:
    typedef void (*TaskFunction)(); // Function pointer type for task setup and loop functions

    // Initialize the scheduler with a given stack size. Should be called before starting any tasks.
    static bool initialize(size_t stackSize);

    // Start a new task with setup and loop functions, and assign a stack size to it.
    static bool startTask(TaskFunction setupFunc, TaskFunction loopFunc, size_t stackSize = DEFAULT_STACK_SIZE);

    // Switch context to the next task.
    static void yield();

    // Return the current task's stack usage.
    static size_t getStackUsage();

    // Return the total stack size.
    static size_t* getStackSize() { return (size_t*) taskStackTop; };

    // Return the number of tasks currently running.
    static size_t getTaskCount() { return taskCount; };

protected:
    // Initialize a task with its setup and loop functions and stack.
    static void initializeTask(TaskFunction setupFunc, TaskFunction loopFunc, const uint8_t* stackTop);

    // Structure for holding task information.
    struct Task {
        Task* next;         // Pointer to the next task in the queue
        Task* previous;     // Pointer to the previous task
        jmp_buf context;    // Context for the task (for switching)
        const uint8_t* stack; // Pointer to the stack assigned to the task
    };

    static const size_t DEFAULT_STACK_SIZE = 512;
    static const size_t STACK_MAX = 0x2000 + 0x4000 + 0x2000;

    static Task mainTask;        // Main task
    static Task* currentTask;    // Currently running task
    static size_t taskStackTop;  // Stack allocation pointer
    static size_t taskCount;     // Number of tasks
};

#define Scheduler getScheduler()
extern TaskScheduler& getScheduler();

// Busy-wait for a condition using the scheduler
#define await(condition) while (!(condition)) yield()

#endif
