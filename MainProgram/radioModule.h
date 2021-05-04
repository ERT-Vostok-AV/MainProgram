#if !defined(RADIO_MODULE_H)
#define RADIO_MODULE_H

#include "flightData.h"
#include "packetSR.h"
#include <SoftwareSerial.h>

#define PACKET_SIZE 53 // 18 byte for the frame and 35 for the payload
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
    uint8_t* getPackedData();

private:
    SRHeader header;
    Payload payload;
    bool isLedOn = false;
    void toggleLed();
};

#endif // RADIO_MODULE_H
