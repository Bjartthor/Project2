#include "digital_in.h"
#include <avr/io.h>

Digital_in::Digital_in(int pin) {
    pinMask = (1<< pin);
}

void Digital_in::init() {
    DDRD &= ~pinMask;
    PORTD |= pinMask;
}

bool Digital_in::is_hi() {
    return (PIND & pinMask) != 0;
}

bool Digital_in::is_lo() {
    return (PIND & pinMask) == 0;
}