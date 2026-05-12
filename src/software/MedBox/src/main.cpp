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

    for (int i = 0; i < NUM_BOXES; i++) {
        Serial.println(i);

        periph.setLed(i, true);
        delay(200);
    }
}

void loop() {
    for (uint8_t i = 0; i < NUM_BOXES; i++) {
        if (periph.isButtonPressed(i)) {
            periph.setLed(i, false);
        }
    }
}