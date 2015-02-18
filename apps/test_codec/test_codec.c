#include <Arduino.h>

#define SAMPLE_RATE 44 // 44.1Khz
#define ADCS 0 // no ADCs are being used
#include <AudioCodec.h>

void setup()
{
  AudioCodec_init(); // setup codec registers
}

void loop()
{
  while (true);
}


// timer1 interrupt routine - all data processed here
volatile int left = 0, right = 0;
ISR(TIMER1_COMPA_vect, ISR_NAKED)
{
    int left_in, right_in;
    AudioCodec_data(&left_in, &right_in, left, right);
    left = left_in; right = right_in;
    reti();
}
