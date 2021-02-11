#if !defined(RADIO_MODULE_H)
#define RADIO_MODULE_H

#include <vector>
#include <string>

typedef std::vector<double> Data;

class RadioModule {

public:
    bool pack(const Data& data);
    bool send();
    bool packSend(const Data& data);
private:
    std::string packedData;
};

#endif // RADIO_MODULE_H
