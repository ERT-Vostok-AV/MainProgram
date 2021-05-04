#include "eventManager.h"

// Altitude before which liftoff can't happen
#define LIFTOFF_ALTITUDE 1.0 //10.0
// Minimum difference from the highest altitude to declare apogee
#define APOGEE_DETECTION_THRESHOLD 0.5 //3 
// Altitude at which the 2nd event should be triggered
#define RE_TRIGGER_ALTITUDE 6.0 //300
// Number of repetition of close value before touchdown can be declared
#define STAGNATION_REPETITION_COUNT 5 // ou 10 ?
// Maximum difference between last altitude to be able to add a stagnation round
#define STAGNATION_DIFF_THRESHOLD 0.3 //0.3
// Output pin for e-match
#define E_MATCH_PIN A8
// Burn time of the e-match (ms)
#define BURN_TIME 500

/*
 * Constructor
 */
EventManager::EventManager(){
  pinMode(E_MATCH_PIN, OUTPUT);  
}

/*
 * @brief Use the given value to tell wheter we have liftoff
 * @param alt : current altitude
 */
bool EventManager::isLiftOff(double alt){
	return (alt > LIFTOFF_ALTITUDE);
}

/*
 * @brief Use the given value to tell wheter we have apogee
 * @param alt : current altitude
 */
bool EventManager::isApogee(double alt){
	if (alt > maxAlt){
	  maxAlt = alt;
	}
	return ((maxAlt - alt) > APOGEE_DETECTION_THRESHOLD);
}

/*
 * @brief Use the given value to tell wheter we should trigger the 2nd event
 * @param alt : current altitude
 */
bool EventManager::isReTrigger(double alt){
	return alt <= RE_TRIGGER_ALTITUDE; 
}

/*
 * @brief Use the given value to tell wheter we have touchdown
 * @param alt : current altitude
 */
bool EventManager::isTouchDown(double alt){
  bool res = false;
  if(lastAlt != -2058){
    if(abs(lastAlt - alt) <= STAGNATION_DIFF_THRESHOLD ){
      stagnation_count += 1;
      if(stagnation_count >= STAGNATION_REPETITION_COUNT){ 
        res = true;
      }
    } else {
      stagnation_count = 0;
    }
  }
  lastAlt = alt;
	return res & alt < TOUCHDOWN_MAX_ALT; // or data2
}

/*
 * @brief Triggers the 2nd event
 */
void EventManager::trigger(){
    //TODO allume l'e-match
    digitalWrite(E_MATCH_PIN, HIGH);
    delay(BURN_TIME);
    digitalWrite(E_MATCH_PIN, LOW);
    triggered = true;
}

/*
 * @brief Getter for the triggered attributes
 */
bool EventManager::hasTriggered(){
    return triggered;
}
