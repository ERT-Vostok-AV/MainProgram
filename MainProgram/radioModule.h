#if !defined(RADIO_MODULE_H)
#define RADIO_MODULE_H

#include <Vector.h>


class RadioModule {

public:
    bool begin();

    bool pack(double* data);
    bool send();
    bool packSend(double* data);
private:
    String packedData;
};

#endif // RADIO_MODULE_H
