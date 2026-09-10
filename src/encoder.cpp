#include <encoder.h>
#include <avr/io.h>

void Encoder::init()
{
    c1.init();
    c2.init();
    led.init();

    EICRA |= (1 << ISC00);  // INT0: trigger on any logical change
    EICRA &= ~(1 << ISC01); 
    EIMSK |= (1 << INT0);   // enable INT0
}

void Encoder::sample()
{
    if (c1_was_lo && c1.is_hi()) {
        c1_was_lo = false;
        if (c2.is_hi()) {
            _position++; //clockwise
        }
        else {
            _position--; //counter clockwise
        }
        led.set_hi();
    }
    else if (c1.is_lo()) {
        c1_was_lo = true;
        led.set_lo();
    }
}

int Encoder::position()
{
    return _position;
}
