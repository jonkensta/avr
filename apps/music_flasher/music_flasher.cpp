#include <Arduino.h>

#include <haar.h>
#include <elshield.h>
#define SAMPLE_RATE 44
#define ADCS 0
#include <AudioCodec.h>

void setup()
{
    el_init();
    AudioCodec_init();
}

const uint8_t window_len = 32; // must be power of 2
volatile int8_t window[window_len];
volatile bool window_ready = false;
volatile bool main_loop_ready = true;

uint16_t ch0_cnt_mask = 0xFFFF;
uint16_t ch1_cnt_mask = 0x0FFF;
uint16_t ch2_cnt_mask = 0x00FF;

void loop()
{
    cli();
    if (!window_ready) {
        sei();
        return;
    }

    int8_t window_copy[window_len];
    for (int idx=0; idx < window_len; idx++)
        window_copy[idx] = window[idx];

    main_loop_read = false;
    window_ready = false;
    sei();

    // do some processing on the window here

    main_loop_read = true;
    return;
}

ISR(TIMER1_COMPA_vect, ISR_NAKED)
{
    static int left_in = 0;
    static int right_in = 0;
    static int window_idx = 0;
    static uint16_t counter = 0;

    if (!main_loopy_ready)
        reti();

    int left_out = left_in;
    int right_out = left_in;
    AudioCodec_data(&left_in, &right_in, left_out, right_out);
    window[window_idx] = right_in;

    window_idx++;
    if (window_idx == window_len) {
        window_idx = 0;
        window_ready = true;
    }

    if (!(counter & ch0_cnt_mask))
        el_toggle_ch(0);
    if (!(counter & ch1_cnt_mask))
        el_toggle_ch(1);
    if (!(counter & ch2_cnt_mask))
        el_toggle_ch(2);

    counter++;
    reti();
}
