#include <string.h>

#include <avr/sleep.h>
#include <avr/pgmspace.h>

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

ShiftAGC_I16 SAMPLER_AGC = ShiftAGC_I16(5000, 5);
ShiftAGC_I16 TEMPORAL_AGC = ShiftAGC_I16(10000, 1000);

volatile int16_t window[FHT_N];
volatile bool WINDOW_READY = false;
volatile bool TIME_EVENT = false;

void capture_sample_callback(void) {
    int16_t sample = SPI_ADC.read();

    SAMPLER_AGC.update(sample);
    sample = SAMPLER_AGC.apply(sample);

    bool event = TEMPORAL_AGC.update(sample);
    if (event) TIME_EVENT = true;

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

void dither_colors(uint8_t* colors) {
    for (int ii=1; ii<NUM_PIXELS; ii++) {
        uint8_t lhs = (3 * (uint16_t)colors[ii]) >> 2;
        uint8_t rhs = (colors[ii-1]) >> 2;
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
    return pixel + 1;
//    if (pixel <= 32)
//        return pixel + 1;
//    else if (pixel <= (32+16))
//        return 2 * (pixel - (32)) + 32;
//    else if (pixel <= (32+16+8))
//        return 4 * (pixel - (32+16)) + 64;
//    else if (pixel <= (32+16+8+4))
//        return 8 * (pixel - (32+16+8)) + 96;
//    else
//        return 0;
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

    static ShiftAGC_U16 AGC[4] = {
        ShiftAGC_U16(5000, 500),
        ShiftAGC_U16(5000, 500),
        ShiftAGC_U16(5000, 500),
        ShiftAGC_U16(5000, 500),
    };
    for (int pixel=0; pixel<NUM_PIXELS; pixel++) {
        int band = pixel / (NUM_PIXELS / 4);
        uint16_t bin = map_pixel_to_bin(pixel);
        AGC[band].update(fht_lin_out[bin]);
        fht_lin_out[bin]= AGC[band].apply(fht_lin_out[bin]);
    }

    static uint8_t avg_mag[NUM_PIXELS];
    for (int pixel=0; pixel<NUM_PIXELS; pixel++) {
        uint16_t bin = map_pixel_to_bin(pixel);
        uint8_t lhs = fht_lin_out[bin] >> 8;
        uint8_t rhs = avg_mag[pixel];
        uint8_t result = (lhs >> 1) + (rhs >> 1);
        avg_mag[pixel] = max(result, lhs);
    }

    for (int pixel=0; pixel<NUM_PIXELS; pixel++) {
        uint8_t r = ((uint16_t)avg_mag[pixel] * colors[0][pixel]) >> 8;
        uint8_t b = ((uint16_t)avg_mag[pixel] * colors[1][pixel]) >> 8;
        uint8_t g = ((uint16_t)avg_mag[pixel] * colors[2][pixel]) >> 8;
        pixels.setPixelColor(pixel, r, g, b);
    }
    pixels.show();

    if (TIME_EVENT) {
        color_mode = next_color_mode(color_mode);
        TIME_EVENT = false;
    }

    for (int ii=0; ii<3; ii++)
        colors[ii][0] = 0;

    int color_index;
    switch (color_mode) {
        case RED:   color_index = 0; break;
        case BLUE:  color_index = 1; break;
        case GREEN: color_index = 2; break;
    }
    colors[color_index][0] = COLOR_MAX;
    for (int ii=0; ii<3; ii++)
        dither_colors(&(colors[ii][0]));
}
