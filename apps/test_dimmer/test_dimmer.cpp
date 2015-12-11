#include <Arduino.h>
#include <TimerOne.h>

#include "DutyCycler.h"

const uint8_t NUM_DUTY_CYCLERS = 2;
DutyCycler duty_cyclers[] = {DutyCycler(5), DutyCycler(6)};

void update_duty_cyclers(void) {
    for (int index=0; index < NUM_DUTY_CYCLERS; index++)
        duty_cyclers[index].update();
}

void reset_duty_cyclers(void) {
    for (int index=0; index < NUM_DUTY_CYCLERS; index++)
        duty_cyclers[index].reset();
}

void setup() {
    for (int index=0; index < NUM_DUTY_CYCLERS; index++)
        duty_cyclers[index].initialize();

    const uint32_t period = 32; // (10 ** 6) / (120 * 256)
    Timer1.initialize(period);
    Timer1.attachInterrupt(update_duty_cyclers);

    const uint8_t interrupt = 0;
    attachInterrupt(interrupt, reset_duty_cyclers, RISING);

    Serial.begin(9600);
}

const uint8_t DUTY_CYCLE_LOOKUP[] = {
#include "lookup_table.inc"
};

void set_dimness(uint8_t index, uint8_t percentage) {
    percentage = min(percentage, 100);
    uint8_t duty_cycle = DUTY_CYCLE_LOOKUP[percentage];
    index = min(index, 1);
    duty_cyclers[index].set_duty_cycle(duty_cycle);
}

int serial_read_int(void) {
    const int BUFFER_SIZE = 5;
    char buffer[BUFFER_SIZE];

    int num_chars = 0;

    while (true) {
        while (!Serial.available())
            ;
        int in_char = Serial.read();

        if (in_char == '\r')
            break;

        if (!isDigit(in_char))
            num_chars = BUFFER_SIZE;

        if (num_chars < BUFFER_SIZE)
            buffer[num_chars++] = in_char;
    }

    if (num_chars >= BUFFER_SIZE)
        return -1;

    buffer[num_chars] = '\0';
    return atoi(buffer);
}

void loop() {
    for (int index=0; index < NUM_DUTY_CYCLERS; index++)
    {
        int percentage = serial_read_int();
        while (percentage < 0)
            percentage = serial_read_int();
        set_dimness(index, percentage);
    }
}
