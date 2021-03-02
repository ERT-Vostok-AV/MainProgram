/*
This is the main program that controls the avionics on board of the Vostok rocket

Tasks :
 - Telemetry
 - Second recovery event redundancy
*/

#include <Vector.h>
#include <QueueList.h>

#include "flightData.h"
#include "bmp280.h"
#include "mpu6050.h"
#include "batteryIndicator.h"
#include "eventManager.h"
#include "storage.h"
#include "radioModule.h"
#include "buzzer.h"

#define BUFFER_ELEMENT_COUNT_MAX 100

enum States{
  Idle, Init, PrefTrans, Ascending, Descending, PostFTrans, Beacon
};

// Holds the current state
States state;

//Data holds 9 double (battery level, altitude, temperature, VelX, VelY, VelZ, RotX, RotY, RotZ)
FlightData measures;

// Buffer for saving data to be logged
typedef Vector<FlightData> Buffer;
FlightData bufferStorageArray[BUFFER_ELEMENT_COUNT_MAX];

Buffer buffer(bufferStorageArray);

// FIFO list for event detection
QueueList<FlightData> fifo;


Bmp280 bmp;
Mpu6050 mpu;
BatteryIndicator battery;
EventManager eventManager;
Storage storage;
RadioModule radio;
Buzzer buzzer;


//Variables used to control frequency of operations
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



void getMeasures();
void logBuffer();
void radioTranssmission();

void clearFifo(QueueList<FlightData>& fifo);

void setup() {
  Serial.begin(9600);
  while(!Serial)
  Serial.println("Ready to rock");



  currTime = 0;
  logTime = 0;
  radioTime = 0;
  state = Idle;

  Serial.println("Setup done !")
}

void loop() {
  currTime = millis();
  switch(state){
    case Idle:
       if(currTime > idleTimeout) {
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

        if(eventManager.isReTrigger(measures.altitude) && !eventManager.hasTriggered()){
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
  mpu.measure();
  battery.measure();

  measures.batteryLevel = battery.getBatteryLevel();
  measures.altitude = bmp.getAlt();
  measures.temperature = bmp.getTemp();
  measures.velocity[0] = mpu.getVelX();
  measures.velocity[1] = mpu.getVelY();
  measures.velocity[2] = mpu.getVelZ();
  measures.rotation[0] = mpu.getRotX();
  measures.rotation[1] = mpu.getRotY();
  measures.rotation[2] = mpu.getRotZ();

  buffer.push_back(measures);

  if (fifo.count() > radioInterval / measureInterval) {
    fifo.pop();
  }
  fifo.push({bmp.getAlt(), mpu.getVelX(), mpu.getVelY(), mpu.getVelZ()});
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
