#include <Arduino.h>
#include <SPI.h>

#include <avr/sleep.h>
#include <avr/interrupt.h>

const uint8_t LED_PIN = 4;
const uint16_t NUM_PIXELS = 300;

Neopixel pixels = Neopixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

volatile int index = 0;
const uint16_t NUM_BYTES = 3*NUM_PIXELS;

volatile bool show_pixels = false;
uint8_t* const pixels = pixels.getPixels();


void setup_pixels(void) {
    pixels.begin();
}


void setup_spi(void) {
    // we're operating as the slave
    pinMode(MISO, OUTPUT);

    // turn on SPI in slave mode
    SPCR |= _BV(SPE) | _BV(SPIE)

    // now turn on interrupts
    SPI.attachInterrupt();
}


void setup (void) {
    setup_pixels();
    setup_spi();
    setup_sleep();
}


// SPI interrupt routine
ISR (SPI_STC_vect) {
    pixels[index++] = SPDR;
    if (index >= NUM_BYTES) {
        index = 0;
        show_pixels = true;  // set show_pixels flag
    }
    SPDR = index;
}


void sleep(void) {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
}


void loop (void) {
    if(show_pixels) {
        show_pixels = false;
        pixels.show();
    } else {
        sleep();
    }
}
