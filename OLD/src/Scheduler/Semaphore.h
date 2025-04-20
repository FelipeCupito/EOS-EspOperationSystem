#ifndef SCHEDULER_SEMAPHORE_H
#define SCHEDULER_SEMAPHORE_H

#include <Scheduler.h>

class Semaphore {
public:
  /**
   * Initiate semaphore with given count.
   * @param[in] count (default mutex).
   */
  Semaphore(unsigned int count = 1) : m_count(count) {}

  /**
   * Wait for semaphore count.
   * @param[in] count (default mutex).
   */
  void wait(unsigned int count = 1){
    await(m_count >= count);
    m_count -= count;
  }

  /**
   * Signal semaphore count.
   * @param[in] count (default mutex).
   */
  void signal(unsigned int count = 1)
  {
    m_count += count;
    yield();
  }

protected:
  volatile unsigned int m_count;
};

#endif
