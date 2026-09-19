#include <digital_in.h>
#include <avr/io.h>

void Digital_in::init()
{
    DDRD &= ~pinMask; // Set pin as input
}

bool Digital_in::is_hi()
{
    return (PIND & pinMask) != 0; // Return true if pin is high
}

bool Digital_in::is_lo()
{
    return (PIND & pinMask) == 0; // Return true if pin is low
}