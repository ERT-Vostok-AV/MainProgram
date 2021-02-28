#include "radioModule.h"

bool RadioModule::begin(){
    //TODO Initialisation
}

bool RadioModule::pack(double* data){
    
}

bool RadioModule::send(){

}

bool RadioModule::packSend(double* data){
    pack(data);
    send();
}
