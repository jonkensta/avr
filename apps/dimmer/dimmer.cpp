#include <Arduino.h>
#include <MCP23017.h>

MCP23017 mcp(0);

void
setup(void)
{
    mcp.begin();
    mcp.set_pin_mode(OUTPUT);
}

void
loop(void)
{
    mcp.write(0x0000);
    delay(1000);

    mcp.write(0xFFFF);
    delay(1000);
}
