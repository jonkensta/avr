#ifndef ELSHIELD_H
#define ELSHIELD_H 1

#include <Arduino.h>

static inline void
el_init(void)
{
    pinMode(4, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
}

static inline void
el_set_all_off(void)
{
    digitalWrite(4, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
}

static inline void
el_set_ch(uint8_t ch)
{
    if (ch > 2) return;
    uint8_t pin = (ch == 0) ? 4 : ch + 5;
    digitalWrite(pin, HIGH);
}

static inline void
el_clear_ch(uint8_t ch)
{
    if (ch > 2) return;
    uint8_t pin = (ch == 0) ? 4 : ch + 5;
    digitalWrite(pin, LOW);
}

#endif
