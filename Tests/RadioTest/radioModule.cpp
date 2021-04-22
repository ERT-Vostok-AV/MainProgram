#include "radioModule.h"

/**Error codes:
 * 0 no error
 * 
 */

UData uData;
Event event;


RadioModule::RadioModule() {
    header.teamID = VOSTOK;
    header.pktID = 0;

    xbee = XBee();
}

int RadioModule::begin(){
    //TODO Initialisation

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

    ++header.pktID;
}

bool RadioModule::send(){
    xbee.send(zbTx);
}

bool RadioModule::packSend(Event event, FlightData data){
    pack(event, data);
    send();
}
