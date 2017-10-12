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
    Wire.endTransmission();

    Wire.beginTransmission(_address);
    Wire.write(MCP23017_GPIOB);
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
    Wire.endTransmission();

    Wire.beginTransmission(_address);
    Wire.write(MCP23017_IODIRB);
    Wire.write(mode);
    Wire.endTransmission();
}

void MCP23017::set_pull_up(bool value) {
    uint8_t mode = (value) ? 0xFF : 0x00;

    Wire.beginTransmission(_address);
    Wire.write(MCP23017_GPPUA);
    Wire.write(mode);
    Wire.endTransmission();

    Wire.beginTransmission(_address);
    Wire.write(MCP23017_GPPUB);
    Wire.write(mode);
    Wire.endTransmission();
}
