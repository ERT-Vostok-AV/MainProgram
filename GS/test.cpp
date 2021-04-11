#include <iostream>

#include "packetSR.h"

using namespace std;

int main(int argc, char const *argv[])
{
    SRHeader head;
    head.teamID = VOSTOK;
    head.pktID = 42;

    Event event = TOUCHDOWN;
    
    UData data;

    data.temperature = 25;
    data.altitude = 300;
    data.battery = 76;

    data.rotA = 90;
    data.rotX = 1;
    data.rotY = 0.5;
    data.rotZ = 0.6;

    data.velX = 14;
    data.velY = 20;
    data.velZ = 200;

    Payload pay;

    packData(head, event, data, pay);

    SRHeader headout;
    Event eventout;
    UData dataout;

    decodeData(headout, eventout, dataout, pay);

    cout << headout.teamID << endl;
    cout << headout.pktID << endl;
    cout << event << endl;
    cout << dataout.temperature << endl;
    cout << dataout.altitude << endl;
    cout << dataout.battery << endl;
    cout << dataout.rotA << endl;
    cout << dataout.rotX << endl;
    cout << dataout.rotY << endl;
    cout << dataout.rotZ << endl;
    cout << dataout.velX << endl;
    cout << dataout.velY << endl;
    cout << dataout.velZ << endl;

    return 0;
}
