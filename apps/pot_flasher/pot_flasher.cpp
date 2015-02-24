#include <Arduino.h>

#define SAMPLE_RATE 44
#define ADCS 1
#include <AudioCodec.h>
#include <elshield.h>

void setup()
{
    el_init();
    AudioCodec_init();
}

void loop()
{
    while (true); // reduces clock jitter
}

// timer1 interrupt routine - all data processed here
volatile uint16_t adc_counter = 0;
const uint16_t adc_count_mask = 0x003F;

volatile uint8_t el_channel = 0;
volatile uint16_t el_counter = 0;
volatile uint16_t el_count_threshold = 0xFFFF;

ISR(TIMER1_COMPA_vect, ISR_NAKED)
{
    uint16_t adc_val;
    AudioCodec_ADC(&adc_val);
    if (!(adc_counter & adc_count_mask))
        el_count_threshold = adc_val;
    adc_counter++;

    if (el_counter < el_count_threshold) {
        el_counter++;
    }
    else {
        el_set_all_off();
        el_set_ch(el_channel);
        el_channel++;
        if (el_channel > 2)
            el_channel = 0;
        el_counter = 0;
    }

    reti();
}
