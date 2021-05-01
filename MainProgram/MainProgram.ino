/*
This is the main program that controls the avionics on board of the Vostok rocket

Tasks :
 - Telemetry
 - Second recovery event redundancy
*/

#include <Vector.h>

#include "flightData.h"
#include "bmp280.h"
#include "mpu6050.h"
#include "batteryIndicator.h"
#include "eventManager.h"
#include "storage.h"
#include "radioModule.h"
#include "buzzer.h"
#include "packetSR.h"

#define BUFFER_ELEMENT_COUNT_MAX 120

//Pinout
#define BATTERY_PIN 14
#define BUZZER_PIN A9

enum States{
  Idle, Init, PrefTrans, Ascending, Descending, PostFTrans, Beacon
};

// Holds the current state
States state;

//Init status of modules for debugging see the modules code for error code documentation
int initStatus;

//Data holds 9 double (battery level, altitude, temperature, VelX, VelY, VelZ, RotX, RotY, RotZ)
FlightData measures;

// Buffer for saving data to be logged
typedef Vector<FlightData> Buffer;
FlightData bufferStorageArray[BUFFER_ELEMENT_COUNT_MAX];

Buffer buffer(bufferStorageArray);



Bmp280 bmp;
Mpu6050 mpu;
BatteryIndicator battery(BATTERY_PIN);
Buzzer buzzer(BUZZER_PIN);
EventManager eventManager;
Storage storage;
RadioModule radio;


//Variables used to control frequency of operations
unsigned long currTime, setupEndTime, measureTime, logTime, radioTime;
unsigned long liftOffTime, apogeeTime, reTriggerTime, touchdownTime; 
int measureInterval;
constexpr int radioInterval = 500; // = 2Hz
constexpr int logInterval = 1000; // = 2Hz

// Timeout if we get stuck in states before PostFTrans STILL USEFUL ?
constexpr int flightTimeout = 240000; // = 4 minutes (Check cette valeur) .
// Timeout between PoR and initialization
constexpr int idleTimeout = 5000;
// Timeout before the beacon state change after the touchdown
constexpr int postFTransTimeout = 60000; // 60 seconds
// Timeout to apogee from liftOff
constexpr int apogeeTimeout = 20000; // 20 seconds
// Bool to keep track if we sent the re trigger event
bool sentTriggerEvent;

//protoype defs
void getMeasures();
void logBuffer();
void radioTranssmission(Event event);



void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Serial monitor ready !");

  sentTriggerEvent = false;
  
  state = Idle;
  
  Serial.println("Setup done !");
  setupEndTime = millis();
}


void loop() {
  currTime = millis() - setupEndTime;
  switch(state){
    case Idle:
      if(currTime > idleTimeout) {
        state = Init;
        buzzer.initStart();
        Serial.println("Starting initialisation");
      }
      break;

    case Init:
      //pleins de ifs pour le testation
      if (!(initStatus = bmp.begin()) == 0) {
        buzzer.error();
        Serial.print("Error starting BMP280. Error code : ");
        Serial.println(initStatus);
      }
      if (!(initStatus = mpu.begin()) == 0) {
        buzzer.error();
        Serial.print("Error starting MPU6050. Error code : ");
        Serial.println(initStatus);
      }
      if (!(initStatus = battery.begin()) == 0) {
        buzzer.error();
        Serial.print("Error starting BatteryIndicator. Error code : ");
        Serial.println(initStatus);
      }
      if (!(initStatus = radio.begin()) == 0) {
        buzzer.error();
        Serial.print("Error starting RadioModule. Error code : ");
        Serial.println(initStatus);
      }
      if (!(initStatus = storage.begin()) == 0) {
        buzzer.error();
        Serial.print("Error starting Storage module. Error code : ");
        Serial.println(initStatus);
      }

      // If no initilisation errors, change state

      state = PrefTrans;
      buzzer.initSuccess();
      Serial.println("Preftrans");
      break;

    case PrefTrans: // 50Hz Log 
      measureInterval = 20; //ms 

      //50Hz data measuring
      if(currTime >= measureTime + measureInterval){
        measureTime += measureInterval;
        getMeasures();
      }

      // 2Hz GS communication & event handling
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;

        if(eventManager.isLiftOff(buffer.back().altitude, buffer.back().acceleration[3])){  
          radioTransmission(LIFTOFF);
          liftOffTime = currTime;
          state = Ascending;
        } else {
          radioTransmission(NO_EVENT);
        }
      }

            // 1Hz Data logging
      if(currTime >= logTime + logInterval){
        logTime += logInterval;
        logBuffer();
      }
      break;

    case Ascending:
      measureInterval = 10; //ms  (100Hz)

      //100Hz data measuring
      if(currTime >= measureTime + measureInterval){
        measureTime += measureInterval;
        getMeasures();
      }
      
      // 2Hz GS communication
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;
    
        if(eventManager.isApogee(buffer.back().altitude, buffer.back().acceleration[3])){
          radioTransmission(APOGEE);
          apogeeTime = currTime;
          state = Descending;
        } else {
          radioTransmission(NO_EVENT);
        }
      }
      
      // 1Hz Data logging
      if(currTime >= logTime + logInterval){
        logTime += logInterval;
        logBuffer();
      }
      break;

    case Descending:
      measureInterval = 100; //ms  (10Hz)

      //10Hz data measuring
      if(currTime >= measureTime + measureInterval){
        measureTime += measureInterval;
        getMeasures();

        //Check for the release of the main parachute (2nd event redundancy)
        if(!eventManager.hasTriggered() && eventManager.isReTrigger(measures.altitude)){
          reTriggerTime = currTime;
          eventManager.trigger();
        }
      }


      
      // 2Hz GS communication
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;
        
        if(eventManager.isTouchDown(buffer.back().altitude, buffer.back().acceleration[3])){
          radioTransmission(TOUCHDOWN);
          touchdownTime = currTime;
          state = PostFTrans;
        } else if(eventManager.hasTriggered() && !sentTriggerEvent){
          radioTransmission(RE_TRIGGER);
           sentTriggerEvent = true;
        } else{
          radioTransmission(NO_EVENT);
        }
      }
      
      // 1Hz Data logging
      if(currTime >= logTime + logInterval){
        logTime += logInterval;
        logBuffer();
      }
      break;

    case PostFTrans:
      measureInterval = 100; //ms 

      //10Hz data measuring
      if(currTime >= measureTime + measureInterval){
        measureTime += measureInterval;
        getMeasures();
      }



      // 2Hz GS communication & event handling
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;
        radioTransmission(NO_EVENT);
      }

      // Timeout before beacon
      if(currTime >= (touchdownTime + postFTransTimeout)){
        storage.logFlightInfo(liftOffTime, apogeeTime, reTriggerTime, touchdownTime);
        state = Beacon;
      }

      // 1Hz Data logging
      if(currTime >= logTime + logInterval){
        logTime += logInterval;
        logBuffer();
      }
      break;

    case Beacon:
      buzzer.beacon();
      delay(2000);
    default:
      state = Idle; //Shouldn't happen but we never know
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
  measures.acceleration[0] = mpu.getAccelX();
  measures.acceleration[1] = mpu.getAccelY();
  measures.acceleration[2] = mpu.getAccelZ();
  measures.rotation[0] = mpu.getRotA();
  measures.rotation[1] = mpu.getRotX();
  measures.rotation[2] = mpu.getRotY();
  measures.rotation[3] = mpu.getRotZ();

  buffer.push_back(measures);
}

void logBuffer() {
  storage.saveSD(buffer);
  buffer.clear();
}

void radioTransmission(Event event){
  Serial.print(measures.temperature); Serial.print(" ");
  Serial.print(measures.altitude); Serial.print(" ");
  Serial.print(measures.batteryLevel); Serial.print(" ");
  mpu.printQuat();
  Serial.println();
  if (!radio.packSend(event, buffer.back())){ // Send most recent data sample
    buzzer.error();
    // Error sending or packing shit
  }
}
