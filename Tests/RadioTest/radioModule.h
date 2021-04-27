#if !defined(RADIO_MODULE_H)
#define RADIO_MODULE_H

#include <XBee.h>
#include "flightData.h"
#include "packetSR.h"
#include <SoftwareSerial.h>

#define PAKCET_SIZE 53 // 18 byte for the frame and 35 for the payload
#define PACKET_SIZE_HEX 0x35 // 53 in decimal
#define RX_PIN 34
#define TX_PIN 35
#define XBEE_FREQ 115200

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
    uint8_t packet[PAKCET_SIZE];
    
    XBeeAddress64 addr64;
    ZBTxRequest zbTx;
    ZBTxStatusResponse txStatus;
    XBee xbee;

};

#endif // RADIO_MODULE_H
