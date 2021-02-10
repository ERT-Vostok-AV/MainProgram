/*
This is the main program that controls the avionics on board of the Vostok rocket

Tasks :
 - Telemetry
 - Second recovery event redundancy
*/

#include "barometer.h"
#include "thermometer.h"
#include "accelerometer.h"
#include "gyroscope.h"

enum States{
  Idle, Init, PrefTrans, Ascending, Descending, PostFTrans, SDTransfer
};

enum Event{
  LiftOff, Apogee, ScndEvent, TouchDown
};

Barometer baro(0x76);
Thermometer thermo(0x76);
Gyroscope gyro(0x68);
Accelerometer accel(0x68);


States state;
bool event;

void setup() {
  
}

void loop() {
  // put your main code here, to run repeatedly:

  switch(state){
    case Idle:
    case Init: 
      if (!baro.begin()){
        // Error starting barometer
      }
      // Init tous les autres
      state = PrefTrans;
    default:
  }

}

bool eventManager(){

}
