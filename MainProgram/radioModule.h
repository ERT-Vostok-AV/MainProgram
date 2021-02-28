#if !defined(RADIO_MODULE_H)
#define RADIO_MODULE_H

#include <Vector.h>

typedef Vector<double> Data;

class RadioModule {

public:
    bool begin();

    bool pack(const Data& data);
    bool send();
    bool packSend(const Data& data);
private:
    String packedData;
};

#endif // RADIO_MODULE_H
