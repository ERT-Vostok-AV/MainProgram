
#if !defined(EVENT_MANAGER_H)
#define EVENT_MANAGER_H

#include <Vector.h>

#include "flightData.h"


class EventManager {

public:
    EventManager();

    bool isLiftOff(double alt, double velZ);
    bool isTouchDown(double alt, double velZ);
    bool isApogee(double alt, double velZ);
    bool isReTrigger(double alt);
    void trigger();
    bool hasTriggered();
private:
    bool triggered;
	double maxAlt = 0;
};



#endif // EVENT_MANAGER_H
