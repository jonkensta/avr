#include <avr/pgmspace.h>
#include <DAC/MCP4911.h>

/* SIN table data and methods */
const uint16_t SIN_TABLE[] PROGMEM = {
#include "sin_table.inc"
};

uint16_t lookup_sin_value(uint16_t index) {
    const uint16_t mask = 0x03FF;  // 1023 points in table
    index &= mask;  // coerce index to fit table
    uint16_t value = pgm_read_word(&(SIN_TABLE[index]));
    return value;
}

/* SPI declarations */
const uint8_t SS_PIN = 10;
const uint8_t SPI_RATE = SPI_CLOCK_DIV2;
MCP4911 SPI_DAC = MCP4911(SPI_RATE, SS_PIN);

/* Setup */ 
void setup() {
    SPI_DAC.begin();
}

/* Main */
void loop() {
    static uint16_t index = 0;
    const uint16_t value = lookup_sin_value(index);
    SPI_DAC.write(value);
    index++;
}
