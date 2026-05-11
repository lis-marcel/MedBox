#ifndef PERIPHERIALS_H
#define PERIPHERIALS_H

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include "config.h"

class Peripherials {
public:
    bool begin();
    void setLed(uint8_t boxIndex, bool state);
    bool isButtonPressed(uint8_t boxIndex);

private:
    Adafruit_MCP23X17 mcp;
};

extern Peripherials periph;

#endif