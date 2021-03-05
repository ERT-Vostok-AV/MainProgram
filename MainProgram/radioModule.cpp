#include "radioModule.h"

/**Error codes:
 * 0 no error
 * 
 */

int RadioModule::begin(){
    //TODO Initialisation
}

bool RadioModule::pack(FlightData data){
    
}

bool RadioModule::send(){

}

bool RadioModule::packSend(FlightData data){
    pack(data);
    send();
}
