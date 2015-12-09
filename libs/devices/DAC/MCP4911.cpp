#include <SPI.h>
#include "MCP4911.h"

void MCP4911::begin(void) {
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(_rate);

    pinMode(_ss_pin, OUTPUT);
    digitalWrite(_ss_pin, HIGH);
}

void MCP4911::end(void) {
    SPI.end();
}

void MCP4911::write(uint16_t value) {
    // Remove non-data bits
    const uint8_t num_bits = 10;  // number of bits of precision
    const uint16_t data_mask = (1 << num_bits) - 1;  // 
    value &= data_mask;

    uint8_t high = (uint8_t)(value >> 8);
    SPI.transfer(high);

    uint8_t low = (uint8_t)value;
    SPI.transfer(low);
}
