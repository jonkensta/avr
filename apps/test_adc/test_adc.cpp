#include <util/delay.h>

#include <Arduino.h>
#include <SPI.h>

#include <neopixel.h>
#include <haar.h>

#define ABS(x) (((x) > 0) ? (x) : -(x))

const int slaveSelectPin = 10;

#define WINDOW_LEN     64
int8_t window[WINDOW_LEN];
int8_t workspace[WINDOW_LEN];

#define UINT8_MAX      255

#define LED_PIN        4
#define NUMPIXELS      60
Neopixel pixels = Neopixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

uint16_t read_spi(void) {
    uint8_t dummy = 0;
    digitalWrite(slaveSelectPin, LOW);
    byte high = SPI.transfer(dummy);
    byte low = SPI.transfer(dummy);
    digitalWrite(slaveSelectPin, HIGH);
    return ((uint16_t)high << 8) | low;
}

int16_t read_adc(void) {
    int16_t value = read_spi();
    value &= 0x1FFF;
    value >>= 1;
    value -= (1 << 11);
    return value;
}

void reset_leds(void) {
    for(int i=0;i<NUMPIXELS;i++)
        pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
}

void leds_write_value(uint8_t value, uint32_t color) {
    uint8_t num_leds = ((uint16_t)NUMPIXELS * value) / (UINT8_MAX);
    for(int i=0;i<num_leds;i++)
        pixels.setPixelColor(i, color);
    pixels.show(); // This sends the updated pixel color to the hardware.
}

void setup() {
    pixels.begin();

    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    pinMode(slaveSelectPin, OUTPUT);
    digitalWrite(slaveSelectPin, HIGH);
}

void loop() {
    for(int ii=0; ii<WINDOW_LEN; ii++)
        window[ii] = (read_spi() >> 4);

    haar64(window, workspace);

    const int num_bins = min(NUMPIXELS, WINDOW_LEN);
    for(int ii=1; ii<num_bins; ii++) {
        uint8_t brt = ABS(window[ii]); // brightness
        uint32_t color = pixels.Color(brt, brt, brt);
        pixels.setPixelColor(ii, color);
    }
    pixels.show();
}
