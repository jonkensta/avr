#include <string.h>

#include <avr/sleep.h>

#include <Arduino.h>
#include <SPI.h>
#include <TimerOne.h>

#define LIN_OUT 1
#define FHT_N 256
#include <FHT.h>

#include <neopixel.h>

#include <AGC.h>
#include <ADC/MCP3201.h>

////////////
// Pixels //
////////////

#define LED_PIN         4
#define NUM_PIXELS      60
Neopixel pixels = Neopixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup_pixels(void) {
    pixels.begin();
}

////////////////////
// Sample capture //
////////////////////

const uint8_t SS_PIN = 10;
const uint8_t SPI_RATE = SPI_CLOCK_DIV2;
MCP3201 SPI_ADC = MCP3201(SPI_RATE, SS_PIN);

const uint16_t SAMPLER_ATTACK_RATE = 30000;
const uint8_t SAMPLER_MARGIN = 3;
ShiftAGC16 SAMPLER_AGC = ShiftAGC16(SAMPLER_ATTACK_RATE, SAMPLER_MARGIN);

volatile int16_t window[FHT_N];
volatile bool WINDOW_READY = false;
volatile bool TIME_EVENT = false;

void capture_sample_callback(void) {
    int16_t sample = SPI_ADC.read();

    bool event = SAMPLER_AGC.update(sample);
    if (event) TIME_EVENT = true;
    sample = SAMPLER_AGC.apply(sample);

    static uint16_t sample_index = 0;
    window[sample_index] = sample;
    sample_index++;

    if (sample_index >= FHT_N) {
        WINDOW_READY = true;
        sample_index = 0;
        Timer1.stop();
    }
}

void setup_sampler(void) {
    const uint16_t sampling_period_us = 200;
    Timer1.initialize(sampling_period_us);
    Timer1.attachInterrupt(capture_sample_callback);
    SPI_ADC.begin();
}

//////////////
// coloring //
//////////////

typedef enum {RED=0, BLUE, GREEN} color_mode_t;
color_mode_t color_mode = GREEN;
uint8_t colors[3][NUM_PIXELS];
const uint8_t COLOR_MAX = 255;

void setup_colors(void) {
    for (int ii=0; ii<NUM_PIXELS; ii++)
        colors[0][ii] = COLOR_MAX;
}

color_mode_t next_color_mode(color_mode_t mode) {
    switch (mode) {
        case RED:
            return BLUE;
        case BLUE:
            return GREEN;
        case GREEN:
        default:
            return RED;
    }
}

void dither_colors(uint8_t* colors, size_t len) {
    for (int ii=1; ii<len; ii++) {
        uint8_t lhs = colors[ii-1] >> 1;
        uint8_t rhs = colors[ii] >> 1;
        colors[ii] = lhs + rhs;
    }
}

///////////////////////////
// sleep, setup and main //
///////////////////////////

void setup_sleep(void) {
    set_sleep_mode(SLEEP_MODE_IDLE);
}

void setup() {
    setup_pixels();
    setup_sampler();
    setup_sleep();
    setup_colors();
}

int map_pixel_to_bin(int pixel) {
    if (pixel <= 32)
        return pixel + 1;
    else if (pixel <= (32+16))
        return 2 * (pixel - (32)) + 32;
    else if (pixel <= (32+16+8))
        return 4 * (pixel - (32+16)) + 64;
    else if (pixel <= (32+16+8+4))
        return 8 * (pixel - (32+16+8)) + 96;
    else
        return 0;
}

void loop() {
    cli();
    if (!WINDOW_READY) {
        sleep_enable();
        sei();
        sleep_cpu();
        sleep_disable();
        return;
    }

    memcpy(fht_input, (const void*)window, sizeof(int16_t) * FHT_N);

    WINDOW_READY = false;
    Timer1.resume();
    sei();

    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_lin(); // take the output of the fht

    static uint16_t avg_mag[FHT_N/2];
    const uint16_t decay = 1;
    for (int bin=0; bin<FHT_N/2; bin++) {
        uint16_t lhs = fht_lin_out[bin];
        uint16_t rhs = avg_mag[bin];
        uint16_t result = (lhs + ((1 << decay) - 1) * rhs) >> decay;
        avg_mag[bin] = max(result, lhs);
    }

    for (int pixel=0; pixel<NUM_PIXELS; pixel++) {
        int bin = map_pixel_to_bin(pixel);
        uint8_t r = (avg_mag[bin] * colors[0][pixel]) >> 8;
        uint8_t b = (avg_mag[bin] * colors[1][pixel]) >> 8;
        uint8_t g = (avg_mag[bin] * colors[2][pixel]) >> 8;
        pixels.setPixelColor(pixel, r, g, b);
    }
    pixels.show();

    if (TIME_EVENT) {
        color_mode = next_color_mode(color_mode);
        TIME_EVENT = false;
    }

    for (int ii=0; ii<3; ii++)
        colors[ii][0] = 0;

    switch (color_mode) {
        case RED:   colors[0][0] = COLOR_MAX; break;
        case BLUE:  colors[1][0] = COLOR_MAX; break;
        case GREEN: colors[2][0] = COLOR_MAX; break;
    }
    for (int ii=0; ii<3; ii++)
        dither_colors(&(colors[ii][0]), NUM_PIXELS);
}
