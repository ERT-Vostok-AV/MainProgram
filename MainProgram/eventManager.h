#if !defined(EVENT_MANAGER_H)
#define EVENT_MANAGER_H

#include <queue>
#include <vector>

class EventManager {

public: 


    EventManager();
    bool isLiftOff(std::queue<std::vector<double>> fifo);
    bool isTouchDown(std::queue<std::vector<double>> fifo);
    bool isApogee(std::queue<std::vector<double>> fifo);
    bool isReTrigger(double alt);
    bool trigger();
    bool hasTriggered();
private:
    bool triggered;
};



#endif // EVENT_MANAGER_H
