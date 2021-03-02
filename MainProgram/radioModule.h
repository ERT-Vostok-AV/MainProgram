#if !defined(RADIO_MODULE_H)
#define RADIO_MODULE_H

#include <Vector.h>
#include "flightData.h"

class RadioModule {

public:
    bool begin();

    bool pack(FlightData data);
    bool send();
    bool packSend(FlightData data);
private:
    String packedData;
};

#endif // RADIO_MODULE_H
