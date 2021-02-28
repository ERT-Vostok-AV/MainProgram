
#if !defined(EVENT_MANAGER_H)
#define EVENT_MANAGER_H

#include <QueueList.h>
#include <Vector.h>


class EventManager {

public:
    EventManager();

    bool isLiftOff(QueueList<double*> fifo);
    bool isTouchDown(QueueList<double*> fifo);
    bool isApogee(QueueList<double*> fifo);
    bool isReTrigger(double alt);
    bool trigger();
    bool hasTriggered();
private:
    bool triggered;
};



#endif // EVENT_MANAGER_H
