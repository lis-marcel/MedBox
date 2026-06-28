#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "peripherials.h"
#include "buzzer.h"

// Czas ostatniego zdarzenia
unsigned long previousMillis = 0;

// Interwał testowy: 5 sekund (5000 milisekund)
const unsigned long interval = 5000; 
// DLA DOCELOWYCH 24 GODZIN:
// const unsigned long interval = 24UL * 60UL * 60UL * 1000UL;

uint8_t currentActiveBox = 0; // Śledzi, która skrytka ma się teraz zapalić

void setup() {
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_BLUE, LOW);

    Serial.begin(115200);
    delay(1000);

    Wire.begin(PIN_I2C_SDA,  PIN_I2C_SCL);

    buzzer.begin();

    if (!periph.begin()) {
        digitalWrite(PIN_LED_RED, HIGH);
        while(1); 
    }

    // Najpierw gasimy wszystkie diody, żeby świeciła tylko jedna
    for (int i = 0; i < NUM_BOXES; i++) {
        periph.setLed(i, false);
    }
}

void loop() {
    // 1. Pobieramy aktualny czas działania mikrokontrolera (w milisekundach)
    unsigned long currentMillis = millis();

    // 2. Sprawdzamy, czy różnica między obecnym czasem a ostatnim zdarzeniem
    //    jest większa lub równa naszemu interwałowi (5000 ms)
    if (currentMillis - previousMillis >= interval) {
        
        // Zapisujemy obecny czas jako punkt odniesienia dla kolejnego cyklu
        previousMillis = currentMillis;

        // Zapalamy diodę dla aktualnej skrytki
        periph.setLed(currentActiveBox, true);

        buzzer.triggerBeep(200); // Wyzwalamy dźwięk na 200 ms

        // Zwiększamy licznik skrytek na poczet kolejnego cyklu
        currentActiveBox++;
        
        // Jeśli doszliśmy do końca (np. 7 skrytek), wracamy do zera
        if (currentActiveBox >= NUM_BOXES) {
            currentActiveBox = 0; 
        }
    }

    for (uint8_t i = 0; i < NUM_BOXES; i++) {
        if (periph.isButtonPressed(i)) {
            periph.setLed(i, false);
        }
    }

    // Aktualizujemy stan buzzera
    buzzer.update(); 
}