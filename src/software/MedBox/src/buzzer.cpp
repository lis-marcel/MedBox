#include "buzzer.h"

Buzzer buzzer;

void Buzzer::begin() {
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW); 
}

void Buzzer::triggerBeep(unsigned long duration) {
    beepDuration = duration;
    beepStartTime = millis();
    isActive = true;
    
    // ZMIANA: Zamiast prądu stałego, generujemy dźwięk.
    // 4000 to częstotliwość w Hercach (Hz)
    tone(PIN_BUZZER, 4000); 
}

void Buzzer::update() {
    if (isActive && (millis() - beepStartTime >= beepDuration)) {
        isActive = false;
        
        noTone(PIN_BUZZER); 
        
        // Dla pewności zostawiamy pin w stanie niskim, 
        // żeby nie grzać cewki buzzera i tranzystora
        digitalWrite(PIN_BUZZER, LOW); 
    }
}