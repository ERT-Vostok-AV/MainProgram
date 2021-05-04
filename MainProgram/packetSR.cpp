#include "packetSR.h"

/*
 * @brief pack the given data into 32 bytes 
 * @param header : struct containing header info to be packed
 * @param event : enum representing the event to be packed
 * @param uData : unified struct of data to be packed
 * @param dest : destination array for the packed byte
 */
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

/*
 * @brief unpacks the data from a given array
 * @param header : destination header struct for the header info unpacked
 * @param event : destination variable for the unpacked event
 * @param uData : destination uData struct for the unpacked data
 * @param src : source array of packed data to read from
 */
void decodeData(SRHeader& header, Event& event, UData& uData, const Payload& src){
    header.teamID = src[0] & 0b11;
    event = static_cast<Event>((src[0] >> 2) & 0b111);
    
    header.pktID = ((0 + src[1]) << 8) + src[2];

    uData.temperature = src[3];
    uData.battery = src[4];
    uData.altitude = ((0 + src[5]) << 8) + src[6];


    int temp = ((0 + src[7]) << 24) + ((0 + src[8]) << 16) + ((0 + src[9]) << 8) + src[10];
    uData.rotA = *((float*)(&temp));

    temp = ((0 + src[11]) << 24) + ((0 + src[12]) << 16) + ((0 + src[13]) << 8) + src[14];
    uData.rotX = *((float*)(&temp));

    temp = ((0 + src[15]) << 24) + ((0 + src[16]) << 16) + ((0 + src[17]) << 8) + src[18];
    uData.rotY = *((float*)(&temp));

    temp = ((0 + src[19]) << 24) + ((0 + src[20]) << 16) + ((0 + src[21]) << 8) + src[22];
    uData.rotZ = *((float*)(&temp));

    temp = ((0 + src[23]) << 24) + ((0 + src[24]) << 16) + ((0 + src[25]) << 8) + src[26];
    uData.accelX = *((float*)(&temp));

    temp = ((0 + src[27]) << 24) + ((0 + src[28]) << 16) + ((0 + src[29]) << 8) + src[30];
    uData.accelY = *((float*)(&temp));

    temp = ((0 + src[31]) << 24) + ((0 + src[32]) << 16) + ((0 + src[33]) << 8) + src[34];
    uData.accelZ = *((float*)(&temp));
}
