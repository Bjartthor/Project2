#ifndef DIGITAL_OUT_H
#define DIGITAL_OUT_H

#include <stdint.h>
#include <avr/io.h>
class Digital_out {
    private:
        uint8_t pinMask;
    public:
        Digital_out(int pin) {
            pinMask = (1 << pin); // breytir tölu í 8 bit address: 5 -> 00010000
        }

        void init() {
            DDRB |= pinMask;
        }

        void set_hi() {
            PORTB |= pinMask;
        }

        void set_lo() {
            PORTB &= ~pinMask;
        }

        void toggle() {
            PORTB ^= pinMask;
        }
};

#endif