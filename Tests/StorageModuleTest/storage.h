#if !defined(STORAGE_H)
#define STORAGE_H

#include <SPI.h>
#include <SD.h>
#include <Vector.h>
#include "flightData.h"

typedef Vector<FlightData> Buffer;

class Storage {

public: 
    Storage();

    //init sd card, create and open a data file
    int begin();

    bool initLog();

    void test();

    bool saveSD(const Buffer& buffer);

    //log last data info and closes the data file
    bool logFlightInfo(unsigned long liftOffTime, unsigned long apogeeTime, unsigned long reTriggerTime, unsigned long touchdownTime);
private:
    File dataFile;
    char separator = '\t';
};


#endif // STORAGE_H
