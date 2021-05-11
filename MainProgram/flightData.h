#if !defined(FLIGHT_DATA_H)
#define FLIGHT_DATA_H

#include "packetSR.h"

struct FlightData {
    double batteryLevel;
    double altitude;
    double temperature;
    
    Event event;
    unsigned long timestamp;

    //accelX, accelY, accelZ
    double acceleration[3];
    //rotA, rotX, rotY, rotZ
    double rotation[4];
};


#endif // FLIGHT_DATA_H
