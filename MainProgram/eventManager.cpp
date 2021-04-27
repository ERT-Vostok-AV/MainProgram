#include "eventManager.h"

/* Unites pour les constantes:
	- Altitude : m
	- Vitesse : m/s
*/

const double LIFTOFF_ALTITUDE = 10;			// Altitude minimum pour detecter le decollage
const double LIFTOFF_SPEED = 10;		// Vitesse minimum pour le detection du decollage

// Idee c'est d'etre sur que la vitesse verticale est bien negatif, et donc que l'apogee est passe
// En gros on va comparer maxAlt et l'altitude actuel, si la difference est plus grande que cette valeur alors il detecte que la fusee redescend.
const double APOGEE_DETECTION_THRESHOLD = 3;	  
const double RE_TRIGGER_ALTITUDE = 300;
const double TOUCHDOWN_MAX_ALTITUDE = 20;		// Si il n'est pas descendu en dessous de cette altitude il ne peut pas declarer Touchdown (retourner true)
const double TOUCHDOWN_MAX_SPEED = 1/2;	

EventManager::EventManager() : triggered(false) {}

bool EventManager::isLiftOff(double alt, double velZ){
	bool data1 = alt > LIFTOFF_ALTITUDE;
	bool data2 = velZ > LIFTOFF_SPEED;
	return data1 or data2;	// and ou or ?
}

bool EventManager::isApogee(double alt, double velZ){
	if (alt > maxAlt){
	  maxAlt = alt;
	}
	bool data1 = velZ < 0;					// detecte vitesse verticale negative
	bool data2 = (maxAlt - alt) > APOGEE_DETECTION_THRESHOLD;	// detecte altitude qui redescent
	return data1 and data2; // and ou or?
	}

bool EventManager::isReTrigger(double alt){
	return alt <= RE_TRIGGER_ALTITUDE; 
}

bool EventManager::isTouchDown(double alt, double velZ){
	bool data1 = alt < RE_TRIGGER_ALTITUDE;								// verifie que c'est apres la descente
	bool data2 = abs(velZ) < TOUCHDOWN_MAX_SPEED;
  // Peut etre verifie si stagne (buffer ?)
 
	return (data2 and data1);
}

void EventManager::trigger(){
    //TODO allume l'e-match
    triggered = true;
}

bool EventManager::hasTriggered(){
    return triggered;
}
