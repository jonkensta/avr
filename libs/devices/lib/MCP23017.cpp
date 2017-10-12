#include <Arduino.h>

#include "MCP23017.h"

void MCP23017::begin(void) {
    Wire.begin();
    set_pin_mode(OUTPUT);  // OUTPUT by default
}

void MCP23017::write(uint16_t value) {
    Wire.beginTransmission(_address);
    Wire.write(MCP23017_GPIOA);
    Wire.write(value & 0xFF);
    Wire.write(value >> 8);
    Wire.endTransmission();
}

/**
 * Sets the pin mode to either INPUT or OUTPUT
 */
void MCP23017::set_pin_mode(uint8_t mode) {
    mode = (mode == INPUT) ? 0xFF : 0x00;
    Wire.beginTransmission(_address);
    Wire.write(MCP23017_IODIRA);
    Wire.write(mode);
    Wire.write(mode);
    Wire.endTransmission();
}

/**
 * Sets the pull-up either on or off
 */
void MCP23017::set_pull_up(uint8_t level) {
    uint8_t mode = (level == LOW) ? 0x00 : 0xFF;
    Wire.beginTransmission(_address);
    Wire.write(MCP23017_GPPUA);
    Wire.write(mode);
    Wire.write(mode);
    Wire.endTransmission();
}
