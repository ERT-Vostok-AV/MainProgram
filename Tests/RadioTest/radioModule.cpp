#include "radioModule.h"

/**Error codes:
 * 0 no error
 * 
 */

UData uData;
Event event;
SoftwareSerial xbeeSS(RX_PIN, TX_PIN);

RadioModule::RadioModule() {
  
    header.teamID = VOSTOK;
    header.pktID = 0;

    xbee = XBee();
}

int RadioModule::begin(){
    //TODO Initialisation
    xbeeSS.begin(XBEE_FREQ);                                          
    packet = {0x7E, PACKET_SIZE_HEX, 0x10, header.pktID, 0x00, 0x13, 0xA2, 0x00, 0x41, 0xCE, 0x3F, 0x60, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, x00
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, x00, 0x00};
    addr64 = XBeeAddress64(0x0013a200, 0x403e0f30); //CHANGE THAT
    zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
    txStatus = ZBTxStatusResponse();

    xbee.setSerial(Serial);
}

bool RadioModule::pack(Event event, FlightData data){
    uData.altitude = (int) data.altitude;
    uData.temperature = (int) data.temperature;
    uData.battery = (int) data.batteryLevel;
    uData.velX = (float) data.velocity[0];
    uData.velY = (float) data.velocity[1];
    uData.velZ = (float) data.velocity[2];
    
    packData(header, event, uData, payload);
    for(int i = 16; i<50; i++){
      packet[i] = payload[i-16];
    }

    ++header.pktID;
      
}

bool RadioModule::send(){
    xbee.send(zbTx);
    xbeeSS.write(packet, sizeof(packet));
}

bool RadioModule::packSend(Event event, FlightData data){
    pack(event, data);
    send();
}
