#include "peripherials.h"

Peripherials periph;

bool Peripherials::begin() {
    if (!mcp.begin_I2C(ADDR_MCP_I2C)) {
        digitalWrite(PIN_LED_RED, HIGH); // Indicate error with red LED
        return false;
    }

    for (uint8_t i = 0; i < NUM_BOXES; i++) {
        // Buttons on port A
        mcp.pinMode(i, INPUT_PULLUP); 

        // LEDs on port B
        mcp.pinMode(i + NUM_BOXES, OUTPUT);
        mcp.digitalWrite(i + NUM_BOXES, LOW);
    }

    return true;
}

void Peripherials::setLed(uint8_t boxIndex, bool state) {
    if (boxIndex < NUM_BOXES) {
        mcp.digitalWrite(boxIndex + NUM_BOXES, state ? HIGH : LOW);
    }
}

bool Peripherials::isButtonPressed(uint8_t boxIndex) {
    if (boxIndex < NUM_BOXES) {
        return mcp.digitalRead(boxIndex) == LOW;
    }
    return false;
}