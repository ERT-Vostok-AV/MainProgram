/*
This is the main program that controls the avionics on board of the Vostok rocket

Tasks :
 - Telemetry
 - Second recovery event redundancy
*/

#include <Vector.h>
#include <QueueList.h>

#include "bmp280.h"
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
typedef Vector<double*> Buffer;
Buffer buffer;

// FIFO list for event detection
QueueList<double*> fifo;

double measuresArray[9], rotAndVelCoord[6];

double alt;

Bmp280 bmp;
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






//int millis(){return 0;} //STUB USELESS PIECE OF SHIT
//void delay(int time){} //STUB delay

void getMeasures();
void logBuffer();
void radioTranssmission();

void clearFifo(QueueList<double*>& fifo);

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
      if (!bmp.begin()) {
        buzzer.error();
        // Error starting bmp280. Serial print pour testing
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

      // If no initilisation errors, change state

      state = PrefTrans;
      buzzer.initSuccess();
      break;

    case PrefTrans: // 50Hz Log 
      measureInterval = 20; //ms 

      //50Hz data measuring
      if(currTime >= measureTime + measureInterval){
        measureTime += measureInterval;
        getMeasures();
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
          clearFifo(fifo);
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
        getMeasures();
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
          clearFifo(fifo);
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
        getMeasures();

        if(eventManager.isReTrigger((int) altAndTemp[0]) && !eventManager.hasTriggered()){
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
          clearFifo(fifo);
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
        getMeasures();
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

void getMeasures() {
  bmp.measure();
  alt = bmp.getAlt();
  rotAndVelCoord = mpu.getRotAndVel(); // xRot yRot zRot xVel yVel zVel

  measures = {(double)battery.getBatteryLevel(), bmp.getAlt(), bmp.getTemp(),
              rotAndVelCoord[0], rotAndVelCoord[1], rotAndVelCoord[2],
              rotAndVelCoord[3], rotAndVelCoord[4], rotAndVelCoord[5]};

  buffer.push_back(measures);

  if (fifo.count() > radioInterval / measureInterval) {
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


void clearFifo(QueueList<double*>& fifo) {
  while (!fifo.isEmpty()) {
    fifo.pop();
  }
}
