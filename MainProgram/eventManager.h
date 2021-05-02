
#if !defined(EVENT_MANAGER_H)
#define EVENT_MANAGER_H

#include <Vector.h>

#include "flightData.h"


class EventManager {

public:
    EventManager();

    bool isLiftOff(double alt, double accelZ);
    bool isTouchDown(double alt, double accelZ);
    bool isApogee(double alt, double accelZ);
    bool isReTrigger(double alt);
    void trigger();
    bool hasTriggered();
private:
    bool triggered;
	  double maxAlt = 0;
    int stagnation_count = 0;
    double lastAtl = 0;
};
#endif // EVENT_MANAGER_H
