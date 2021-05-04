#if !defined(STORAGE_H)
#define STORAGE_H

#include <SPI.h>
#include <SD.h>
#include <Vector.h>
#include "flightData.h"
#include "packetSR.h"

typedef Vector<FlightData> Buffer;

class Storage {

public: 
    Storage();

    int begin();

    bool saveSD(const Buffer& buffer);

    //log last data info and closes the data file
    bool logFlightInfo(unsigned long liftOffTime, unsigned long apogeeTime, unsigned long reTriggerTime, unsigned long touchdownTime);
    bool logEvent(Event event);
private:
    File dataFile;
    char separator = '\t';
};


#endif // STORAGE_H
