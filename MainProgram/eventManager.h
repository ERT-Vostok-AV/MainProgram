
#if !defined(EVENT_MANAGER_H)
#define EVENT_MANAGER_H

#include <cppQueue.h>
#include <Vector.h>

typedef Vector<double> Data;

class EventManager {

public:
    EventManager();

    bool isLiftOff(cppQueue fifo);
    bool isTouchDown(cppQueue fifo);
    bool isApogee(cppQueue fifo);
    bool isReTrigger(double alt);
    bool trigger();
    bool hasTriggered();
private:
    bool triggered;
};



#endif // EVENT_MANAGER_H
