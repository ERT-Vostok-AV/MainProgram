#if !defined(BUZZER_H)
#define BUZZER_H

class Buzzer {

public:
    Buzzer(int pin);
    
    void error();
    void initStart();
    void initSuccess();
    void beacon();
private:
    int pin;

    void bip(int freq, int duration);
};

#endif // BUZZER_H
