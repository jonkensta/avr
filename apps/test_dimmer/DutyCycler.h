#ifndef DUTY_CYCLER_H
#define DUTY_CYCLER_H

#include <Arduino.h>

class DutyCycler
{
private:
    uint8_t _pin;
    volatile uint8_t _count;
    uint8_t _duty_cycle;

public:
    DutyCycler(uint8_t pin) : _pin(pin), _count(0), _duty_cycle(0) { }

    inline void initialize(void) {
        pinMode(_pin, OUTPUT);
    }

    inline void update(void) {
        digitalWrite(_pin, (_count < _duty_cycle) ? LOW : HIGH);
        _count++;
    }

    inline void set_duty_cycle(uint8_t value) { _duty_cycle = value; }
    inline bool ready(void) { _count >= (_duty_cycle >> 1); }
    inline void reset(void) { _count = 0; }
};

#endif
