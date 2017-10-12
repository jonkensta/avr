#ifndef _MCP23017_H_
#define _MCP23017_H_

#include <Wire.h>

#define MCP23017_ADDRESS 0x20

#define MCP23017_IODIRA 0x00
#define MCP23017_IPOLA 0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA 0x06
#define MCP23017_INTCONA 0x08
#define MCP23017_IOCONA 0x0A
#define MCP23017_GPPUA 0x0C
#define MCP23017_INTFA 0x0E
#define MCP23017_INTCAPA 0x10
#define MCP23017_GPIOA 0x12
#define MCP23017_OLATA 0x14

#define MCP23017_IODIRB 0x01
#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONB 0x0B
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATB 0x15

#define MCP23017_INT_ERR 255

class MCP23017 {
    public:
        MCP23017(uint8_t address)
                : _address(MCP23017_ADDRESS | (address & 0x07))
        {
            // otherwise empty
        }

        void begin(void);

        void set_pin_mode(uint8_t mode);
        void set_pull_up(bool value);

        void write(uint16_t);

    private:
        uint8_t _address;
};

#endif
