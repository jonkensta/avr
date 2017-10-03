#ifndef MCP4911_H
#define MCP4911_H 1

#include "pins_arduino.h"
#include "SPI.h"

#define BUF  14  // Input buffer control bit
#define GA   13  // Output gain selection bit
#define SHDN 12  // Output shutdown control bit

class MCP4911 {
  public:
    // Constructor
    MCP4911(const uint8_t rate, const uint8_t ss_pin)
        : _rate(rate), _ss_pin(ss_pin)
    {
        // empty constructor
    }

    ~MCP4911()
    {
        end();
    }

    void begin();
    void end();
    void write(uint16_t value);

  private:
    const uint8_t _rate;
    const uint8_t _ss_pin;
};

#endif
