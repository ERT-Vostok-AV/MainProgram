#include "packetSR.h"

typedef union {
    float f_;
    int i_;
} FloatBits;

void packData(const SRHeader& header, const Event& event, const UData& uData, Payload& dest){
    dest[0] = (header.teamID & 0b11) + ((event & 0b111) << 2); //0b00000001

    dest[1] = (header.pktID >> 8) & 0xFF;
    dest[2] = header.pktID & 0xFF;

    dest[3] = uData.temperature & 0xFF;
    dest[4] = uData.battery & 0xFF;

    dest[5] = (uData.altitude >> 8) & 0xFF;
    dest[6] = uData.altitude & 0xFF;


    
    for (int i = 3; i >= 0; --i) {
        dest[6 + (4-i)] = ( *((int*) (&uData.rotA)) >> 8*i) & 0xFF; //on regarde les bits a l addresse du float et on les interprete comme un int
        dest[10 + (4-i)] = ( *((int*) (&uData.rotX)) >> 8*i) & 0xFF;
        dest[14 + (4-i)] = ( *((int*) (&uData.rotY)) >> 8*i) & 0xFF;
        dest[18 + (4-i)] = ( *((int*) (&uData.rotZ)) >> 8*i) & 0xFF;
        dest[22 + (4-i)] = ( *((int*) (&uData.accelX)) >> 8*i) & 0xFF;
        dest[26 + (4-i)] = ( *((int*) (&uData.accelY)) >> 8*i) & 0xFF;
        dest[30 + (4-i)] = ( *((int*) (&uData.accelZ)) >> 8*i) & 0xFF;
    }
}

void decodeData(SRHeader& header, Event& event, UData& uData, const Payload& dest){
    header.teamID = dest[0] & 0b11;
    event = static_cast<Event>((dest[0] >> 2) & 0b111);
    
    header.pktID = ((0 + dest[1]) << 8) + dest[2];

    uData.temperature = dest[3];
    uData.battery = dest[4];
    uData.altitude = ((0 + dest[5]) << 8) + dest[6];


    int temp = ((0 + dest[7]) << 24) + ((0 + dest[8]) << 16) + ((0 + dest[9]) << 8) + dest[10];
    uData.rotA = *((float*)(&temp));

    temp = ((0 + dest[11]) << 24) + ((0 + dest[12]) << 16) + ((0 + dest[13]) << 8) + dest[14];
    uData.rotX = *((float*)(&temp));

    temp = ((0 + dest[15]) << 24) + ((0 + dest[16]) << 16) + ((0 + dest[17]) << 8) + dest[18];
    uData.rotY = *((float*)(&temp));

    temp = ((0 + dest[19]) << 24) + ((0 + dest[20]) << 16) + ((0 + dest[21]) << 8) + dest[22];
    uData.rotZ = *((float*)(&temp));

    temp = ((0 + dest[23]) << 24) + ((0 + dest[24]) << 16) + ((0 + dest[25]) << 8) + dest[26];
    uData.accelX = *((float*)(&temp));

    temp = ((0 + dest[27]) << 24) + ((0 + dest[28]) << 16) + ((0 + dest[29]) << 8) + dest[30];
    uData.accelY = *((float*)(&temp));

    temp = ((0 + dest[31]) << 24) + ((0 + dest[32]) << 16) + ((0 + dest[33]) << 8) + dest[34];
    uData.accelZ = *((float*)(&temp));
}
