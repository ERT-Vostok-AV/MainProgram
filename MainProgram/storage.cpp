#include "storage.h"

/**Error codes:
 * 0 no error
 * 1 SD card init error
 */

Storage::Storage(){}

int Storage::begin() {
  if(SD.begin(BUILTIN_SDCARD)){
    dataFile = SD.open("FDATA.txt", FILE_WRITE);
    if(dataFile){
      dataFile.println("--------------- VOSTOK FLIGHT DATA ---------------");

      //data structure :
      //batteryLevel ; altitude ; temperature ; velX ; velY ; velZ ; rotA ; rotX ; rotY ; rotZ
      dataFile.print("battery"); dataFile.print(separator);
      dataFile.print("alt"); dataFile.print(separator);
      dataFile.print("temp"); dataFile.print(separator);
      dataFile.print("velX"); dataFile.print(separator);
      dataFile.print("velY"); dataFile.print(separator);
      dataFile.print("velZ"); dataFile.print(separator);
      dataFile.print("rotA"); dataFile.print(separator);
      dataFile.print("rotX"); dataFile.print(separator);
      dataFile.print("rotY"); dataFile.print(separator);
      dataFile.println("rotZ");
      dataFile.close();
    }
    return 0;
  } else {
    return 1;
  }
}


bool Storage::saveSD(const Buffer& buffer){
  dataFile = SD.open("FDATA.txt", FILE_WRITE);
  if (dataFile) {
    for (unsigned i = 0; i < buffer.size(); i++) {
      dataFile.print(String(buffer[i].batteryLevel)); dataFile.print(separator);
      dataFile.print(String(buffer[i].altitude)); dataFile.print(separator);
      dataFile.print(String(buffer[i].temperature)); dataFile.print(separator);
      dataFile.print(String(buffer[i].acceleration[0])); dataFile.print(separator);
      dataFile.print(String(buffer[i].acceleration[1])); dataFile.print(separator);
      dataFile.print(String(buffer[i].acceleration[2])); dataFile.print(separator);
      dataFile.print(String(buffer[i].rotation[0])); dataFile.print(separator);
      dataFile.print(String(buffer[i].rotation[1])); dataFile.print(separator);
      dataFile.print(String(buffer[i].rotation[2])); dataFile.print(separator);
      dataFile.println(String(buffer[i].rotation[3]));
    }
    dataFile.close();
  }
}

bool Storage::logFlightInfo(unsigned long liftOffTime, unsigned long apogeeTime, unsigned long reTriggerTime, unsigned long touchdownTime){
  dataFile = SD.open("FDATA.txt", FILE_WRITE);
  dataFile.println("--------------- END OF FLIGHT ---------------");
  dataFile.println("Flight summary :");
  dataFile.print("Lift off time : "); dataFile.println(liftOffTime);
  dataFile.print("Apogee time : "); dataFile.println(apogeeTime);
  dataFile.print("Recovery trigger time : "); dataFile.println(reTriggerTime);
  dataFile.print("Touchdown time : "); dataFile.println(touchdownTime);
  dataFile.close();
}
