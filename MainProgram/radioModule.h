#if !defined(RADIO_MODULE_H)
#define RADIO_MODULE_H

#include <XBee.h>
#include "flightData.h"
#include "packetSR.h"

class RadioModule {

public:
    RadioModule();
    int begin();

    bool pack(Event event, FlightData data);
    bool send();
    bool packSend(Event event, FlightData data);

    XBee getXbee() {return xbee;};
    ZBTxStatusResponse getTxStatus() {return txStatus;};

private:
    SRHeader header;
    Payload payload;
    
    XBeeAddress64 addr64;
    ZBTxRequest zbTx;
    ZBTxStatusResponse txStatus;
    XBee xbee;

};

#endif // RADIO_MODULE_H
