#include <Arduino.h>
#include <elshield.h>

void setup()
{
    el_init();
}

int count = 0; 

void loop()
{ 
    el_set_ch(count%3);
    delay(200);
    if(count++ == 999)
        count = 0;
}
