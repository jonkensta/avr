#include <string.h>

#include <avr/sleep.h>

#include <Arduino.h>
#include <SPI.h>
#include <TimerOne.h>

#define LIN_OUT 1
#define FHT_N 256
#include <FHT.h>

#include <neopixel.h>

////////////
// Pixels //
////////////

#define LED_PIN         4
#define NUM_PIXELS      60
Neopixel pixels = Neopixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup_pixels(void) {
    pixels.begin();
}

/////////////////
// SPI and ADC //
/////////////////

const int slaveSelectPin = 10;

void setup_spi(void) {
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV2);

    pinMode(slaveSelectPin, OUTPUT);
    digitalWrite(slaveSelectPin, HIGH);
}

uint16_t read_spi(void) {
    // Signal start of transfer
    digitalWrite(slaveSelectPin, LOW);

    // Transfer data
    uint8_t dummy = 0;
    byte high = SPI.transfer(dummy);
    byte low = SPI.transfer(dummy);

    // Signal stop of transfer
    digitalWrite(slaveSelectPin, HIGH);

    return ((uint16_t)high << 8) | low;
}

int16_t read_adc(void) {
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

////////////////////
// Sample capture //
////////////////////

volatile uint16_t shift = 0;

volatile int16_t window[FHT_N];
volatile bool window_ready = false;
volatile bool temporal_event = false;

uint16_t calculate_mask(uint16_t shift, const uint8_t margin) {
    return ~((1 << (15 - (shift + margin))) - 1);
}

uint16_t update_shift(uint16_t shift, int16_t sample) {
    const uint8_t margin = 3;
    const uint16_t attack_rate = 30000;
    static uint16_t attack_count = 0;
    static uint16_t mask = 0x0000;

    uint16_t abs_sample = (sample >= 0) ? (sample) : (-sample);

    if (!(abs_sample & mask)) {
        attack_count++;

        if (attack_count >= attack_rate) {
            attack_count = 0;
            shift++;
            mask = calculate_mask(shift, margin);
        }
    }
    else {
        temporal_event = true;
        shift--;
        mask = calculate_mask(shift, margin);
    }

    return shift;
}

void capture_sample(void) {
    int16_t sample = read_adc();

    shift = update_shift(shift, sample);
    sample <<= shift;

    static uint16_t sample_index = 0;
    window[sample_index] = sample;
    sample_index++;

    if (sample_index >= FHT_N) {
        window_ready = true;
        sample_index = 0;
        Timer1.stop();
    }
}

void setup_sampler(void) {
    const uint16_t sampling_period_us = 200;
    Timer1.initialize(sampling_period_us);
    Timer1.attachInterrupt(capture_sample);
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
    setup_spi();
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
    if (!window_ready) {
        sleep_enable();
        sei();
        sleep_cpu();
        sleep_disable();
        return;
    }

    memcpy(fht_input, (const void*)window, sizeof(int16_t) * FHT_N);

    window_ready = false;
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

    if (temporal_event) {
        color_mode = next_color_mode(color_mode);
        temporal_event = false;
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
