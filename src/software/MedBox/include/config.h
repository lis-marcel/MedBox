#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// I2C pins for the ESP8266
#define PIN_I2C_SDA 5
#define PIN_I2C_SCL 4

// Pins for the test LEDs
#define PIN_LED_RED 12
#define PIN_LED_BLUE 13

// Additional pins for the MCP23017 and buzzer
#define PIN_MCP_INIT 14
#define PIN_BUZZER 16

// I2C addresses for the devices
#define ADDR_MCP_I2C 0x20
#define ADDR_DS3231_I2C 0x68

#define NUM_BOXES 8

#endif