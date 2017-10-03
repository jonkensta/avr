#include <Arduino.h>

#include "MCP23017.h"

void MCP23017::begin(void) {
    set_pin_mode(OUTPUT);  // OUTPUT by default
}

uint16_t MCP23017::read() {
    Wire.beginTransmission(_address);
    Wire.write(MCP23017_GPIOA);
    Wire.endTransmission();

    const uint8_t num_bytes = 2;
    Wire.requestFrom(_address, num_bytes);

    uint16_t value = ((uint16_t)Wire.read() << 8);
    value |= Wire.read();

    return value;
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
    mode = (mode) ? 0xFF: 0x00;
    Wire.beginTransmission(_address);
    Wire.write(MCP23017_IODIRA);
    Wire.write(mode);
    Wire.write(mode);
    Wire.endTransmission();
}

void MCP23017::set_pull_up(uint8_t mode) {
    mode = (mode) ? 0xFF : 0x00;
    Wire.beginTransmission(_address);
    Wire.write(MCP23017_GPPUA);
    Wire.write(mode);
    Wire.write(mode);
    Wire.endTransmission();
}
