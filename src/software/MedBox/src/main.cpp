#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "peripherials.h"

void setup() {
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_BLUE, LOW);

    Serial.begin(115200);
    delay(1000);

    Wire.begin(PIN_I2C_SDA,  PIN_I2C_SCL); 

    if (!periph.begin()) {
        digitalWrite(PIN_LED_RED, HIGH);
        while(1); 
    }

    digitalWrite(PIN_LED_BLUE, HIGH);
}

void loop() {
    // Przechodzimy po kolei przez wszystkie diody (od 0 do 6 lub 7)
    for (int i = 0; i < NUM_BOXES; i++) {
        Serial.println(i);

        periph.setLed(i, true);  // Zapal diodę i
        delay(200);              // Czekaj 200ms
        periph.setLed(i, false); // Zgaś diodę i
    }

    delay(500); // Przerwa przed kolejnym przebiegiem wszystkich skrytek
}