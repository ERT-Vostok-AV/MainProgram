    #include "bmp280.h"
#include "mpu6050.h"
#include "buzzer.h"
//#include <Keypad.h>


#define LIFTOFF_THRESH 0.5
#define APOGEE_THRESH 2
#define RECOVERY_THRESH 1

Buzzer buzzer(10);
Bmp280 bmp;
Mpu6050 mpu;

/*
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'},
  {'C','D','E','F'}
};
byte rowPins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
//Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
*/
constexpr int radioInterval = 500; // = 2Hz
unsigned long currTime, setupEndTime, radioTime, liftOffTime, apogeeTime, touchdownTime;
unsigned long idleTimeout = 3000;

bool apogeeOverride, isReTrigger, flightDone = false;
int initStatus;
double apogeeHeight, triggerHeight, touchdownHeight, liftoffheight;

enum States{
  Idle, Init, PreFTrans, Ascending, Descending, PostFTrans
};

States state;

void setup(){
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Serial monitor ready !");
  state = Idle;
  
  if(mpu.begin() != 0){
    Serial.println("Couldn't init MPU");
  }
  
  Serial.println("Setup done !");
  buzzer.initStart();
  setupEndTime = millis();
}

void loop(){
  mpu.measure();
  mpu.printQuat();
  delay(10);
  /*
  char customKey = customKeypad.getKey();
  if(customKey == '0'){
    apogeeOverride = true;
  }
  */
  /*
  currTime = millis() - setupEndTime;
  switch(state){
    case Idle:
      if(currTime > idleTimeout & !flightDone) {
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
      if(initStatus == 0){
        buzzer.initSuccess();
        Serial.println("Init success");
        state = PreFTrans;
      }
      break;
      
    case PreFTrans: 
      // 2Hz event handling
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;

        bmp.measure();
        if(bmp.getAlt() > LIFTOFF_THRESH){
          buzzer.liftoff();
          Serial.println("LIFT OFF CONFIRMED HOUSTON");
          liftoffheight = bmp.getAlt();
          liftOffTime = currTime;
          state = Ascending;
          break;
        }
      }
      break;
      
    case Ascending:
      
      // 2Hz GS communication
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;
        bmp.measure();
    
        if((bmp.getAlt() > APOGEE_THRESH) or apogeeOverride){
          buzzer.apogee();
          Serial.println("APOGEE");
          apogeeOverride = false;          
          apogeeHeight = bmp.getAlt();
          apogeeTime = currTime;
          state = Descending;
          break;
        }
      }
      break;
    case Descending:
      if(currTime >= radioTime + radioInterval){
        radioTime += radioInterval;
        
        bmp.measure();

        if(bmp.getAlt() < RECOVERY_THRESH & !isReTrigger){
          Serial.print("RECOVERY TRIGGER AT: "); Serial.println(bmp.getAlt());
          buzzer.reTrigger();
          triggerHeight = bmp.getAlt();
          isReTrigger = true;
        }
        if(bmp.getAlt() < LIFTOFF_THRESH){
          buzzer.liftoff();
          Serial.println("TOUCHDOWN BEBE");
          touchdownTime = currTime;
          touchdownHeight = bmp.getAlt();
          state = PostFTrans;
          break;
        }
      }
      break;
    case PostFTrans:
      buzzer.beacon();
      Serial.println("END OF FLIGHT");
      Serial.print("Liftoff: "); Serial.println(liftoffheight);
      Serial.print("Apogee: "); Serial.println(apogeeHeight);
      Serial.print("Trigger: "); Serial.println(triggerHeight);
      Serial.print("Touchdown: "); Serial.println(touchdownHeight);
      flightDone = true;
      state = Idle;
  }
  */
}
