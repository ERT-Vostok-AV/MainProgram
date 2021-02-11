#if !defined(BUZZER_H)
#define BUZZER_H

class Buzzer {

public:
    Buzzer();
    
    void error();
    void initStart();
    void initSuccess();
    void beacon();
private:
};

#endif // BUZZER_H
