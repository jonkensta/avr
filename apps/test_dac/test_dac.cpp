#include <DAC/MCP4911.h>

const uint16_t SIN_TABLE[] = {
#include "sin_table.inc"
};

uint16_t lookup_sin(uint16_t index) {
    const uint16_t mask = 0xFFFF;  // 1023 points in table
    index &= mask;  // coerce index to fit table
    return SIN_TABLE[index];
}

const uint8_t SS_PIN = 10;
const uint8_t SPI_RATE = SPI_CLOCK_DIV2;
MCP4911 SPI_DAC = MCP4911(SPI_RATE, SS_PIN);


void setup() {
    SPI_DAC.begin();
}

void loop() {
    static uint16_t index = 0;
    const uint16_t value = lookup_sin(index);
    SPI_DAC.write(value);
    index++;
}
