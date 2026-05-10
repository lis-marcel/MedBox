#include <Arduino.h>
#include <Wire.h>
#include "config.h"

void setup() {
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);

    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_BLUE, LOW);

    Serial.begin(115200);
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL); // Twoje niestandardowe piny

    Wire.beginTransmission(ADDR_MCP_I2C);

    if (Wire.endTransmission() == 0) {
        digitalWrite(PIN_LED_BLUE, HIGH);
    } 

    delay(1000);
    digitalWrite(PIN_LED_BLUE, LOW);

    Wire.beginTransmission(ADDR_DS3231);
    if (Wire.endTransmission() == 0) {
        digitalWrite(PIN_LED_RED, HIGH);
    } 

  }

void loop() {
    // Tutaj będzie logika: 
    // 1. Sprawdź czy czas z RTC zgadza się z alarmem.
    // 2. Jeśli tak -> zapal LED i włącz Buzzer.
    // 3. Czekaj na przycisk przez ekspander -> wyłącz alarm.
}