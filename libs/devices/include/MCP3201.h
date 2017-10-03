#ifndef MCP3201_H
#define MCP3201_H 1

#include "pins_arduino.h"
#include "SPI.h"

class MCP3201 {
  public:
    // Constructor
    MCP3201(const uint8_t rate, const uint8_t ss_pin) : _rate(rate), _ss_pin(ss_pin) { }

    void begin();
    void end();
    int16_t read(void);

  private:
    const uint8_t _rate;
    const uint8_t _ss_pin;

    uint16_t read_spi(void);
};

#endif
