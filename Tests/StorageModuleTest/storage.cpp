#include "storage.h"

/* Structure du buffer
  [[bat1, alt1, temp1, accX1, accY1, accZ1, rotX1, rotY1, rotZ1],
  [bat2, alt2, temp2, accX2, accY2, accZ2, rotX2, rotY2, rotZ2],
  [bat3, alt3, temp3, accX3, accY3, accZ3, rotX3, rotY3, rotZ3],
  ...]
*/

/**Error codes:
 * 0 no error
 * 1 SD card init error
 */

Storage::Storage(){}

int Storage::begin() {
  if(SD.begin(BUILTIN_SDCARD)){
    return 0;
  } else {
    return 1;
  }
}

bool Storage::initLog(){
   dataFile = SD.open("data.txt", FILE_WRITE);
   if(dataFile){
    dataFile.println("--------------- VOSTOK FLIGHT DATA ---------------");

    //data structure :
    //batteryLevel ; altitude ; temperature ; velX ; velY ; velZ ; rotX ; rotY ; rotZ
    dataFile.print("battery"); dataFile.print(separator);
    dataFile.print("alt"); dataFile.print(separator);
    dataFile.print("temp"); dataFile.print(separator);
    dataFile.print("velX"); dataFile.print(separator);
    dataFile.print("velY"); dataFile.print(separator);
    dataFile.print("velZ"); dataFile.print(separator);
    dataFile.print("rotX"); dataFile.print(separator);
    dataFile.print("rotY"); dataFile.print(separator);
    dataFile.println("rotZ");
    
    return true;
   } else {
    return false;
   }
}

bool Storage::saveSD(const Buffer& buffer){
  //Serial.println("imhere");
    if (dataFile) {
      for (unsigned i = 0; i < buffer.size(); i++) {
        dataFile.print(String(buffer[i].batteryLevel)); dataFile.print(separator);
        dataFile.print(String(buffer[i].altitude)); dataFile.print(separator);
        dataFile.print(String(buffer[i].temperature)); dataFile.print(separator);
        dataFile.print(String(buffer[i].velocity[0])); dataFile.print(separator);
        dataFile.print(String(buffer[i].velocity[1])); dataFile.print(separator);
        dataFile.print(String(buffer[i].velocity[2])); dataFile.print(separator);
        dataFile.print(String(buffer[i].rotation[0])); dataFile.print(separator);
        dataFile.print(String(buffer[i].rotation[1])); dataFile.print(separator);
        dataFile.println(String(buffer[i].rotation[2]));
      }
    }
}

bool Storage::logFlightInfo(unsigned long liftOffTime, unsigned long apogeeTime, unsigned long reTriggerTime, unsigned long touchdownTime){
  dataFile.println("--------------- END OF FLIGHT ---------------");
  dataFile.println("Flight summary :");
  dataFile.print("Lift off time : "); dataFile.println(liftOffTime);
  dataFile.print("Apogee time : "); dataFile.println(apogeeTime);
  dataFile.print("Recovery trigger time : "); dataFile.println(reTriggerTime);
  dataFile.print("Touchdown time : "); dataFile.println(touchdownTime);
  dataFile.close();
}

void Storage::test(){
  dataFile = SD.open("data.txt", FILE_WRITE);
  dataFile.close();
}
