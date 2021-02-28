#if !defined(STORAGE_H)
#define STORAGE_H
#include <Vector.h>

typedef Vector<double*> Buffer;

class Storage {

public: 
    Storage();

    bool saveMemory(const Buffer& buffer);
    bool saveSD(const Buffer& buffer);
    bool sdTransfer();
private: 
};


#endif // STORAGE_H
