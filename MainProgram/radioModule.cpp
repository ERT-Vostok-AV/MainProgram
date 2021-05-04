#include "radioModule.h"

/**Error codes:
 * 0 no error
 * else see xbee doc
 */

// Xbee serial port 
#define xbee Serial8
// Index from the begining of the packet array of the first data byte
#define PACKET_PAYLOAD_OFFSET 17

UData uData;
Event event;

//7E start delimiter, 0x00 & 0x2C packet size (should be constant, = 44(12 + 35))
// 0x00 to 0x60 is destintation 64-bit address, 0xFF & 0xFE adress unknown code (not important)
// 0x00 & 0x00 broadcast & radius bytes
uint8_t packet[PACKET_SIZE] = {0x7E, 0x00, 0x31, 0x10, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x41, 0xCE, 0x3F, 0x60, 0xFF, 0xFE, 0x00, 0x00,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //data
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //data
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       //data
              0x00}; //checksum

/*
 * @brief constructor
 */
RadioModule::RadioModule() {
    header.teamID = VOSTOK;
    header.pktID = 0;
}

/*
 * @brief initialize the radio
 */
int RadioModule::begin(){
    pinMode(TX_PIN, OUTPUT);
    pinMode(RX_PIN, INPUT);
    xbee.setRX(RX_PIN);
    xbee.setTX(TX_PIN);
    xbee.begin(XBEE_FREQ);
    return 0;
}

/*
 * @brief Packs the event and puts it into the payload
 * @param event : event to pack
 * @param data : data to pack
 */
bool RadioModule::pack(Event event, FlightData data){
    uData.altitude = (int) data.altitude;
    uData.temperature = (int) data.temperature;
    uData.battery = (int) data.batteryLevel;
    uData.accelX = (float) data.acceleration[0];
    uData.accelY = (float) data.acceleration[1];
    uData.accelZ = (float) data.acceleration[2];
    uData.rotA = (float) data.rotation[0];
    uData.rotX = (float) data.rotation[1];
    uData.rotY = (float) data.rotation[2];
    uData.rotZ = (float) data.rotation[3];
    
    // Pack the data using packetSR
    packData(header, event, uData, payload);

    // Puts the packed values into the payload and compute checksum
    int sum = 0x471;
    for(int i = 0; i<PAYLOAD_SIZE; i++){
      packet[i+PACKET_PAYLOAD_OFFSET] = payload[i];
      sum = sum + (int)payload[i];
    }

    // Add checksum
    uint8_t sum8bit = (sum & 0b11111111);
    packet[PACKET_SIZE-1] = (255 - sum8bit);
    // Increment pktID for next packet
    header.pktID = header.pktID + 1;
    
}

/*
 * @brief send the packet. Make sure to pack the data into it first
 */
bool RadioModule::send(){
    xbee.write(packet, sizeof(packet));
}

/*
 * @brief Pack the data and immediately send it
 * @param event : event to pack and send
 * @param data : data to pack and send
 */
bool RadioModule::packSend(Event event, FlightData data){
    toggleLed();
    pack(event, data);
    send();
    toggleLed();
}
