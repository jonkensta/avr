#include <string.h>

#include <avr/sleep.h>

#include <Arduino.h>
#include <SPI.h>
#include <TimerOne.h>

#define LIN_OUT 1
#define FHT_N 128
#include <FHT.h>

#include <neopixel.h>

#include <AGC.h>
#include <MCP3201.h>

const uint8_t NUM_BINS = FHT_N >> 1;

////////////
// Pixels //
////////////

const uint8_t LED_PIN = 4;
const uint16_t NUM_PIXELS = 300;
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

volatile bool INPUT_READY = false;

void capture_sample_callback(void) {
    int16_t sample = SPI_ADC.read();

    SAMPLER_AGC.update(sample);
    sample = SAMPLER_AGC.apply(sample);

    static uint16_t sample_index = 0;
    fht_input[sample_index] = sample;
    sample_index++;

    if (sample_index >= FHT_N) {
        INPUT_READY = true;
        sample_index = 0;
        Timer1.stop();
    }
}

void setup_sampler(void) {
    const uint16_t sampling_period_us = 350;
    Timer1.initialize(sampling_period_us);
    Timer1.attachInterrupt(capture_sample_callback);
    SPI_ADC.begin();
}

////////////
// colors //
////////////

void next_color_state(uint8_t& r, uint8_t& b, uint8_t& g)
{
    static uint8_t _r = 255, _b = 0, _g = 0;

    if (_r > 0 && _g == 0) {
        _r--;
        _b++;
    } else if (_b > 0 && _r == 0) {
        _b--;
        _g++;
    } else {
        _g--;
        _r++;
    }

    r = _r;
    b = _b;
    g = _g;
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
}

void loop() {
    cli();
    if (!INPUT_READY) {
        sleep_enable();
        sei();
        sleep_cpu();
        sleep_disable();
        return;
    }

    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_lin(); // take the output of the fht

    const uint8_t num_bands = 8;
    static ShiftAGC_U16 AGC[num_bands] = {
        ShiftAGC_U16(5000, 500),
        ShiftAGC_U16(5000, 500),
        ShiftAGC_U16(5000, 500),
        ShiftAGC_U16(5000, 500),
        ShiftAGC_U16(5000, 500),
        ShiftAGC_U16(5000, 500),
        ShiftAGC_U16(5000, 500),
        ShiftAGC_U16(5000, 500),
    };
    for (int bin=1; bin<NUM_BINS; bin++) {
        int band = bin / (NUM_BINS / num_bands);
        AGC[band].update(fht_lin_out[bin]);
        fht_lin_out[bin] = AGC[band].apply(fht_lin_out[bin]);
    }

    static uint8_t avg_mag[NUM_BINS];
    for (uint8_t bin=0; bin<NUM_BINS; bin++) {
        uint8_t lhs = fht_lin_out[bin] >> 8;
        uint8_t rhs = avg_mag[bin];
        uint8_t result = (lhs >> 1) + (rhs >> 1);
        avg_mag[bin] = max(result, lhs);
    }

    INPUT_READY = false;
    Timer1.resume();
    sei();

    uint8_t r, g, b;
    next_color_state(r, g, b);
    const uint8_t factor = (NUM_PIXELS + (NUM_BINS >> 1)) / NUM_BINS;
    for (uint16_t pixel=0; pixel<NUM_PIXELS; pixel++) {
        uint8_t brightness = avg_mag[pixel / factor + 2];
        uint8_t _r = ((uint16_t)r * brightness) >> 8;
        uint8_t _g = ((uint16_t)g * brightness) >> 8;
        uint8_t _b = ((uint16_t)b * brightness) >> 8;
        pixels.setPixelColor(pixel, _r, _g, _b);
    }

    pixels.show();
}
