#include "buzzer.h"
#include "radioModule.h"
#include "mpu6050.h"
#include "bmp280.h"

#define led 13
const int buzzerPin = 10;


Mpu6050 mpu;
Bmp280 bmp;
Buzzer buzzer(buzzerPin);

RadioModule radio;
FlightData data;

void setup(){
    Serial.begin(9600);
    mpu.begin();
    bmp.begin();
    radio.begin();

    pinMode(led, OUTPUT);
    data.altitude = 0;
    data.temperature = 0;
    data.batteryLevel = 0;
    data.velocity[0] = 0;
    data.velocity[1] = 0;
    data.velocity[2] = 0;

    data.rotation[0] = 0;  //Rot angle
    data.rotation[1] = 0; //weights
    data.rotation[2] = 0;
    data.rotation[3] = 0;
}

void loop(){
    //radio.packSend(NO_EVENT, data);
    bmp.measure();
    data.temperature = bmp.getTemp();
    Serial.println(data.altitude);
    data.altitude = bmp.getAlt();

    mpu.measure();
    data.velocity[0] = mpu.getVelX();
    data.velocity[1] = mpu.getVelY();
    data.velocity[2] = mpu.getVelZ();

    data.rotation[0] = mpu.getRotX();
    data.rotation[1] = mpu.getRotY();
    data.rotation[2] = mpu.getRotZ();
    radio.pack(NO_EVENT, data);
    uint8_t* packed;
    packed = radio.getPackedData();
    for(int i = 17; i<53; i++){
      Serial.print(packed[i]); Serial.print(" ");
    }
    radio.send();
    Serial.println();
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    
    delay(2000);
}
