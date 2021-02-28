/*
This is the main program that controls the avionics on board of the Vostok rocket

Tasks :
 - Telemetry
 - Second recovery event redundancy
*/

#include <Vector.h>
#include <QueueList.h>

#include "barometer.h"
#include "thermometer.h"
#include "mpu6050.h"
#include "batteryIndicator.h"
#include "eventManager.h"
#include "storage.h"
#include "radioModule.h"
#include "buzzer.h"

enum States{
  Idle, Init, PrefTrans, Ascending, Descending, PostFTrans, Beacon
};

// Holds the current state
States state;

// Buffer for saving data to be logged
typedef Vector<Data> Buffer;
Buffer buffer;
cppQueue fifo25Rec(sizeof(Vector), 25, FIFO, false); //25 records
cppQueue fifo50Rec(sizeof(Vector), 50, FIFO, false); //50 records
cppQueue fifo5Rec(sizeof(Vector), 5, FIFO, false); //5 records

double measures[9], rotAndVelCoord[6];
double alt;

Barometer baro;
Thermometer thermo;
Mpu6050 mpu;
BatteryIndicator battery;
EventManager eventManager;
Storage storage;
RadioModule radio;
Buzzer buzzer;

unsigned long currTime, measureTime, logTime, radioTime, liftOffTime, touchdownTime; 
int measureInterval;
constexpr int radioInterval = 500; // = 2Hz
constexpr int logInterval = 1000; // = 2Hz

// Timeout if we get stuck in states before PostFTrans
constexpr int flightTimeout = 240000; // = 4 minutes (Check cette valeur) .
// Timeout between PoR and initialization
constexpr int idleTimeout = 5000;
// Timeout to start transfering to sdcard after the flight
constexpr int transferTimeout = 60000; // 60 seconds
// Timeout to apogee
constexpr int apogeeTimeout = 20000; // 20 seconds






int millis(){return 0;} //STUB USELESS PIECE OF SHIT
void delay(int sex){} //STUB delay

void getMeasures(cppQueue fifo);
void logBuffer();
void radioTranssmission();

void setup() {
  Serial.begin(9600);
  while(!Serial)
  Serial.println("Ready to rock");

  currTime = 0;
  logTime = 0;
  radioTime = 0;
  state = Idle;
}

void loop() {
  currTime = millis();
  switch(state){
    case Idle:
       if(currTime > idleTimeout){
         state = Init;
         Serial.println("New state: "); Serial.println(state);
         buzzer.initStart();
       }
       break;

    case Init:
      //pleins de ifs pour le testation
      if (!baro.begin()){
        buzzer.error();
        // Error starting barometer. Serial print pour testing
      }
      if (!thermo.begin()) {
        buzzer.error();
        // Error starting thermometer. Serial print pour testing
      }
      if (!mpu.begin()) {
        buzzer.error();
        // Error starting mpu6050. Serial print pour testing
      }
      if (!battery.begin()) {
        buzzer.error();
        // Error starting battery indicator. Serial print pour testing
      }
      if (!radio.begin()) {
        buzzer.error();
        // Error starting radio. Serial print pour testing
      }

      // If no errors initializing, change state

      state = PrefTrans;
      buzzer.initSuccess();
      break;

    case PrefTrans: // 50Hz Log 
      measureInterval = 20; //ms 

      //50Hz data measuring
      if(currTime >= measureTime + measureInterval){
        measureTime += measureInterval;
        getMeasures(fifo25Rec);
      }

      // 1Hz Data logging
      if(currTime >= logTime + logInterval){
        logTime += logInterval;
        logBuffer();
      }

      // 2Hz GS communication & event handling
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;
        radioTransmission();

        if(eventManager.isLiftOff(fifo)){
          fifo25Rec.flush(); //empties the queue ,_,
          liftOffTime = currTime;
          state = Ascending;
          break;
        }
      }

      /* Timeout Check  /!\ ONLY IF WE USE MEMORY BEFORE STORING IN THE SD CARD /!\ */
      if(currTime >= flightTimeout){
        touchdownTime = currTime;
        state = PostFTrans;
      }
      break;

    case Ascending:
      measureInterval = 10; //ms  (100Hz)

      //100Hz data measuring
      if(currTime >= measureTime + measureInterval){
        measureTime += measureInterval;
        getMeasures(fifo50Rec);
      }

      // 1Hz Data logging
      if(currTime >= logTime + logInterval){
        logTime += logInterval;
        logBuffer();
      }
      
      // 2Hz GS communication
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;
        radioTransmission();
    
        if(eventManager.isApogee(fifo)){
          fifo50Rec.flush(); //empties the queue ,_,
          state = Descending;
          break;
        }
      }

      /* Timeout Check  /!\ ONLY IF WE USE MEMORY BEFORE STORING IN THE SD CARD /!\ */
      if(currTime >= (apogeeTimeout - liftOffTime)){
        state = PostFTrans;
      }

      break;

    case Descending:
      measureInterval = 100; //ms  (10Hz)

      //10Hz data measuring
      if(currTime >= measureTime + measureInterval){
        measureTime += measureInterval;
        getMeasures(fifo5Rec);

        if(eventManager.isReTrigger(alt) && !eventManager.hasTriggered()){
          eventManager.trigger();
        }
      }

      // 1Hz Data logging
      if(currTime >= logTime + logInterval){
        logTime += logInterval;
        logBuffer();
      }
      
      // 2Hz GS communication
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;
        radioTransmission();
    
        if(eventManager.isTouchDown(fifo)){
          fifo5Rec.flush(); //empties the queue ,_,
          touchdownTime = currTime;
          state = PostFTrans;
          break;
        }
      }

      /* Timeout Check  /!\ ONLY IF WE USE MEMORY BEFORE STORING IN THE SD CARD /!\ */
      if(currTime >= flightTimeout){
        touchdownTime = currTime;
        state = PostFTrans;
      }

      break;

    case PostFTrans:
      measureInterval = 100; //ms 

      //10Hz data measuring
      if(currTime >= measureTime + measureInterval){
        measureTime += measureInterval;
        getMeasures(fifo5Rec);
      }

      // 1Hz Data logging
      if(currTime >= logTime + logInterval){
        logTime += logInterval;
        logBuffer();
      }

      // 2Hz GS communication & event handling
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;
        radioTransmission();
      }

      // Timeout if touchdown was missed
      if(currTime >= (touchdownTime + transferTimeout)){
        storage.sdTransfer();
        state = Beacon;
      }
      break;

    case Beacon:
      buzzer.beacon();
      delay(1000);
    default:
      // C LA MERD
      break;
  }
}

void getMeasures(cppQueue fifo) {
  alt = baro.getAltitude();
  rotAndVelCoord = mpu.getRotAndVel(); // xRot yRot zRot xVel yVel zVel

  measures = {(double)battery.getBatteryLevel(), (double)alt, (double)thermo.getTemperature(),
              rotAndVelCoord[0], rotAndVelCoord[1], rotAndVelCoord[2], rotAndVelCoord[3], rotAndVelCoord[4], rotAndVelCoord[5]};
  buffer.push_back(measures);

  if (fifo.isFull()) {
    fifo.pop();
  }
  fifo.push({alt, rotAndVelCoord[3], rotAndVelCoord[4], rotAndVelCoord[5]});
}

void logBuffer() {
  // If we use memory
  if(!storage.saveMemory(buffer)){
    buzzer.error(); // Really ?
    // Error while saving in memory
  }
  // If we don't use memory (uncomment)
  //storage.savaDataSD(buffer);

  buffer.clear();
}

void radioTransmission(){
  if (!radio.packSend(buffer.back())){ // Send most recent data sample
    buzzer.error();
    // Error sending or packing shit
  }
}
