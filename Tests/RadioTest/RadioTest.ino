#include "buzzer.h"
#include "radioModule.h"

const int buzzerPin = 10;

Buzzer buzzer(buzzerPin);

RadioModule radio;
FlightData data;

void setup(){

    radio.begin();

    data.altitude = 10;
    data.temperature = 25;
    data.batteryLevel = 76;
    data.velocity[0] = 5;
    data.velocity[1] = 2.5;
    data.velocity[2] = 3.6;

    data.rotation[0] = 90;  //Rot angle
    data.rotation[1] = 0.5; //weights
    data.rotation[2] = 0.25;
    data.rotation[3] = 1;
}

void loop(){
    Event event = NO_EVENT;
    radio.packSend(event, data);

    
    delay(1000);
}
