#include "radioModule.h"

bool RadioModule::pack(const Data& data){
    
}

bool RadioModule::send(){

}

bool RadioModule::packSend(const Data& data){
    pack(data);
    send();
}