#include <neopixel.h>
#include <util/delay.h>

#define PIN            4
#define NUM_ROWS       8
#define NUM_COLS       34
#define NUM_PIXELS     (NUM_ROWS*NUM_COLS)
#define BRIGHTNESS     125

Neopixel pixels = Neopixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int DELAY = 40; // delay in ms
uint32_t ON = pixels.Color(0, 0, 255);
const uint32_t OFF = pixels.Color(0, 0, 0);


void setup() {
    pixels.begin();
    pixels.setBrightness(BRIGHTNESS);
}

bool
index_is_even(int index) {
    return (index % 2 == 0);
}

uint16_t
index_to_pixel(uint8_t row, uint8_t col) {
    uint16_t pixel;
    if (index_is_even(row)) {
        pixel = (NUM_COLS * row) + col;
    } else {
        pixel = (NUM_COLS * (row + 1)) - col - 1;
    }
    return pixel;
}

void
set_column(uint8_t col, uint32_t color) {
    for (uint8_t row=0; row<NUM_ROWS; row++) {
        uint16_t pixel = index_to_pixel(row, col);
        pixels.setPixelColor(pixel, color);
    }
}

uint32_t
get_next_color(uint32_t current_color) {
    uint8_t r = (uint8_t)(current_color >> 16);
    uint8_t g = (uint8_t)(current_color >> 8);
    uint8_t b = (uint8_t)(current_color);

    if (r > 0 && g == 0) {
        r--;
        b++;
    } else if (b > 0 && r == 0) {
        b--;
        g++;
    } else {
        g--;
        r++;
    }

    return pixels.Color(r, g, b);
}

void
loop() {
    set_column(0, ON);
    pixels.show();

    for (uint8_t ii=0; ii<NUM_COLS; ii++) {
        set_column(ii-1, OFF);
        set_column(ii, ON);
        pixels.show();
        _delay_ms(DELAY);
    }

    set_column(NUM_COLS-1, OFF);

    ON = get_next_color(ON);
}
