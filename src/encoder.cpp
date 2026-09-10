#include <encoder.h>
#include <avr/io.h>

void Encoder::init()
{
    c1.init();
    c2.init();
    led.init();
    timer.init();

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

int Encoder::get_position()
{
    return _position;
}

void Encoder::update_speed()
{
    speed_rpm = float(_position - last_position)/1050/(float(speed_period)/1000/60);
    last_position = _position;
}

int Encoder::get_speed_rpm()
{
    return speed_rpm;
}
