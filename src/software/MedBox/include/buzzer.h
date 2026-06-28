#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "config.h" 

class Buzzer {
public:
    void begin();
    // Metoda wyzwalająca dźwięk na określony czas (domyślnie 200ms)
    void triggerBeep(unsigned long duration = 200); 
    // Metoda, która musi być stale wywoływana w loop()
    void update(); 

private:
    unsigned long beepStartTime = 0;
    unsigned long beepDuration = 0;
    bool isActive = false;
};

extern Buzzer buzzer; // Globalna instancja

#endif