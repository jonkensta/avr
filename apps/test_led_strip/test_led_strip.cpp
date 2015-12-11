// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <neopixel.h>
#include <avr/power.h>
#include <util/delay.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            4

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      300

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Neopixel pixels = Neopixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int delayval = 100; // delay for half a second

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void reset_leds(void) {
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
}

void toggle_leds(uint8_t r, uint8_t g, uint8_t b) {
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(r, g, b)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    _delay_ms(delayval); // Delay for a period of time (in milliseconds).
  }
}

void loop() {
  reset_leds();
  toggle_leds(150, 0, 0);

  reset_leds();
  toggle_leds(0, 150, 0);

  reset_leds();
  toggle_leds(0, 0, 150);
}
