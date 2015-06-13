#include <SPI.h>
#include "MCP3201.h"

void MCP3201::begin(void) {
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(_rate);

    pinMode(_ss_pin, OUTPUT);
    digitalWrite(_ss_pin, HIGH);
}

void MCP3201::end(void) {
    SPI.end();
}

uint16_t MCP3201::read_spi(void) {
    // Signal start of transfer
    digitalWrite(_ss_pin, LOW);

    // Transfer data
    uint8_t dummy = 0;
    byte high = SPI.transfer(dummy);
    byte low = SPI.transfer(dummy);

    // Signal stop of transfer
    digitalWrite(_ss_pin, HIGH);

    return ((uint16_t)high << 8) | low;
}

int16_t MCP3201::read(void) {
    int16_t value = read_spi();

    // Remove non-data bits
    const int16_t data_mask = 0x1FFF;
    value &= data_mask;

    // Remove extra LSB1 bit
    value >>= 1;

    // Adjust center to zero
    const int16_t zero_point = (1 << 11);
    value -= zero_point;

    return value;
}
