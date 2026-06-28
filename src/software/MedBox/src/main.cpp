#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "peripherials.h"
#include "buzzer.h"
#include "web_server.h"
#include "rtc_module.h"

unsigned long previousMillis = 0;
const unsigned long interval = 5000; 
uint8_t currentActiveBox = 0; 

void setup() {
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_BLUE, LOW);

    Serial.begin(115200);
    delay(1000);

    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

    buzzer.begin();

    if (!periph.begin()) {
        digitalWrite(PIN_LED_RED, HIGH);
        while(1); 
    }

    for (int i = 0; i < NUM_BOXES; i++) {
        periph.setLed(i, false);
    }
    
    myRtc.begin();

    webUI.begin(); 
}

void loop() {
    // 3. Nasłuchujemy klientów na stronie www
    webUI.handleClient(); 

    myRtc.saveHeartbeat();
    
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        periph.setLed(currentActiveBox, true);
        buzzer.triggerBeep(200); 
        currentActiveBox++;
        if (currentActiveBox >= NUM_BOXES) {
            currentActiveBox = 0; 
        }
    }

    for (uint8_t i = 0; i < NUM_BOXES; i++) {
        if (periph.isButtonPressed(i)) {
            periph.setLed(i, false);
        }
    }

    buzzer.update(); 
}