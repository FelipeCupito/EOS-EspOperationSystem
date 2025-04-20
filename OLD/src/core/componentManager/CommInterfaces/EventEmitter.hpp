#ifndef EVENTEMITTER_HPP
#define EVENTEMITTER_HPP

/**
 * @file EventEmitter.cpp
 * @brief Simple class to signify that a Peripheral can emit events.
 * 
 * This class is used as a marker for Peripheral classes that can emit events. 
 * The actual event-handling logic will be implemented elsewhere.
 */

class EventEmitter {
public:
    EventEmitter() {}
};

#endif // EVENTEMITTER_HPP