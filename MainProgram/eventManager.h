
#if !defined(EVENT_MANAGER_H)
#define EVENT_MANAGER_H

#include <QueueList.h>
#include <Vector.h>

#include "flightData.h"


class EventManager {

public:
    EventManager();

    bool isLiftOff(QueueList<FlightData> fifo);
    bool isTouchDown(QueueList<FlightData> fifo);
    bool isApogee(QueueList<FlightData> fifo);
    bool isReTrigger(double alt);
    bool trigger();
    bool hasTriggered();
private:
    bool triggered;
};



#endif // EVENT_MANAGER_H
