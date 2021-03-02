#include "eventManager.h"

/* Structure de la fifo //outdated
  [[alt1, accX1, accY1, accZ1],
  [alt2, accX2, accY2, accZ2],
  [alt3, accX3, accY3, accZ3],
  ...]
*/

EventManager::EventManager() : triggered(false) {}

bool EventManager::isLiftOff(QueueList<FlightData> fifo){

}

bool EventManager::isApogee(QueueList<FlightData> fifo){

}

bool EventManager::isReTrigger(double alt){
  return alt <= 300 //SOME RANDOM ALTITUDE NEED TO CHECK
}

bool EventManager::isTouchDown(QueueList<FlightData> fifo){

}

bool EventManager::trigger(){
    //TODO allume l'e-match
    triggered = true;
}

bool EventManager::hasTriggered(){
    return triggered;
}