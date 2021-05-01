#if !defined(PACKET_SR_H)
#define PACKET_SR_H

#include <stdint.h>

#define PAYLOAD_SIZE 35

enum Team {ECHO, VOSTOK, COSMOS};

enum Event {NO_EVENT, LIFTOFF, APOGEE, RE_TRIGGER, TOUCHDOWN};

struct SRHeader
{
    int teamID;
    int pktID;
};


struct UData
{
    int altitude;
    int temperature;
    int battery;

    float accelX;
    float accelY;
    float accelZ;

    float rotA;
    float rotX;
    float rotY;
    float rotZ;
};


typedef uint8_t Payload[PAYLOAD_SIZE];

//pack les datas dans un payload pour l'envoie avec la librairie Xbee-Arduino
void packData(const SRHeader& header, const Event& event, const UData& uData, Payload& dest);

//Fonction de decodage pour la GS, faudra peut-etre la traduire dans un autre langage si vous preferez
void decodeData(SRHeader& header, Event& event, UData& uData, const Payload& dest);

#endif // PACKET_SR_H
