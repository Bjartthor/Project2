#ifndef ENCODER_H
#define ENCODER_H
#include <avr/io.h>
#include <stdint.h>
#include "digital_in.h"
#include "digital_out.h"

class Encoder {
    private:
        Digital_in P1;
        Digital_in P2;
        Digital_out Pout;
        volatile int32_t counter; // bara position nema loopar ekki
        const int16_t rev_res = 700; // Steps í einum hring á mótor
        volatile int16_t ext_counter; // position sem loopar
        volatile bool P1prevstate;
        volatile bool dir; // true fyrir áfram false fyrir afturábak. Fer samt eftir hvernig maður tengir hvort áttin er hvað
        struct History {
            // Þetta er til að reikna hraðann, hægt að averagea yfir seinustu X mörg counter steps
            volatile int32_t counter_mem;
            volatile unsigned long timestamps;
        };
        static const int history_length = 40; // Averagear hraðann yfir þetta mörg steps
        History enc_memory[history_length];
        volatile int history_head = 0;
        const unsigned long timeout = 100;
        float rpm; // Reiknuð rpm
    public:
        Encoder(int pin1, int pin2, int pin_out); 
        void init();
        void update(); // .update() bara til að lesa af encoder, reiknar hraðann í leiðinni
        int16_t position(); // .position() skilar counter, ekki búinn að setja inn þannig að hann resettar eftir hring
        bool direction(); // .direction() skilar "bool dir" sem er true/false
        float speed(); // skilar rpm sem float
};

#endif