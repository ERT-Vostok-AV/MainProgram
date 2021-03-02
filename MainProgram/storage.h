#if !defined(STORAGE_H)
#define STORAGE_H
#include <Vector.h>
#include "flightData.h"

typedef Vector<FlightData> Buffer;

class Storage {

public: 
    Storage();

    bool logFlightInfo(unsigned long liftOffTime, unsigned long apogeeTime, unsigned longreTriggerTime, unsigned long touchdownTime);
    bool saveSD(const Buffer& buffer);
    bool sdTransfer();
private: 
};


#endif // STORAGE_H
