#include "eventManager.h"

/* Unites pour les constantes:
	- Altitude : m
	- Vitesse : m/s
*/

#define LIFT_OFF 10.0
#define APOGEE_DETECTION_THRESHOLD 3
#define RE_TRIGGER_ALTITUDE 300
#define TOUCHDOWN_MAX_ALTITUDE 20
#define STAGNATION_REPETITION_COUNT 10
#define STAGNATION_DIFF_THRESHOLD 0.3

EventManager::EventManager() : triggered(false) {}

bool EventManager::isLiftOff(double alt, double accelZ){
	bool data1 = alt > LIFTOFF_ALTITUDE;
	//bool data2 = accelZ > LIFTOFF_SPEED; // To keep or not to keep ?
	return data1; // or data2;	// and ou or ?
}

bool EventManager::isApogee(double alt, double accelZ){
	if (alt > maxAlt){
	  maxAlt = alt;
	}
	//bool data1 = accelZ < 0;					// detecte vitesse verticale negative
	bool data2 = (maxAlt - alt) > APOGEE_DETECTION_THRESHOLD;	// detecte altitude qui redescent
	return data2; // or data1
	}

bool EventManager::isReTrigger(double alt){
	return alt <= RE_TRIGGER_ALTITUDE; 
}

bool EventManager::isTouchDown(double alt, double accelZ){
  bool res = false;
  if(lastAlt != 0){
    if(abs(lastAlt - alt) <= STAGNATION_DIFF_THRESHOLD ){
      stagnation_count += 1;
      if(stagnation_count >= 5){ // ou 10 ?
        res = true;
      }
      else{
        res = false;
      }
    } else {
      stagnation_count = 0;
    }
    lastAlt = alt;
  }
	return data1; // or data2
}

void EventManager::trigger(){
    //TODO allume l'e-match
    triggered = true;
}

bool EventManager::hasTriggered(){
    return triggered;
}
