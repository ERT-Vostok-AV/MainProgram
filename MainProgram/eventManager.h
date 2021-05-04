
#if !defined(EVENT_MANAGER_H)
#define EVENT_MANAGER_H

#include <Vector.h>

#include "flightData.h"


class EventManager {

public:
    EventManager();

    bool isLiftOff(double alt);
    bool isTouchDown(double alt);
    bool isApogee(double alt);
    bool isReTrigger(double alt);
    void trigger();
    bool hasTriggered();
private:
    bool triggered = false;
	  double maxAlt = 0;
    int stagnation_count = 0;
    double lastAlt = -2058.0; //out of bound to make sure it couldn't have been produced by the bmp 
};
#endif // EVENT_MANAGER_H
