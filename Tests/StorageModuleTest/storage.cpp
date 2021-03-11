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

bool initLog(){
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
    if (dataFile) {
      for (i = 0; i > buffer.size(); i++) {
        dataFile.print("battery"); dataFile.print(separator);
        dataFile.print("alt"); dataFile.print(separator);
        dataFile.print("temp"); dataFile.print(separator);
        dataFile.print("velX"); dataFile.print(separator);
        dataFile.print("velY"); dataFile.print(separator);
        dataFile.print("velZ"); dataFile.print(separator);
        dataFile.print("rotX"); dataFile.print(separator);
        dataFile.print("rotY"); dataFile.print(separator);
        dataFile.println("rotZ");
      }
    }
}

bool Storage::logFlightInfo(unsigned long liftOffTime, unsigned long apogeeTime, unsigned longreTriggerTime, unsigned long touchdownTime){

}
