#include "buzzer.h"
#include <Keypad.h>
#include "mpu6050.h"
#include "bmp280.h"
#include "batteryIndicator.h"

// BATTERY STUFF
BatteryIndicator battery(A7);


// MPU STUFF
Mpu6050 mpu;
int mpuEnabled = 0;

// BUZZER STUFF
Buzzer buzzer(10);

// BMP280 STUFF
Bmp280 bmp;
int bmpEnabled= 0;

// KEYPAD STUFF
const byte ROWS = 4;
const byte COLS = 4;
char key;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}};

byte rowPins[ROWS] = {6,7,8,9};
byte colPins[COLS] = {2,3,4,5};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup(){
    
    Serial.begin(9600);
    while(!Serial);
    Serial.println("Serial ready");

    bmp.begin();
    Serial.println("BMP ready!");
    
    mpu.begin();
    Serial.println("MPU ready!");
    delay(1000);
    Serial.println("0 0 0");
}

void loop(){
  key = keypad.getKey();
  if(key == '1'){
    Serial.println("Success");
    buzzer.initSuccess();
    delay(200);
  } else if(key == '2'){
    Serial.println("Start");
    buzzer.initStart();
    delay(200);
  } else if(key == '3'){
    Serial.println("Error");
    buzzer.error();
    delay(200);
  } else if(key == 'A'){
    Serial.println("Beacon");
    buzzer.beacon();
    delay(200);
  } else if(key == '4'){
    mpuEnabled = 1;
    delay(200);
  } else if(key == '5'){
    mpuEnabled = 0;
    delay(200);
  }  else if(key == '6'){
    bmpEnabled = 1;
    delay(200);
  } else if(key == 'B'){
    bmpEnabled = 0;
    delay(200);
  } else if(key == '7'){  
    battery.measure();
    Serial.print("Battery level: ");
    Serial.print(battery.getBatteryLevel()); Serial.println(" %");
    delay(1000);
  }

  if(bmpEnabled == 1){
    bmp.measure();
    Serial.print(bmp.getAlt()); Serial.print(" ");
    Serial.print(bmp.getTemp()); Serial.println();
  }

  mpu.measure();
  if(mpuEnabled == 1){
    Serial.print(mpu.getRotX()); Serial.print(" ");
    Serial.print(mpu.getRotY()); Serial.print(" ");
    Serial.print(mpu.getRotZ()); Serial.println();
  }
}
