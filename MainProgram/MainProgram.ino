/*
This is the main program that controls the avionics on board of the Vostok rocket

Tasks :
 - Telemetry
 - Second recovery event redundancy
*/
#include <queue>
#include <vector>
#include "barometer.h"
#include "thermometer.h"
#include "accelerometer.h"
#include "gyroscope.h"
#include "batteryIndicator.h"
#include "eventManager.h"
#include "storage.h"
#include "radioModule.h"
#include "buzzer.h"

enum States{
  Idle, Init, PrefTrans, Ascending, Descending, PostFTrans, Beacon
};

// Buffer for saving data to be logged
typedef std::vector<std::vector<double>> Buffer;
Buffer buffer;
std::queue<std::vector<double>> fifo;
std::vector<double> measures;

Barometer baro;
Thermometer thermo(0x76);
Gyroscope gyro(0x68);
Accelerometer accel(0x68);
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



// Holds the current state
States state;


int millis(){return 0;} //STUB USELESS PIECE OF SHIT
void delay(int sex){} //STUB delay

void setup() {
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
         buzzer.initStart();
       } 
       
       break;

    case Init: 
      if (!baro.begin()){
        buzzer.error();
        // Error starting barometer (may be buzer?) Serial print pour testing
      }
      //TODO Init tous les autres et buzzer

      // If no errors initializing, change state

      state = PrefTrans;
      buzzer.initSuccess();
      break;

    case PrefTrans: // 50Hz Log 
      measureInterval = 20; //ms 

      //50Hz data measuring
      if(currTime >= measureTime + measureInterval){
        measureTime += measureInterval;
        double alt = baro.getAltitude();
        double accelX = accel.getXAcc();
        double accelY = accel.getYAcc();
        double accelZ = accel.getZAcc(); 

        measures = {(double)battery.getBatteryLevel(), (double)alt, (double)thermo.getTemperature(),
                     accelX, accelY, accelZ, gyro.getXRot(), gyro.getYRot(), gyro.getZRot()};
        buffer.push_back(measures);

        if(fifo.size() >= (radioInterval / measureInterval)){
          fifo.pop();
        }
        fifo.push({alt, accelX, accelY, accelZ});
      }

      // 1Hz Data logging
      if(currTime >= logTime + logInterval){
        logTime += logInterval;

        // If we use memory
        if(!storage.saveDataMemory(buffer)){
          buzzer.error(); // Really ?
          // Error while saving in memory
        }
        // If we don't use memory (uncomment)
        //storage.savaDataSD(buffer);

        buffer.clear();
      }

      // 2Hz GS communication & event handling
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;
        if (!radio.packSend(buffer.back())){ // Send most recent data sample
          buzzer.error();
          // Error sending or packing shit
        }

        if(eventManager.isLiftOff(fifo)){
          fifo = std::queue<std::vector<double>>(); //empties the queue ,_,
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
        double alt = baro.getAltitude();
        double accelX = accel.getXAcc();
        double accelY = accel.getYAcc();
        double accelZ = accel.getZAcc(); 

        measures = {(double)battery.getBatteryLevel(), (double)alt, (double)thermo.getTemperature(),
                     accelX, accelY, accelZ, gyro.getXRot(), gyro.getYRot(), gyro.getZRot()};
        buffer.push_back(measures);
        if(fifo.size() >= (radioInterval / measureInterval)){
          fifo.pop();
        }
        fifo.push({alt, accelX, accelY, accelZ});
      }

      // 1Hz Data logging
      if(currTime >= logTime + logInterval){
        logTime += logInterval;

        // If we use memory
        if(!storage.saveDataMemory(buffer)){
          // Error while saving in memory
        }
        // If we don't use memory (uncomment)
        //storage.savaDataSD(buffer);
        buffer.clear();
      }
      
      // 2Hz GS communication
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;

        if (!radio.packSend(buffer.back())){
          // Error sending or packing shit
        }
    
        if(eventManager.isApogee(fifo)){
          fifo = std::queue<std::vector<double>>(); //empties the queue ,_,
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
        double alt = baro.getAltitude();
        double accelX = accel.getXAcc();
        double accelY = accel.getYAcc();
        double accelZ = accel.getZAcc(); 

        // Check if we've reached trigger altitude
        if(eventManager.isReTrigger(alt) && !eventManager.hasTriggered()){
          eventManager.trigger();
        }
 
        measures = {(double)battery.getBatteryLevel(), (double)alt, (double)thermo.getTemperature(),
                     accelX, accelY, accelZ, gyro.getXRot(), gyro.getYRot(), gyro.getZRot()};
        buffer.push_back(measures);
        if(fifo.size() >= (radioInterval / measureInterval)){
          fifo.pop();
        }
        fifo.push({alt, accelX, accelY, accelZ});
      }

      // 1Hz Data logging
      if(currTime >= logTime + logInterval){
        logTime += logInterval;

        // If we use memory
        if(!storage.saveDataMemory(buffer)){
          // Error while saving in memory
        }
        // If we don't use memory (uncomment)
        //storage.savaDataSD(buffer);
        buffer.clear();
      }
      
      // 2Hz GS communication
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;

        if (!radio.packSend(buffer.back())){
          // Error sending or packing shit
        }
    
        if(eventManager.isTouchDown(fifo)){
          fifo = std::queue<std::vector<double>>(); //empties the queue ,_,
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
        double alt = baro.getAltitude();
        double accelX = accel.getXAcc();
        double accelY = accel.getYAcc();
        double accelZ = accel.getZAcc(); 

        measures = {(double)battery.getBatteryLevel(), (double)alt, (double)thermo.getTemperature(),
                     accelX, accelY, accelZ, gyro.getXRot(), gyro.getYRot(), gyro.getZRot()};
        buffer.push_back(measures);

        if(fifo.size() >= (radioInterval / measureInterval)){
          fifo.pop();
        }
        fifo.push({alt, accelX, accelY, accelZ});
      }

      // 1Hz Data logging
      if(currTime >= logTime + logInterval){
        logTime += logInterval;

        // If we use memory
        if(!storage.saveDataMemory(buffer)){
          buzzer.error(); // Really ?
          // Error while saving in memory
        }
        // If we don't use memory (uncomment)
        //storage.savaDataSD(buffer);

        buffer.clear();
      }

      // 2Hz GS communication & event handling
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;
        if (!radio.packSend(buffer.back())){ // Send most recent data sample
          buzzer.error();
          // Error sending or packing shit
        }
      }

      // Timeout if touchdown was missed
      if(currTime >= (touchdownTime + transferTimeout)){
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
