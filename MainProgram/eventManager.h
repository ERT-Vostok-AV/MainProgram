#if !defined(EVENT_MANAGER_H)
#define EVENT_MANAGER_H

#include <queue>
#include <vector>

typedef std::vector<double> Data;

class EventManager {

public:
    EventManager();

    bool isLiftOff(std::queue<Data> fifo);
    bool isTouchDown(std::queue<Data> fifo);
    bool isApogee(std::queue<Data> fifo);
    bool isReTrigger(double alt);
    bool trigger();
    bool hasTriggered();
private:
    bool triggered;
};



#endif // EVENT_MANAGER_H
