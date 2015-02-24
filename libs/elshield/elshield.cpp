#include <Arduino.h>

#include "elshield.h"

static uint8_t EL_CH_STATE[] = {LOW, LOW, LOW};

void
el_init(void)
{
    pinMode(4, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
}

void
el_set_all_off(void)
{
    digitalWrite(4, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
}

static inline uint8_t
el_ch_to_pin(uint8_t ch)
{
    return (ch == 0) ? 4 : ch + 5;
}

void
el_set_ch(uint8_t ch)
{
    if (ch >= EL_NUM_CH) return;
    uint8_t pin = el_ch_to_pin(ch);
    digitalWrite(pin, HIGH);
    EL_CH_STATE[ch] = HIGH;
}

void
el_clear_ch(uint8_t ch)
{
    if (ch >= EL_NUM_CH) return;
    uint8_t pin = el_ch_to_pin(ch);
    digitalWrite(pin, LOW);
    EL_CH_STATE[ch] = LOW;
}

void
el_toggle_ch(uint8_t ch)
{
    if (ch >= EL_NUM_CH) return;
    uint8_t pin = el_ch_to_pin(ch);
    if (EL_CH_STATE[ch] == HIGH) {
        digitalWrite(pin, LOW);
        EL_CH_STATE[ch] = LOW;
    }
    else { // must be in LOW state
        digitalWrite(pin, HIGH);
        EL_CH_STATE[ch] = HIGH;
    }
}
