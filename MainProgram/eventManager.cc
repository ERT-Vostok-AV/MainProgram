#include "eventManager.h"

bool EventManager::isLiftOff(std::queue<std::vector<double>> fifo){

}

bool EventManager::isApogee(std::queue<std::vector<double>> fifo){

}

bool EventManager::isReTrigger(double alt){

}

bool EventManager::isTouchDown(std::queue<std::vector<double>> fifo){

}

bool EventManager::trigger(){
    //TODO allume l'e-match
    triggered = true;
}

bool EventManager::hasTriggered(){
    return triggered;
}