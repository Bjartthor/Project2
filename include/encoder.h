#ifndef ENCODER_H
#define ENCODER_H
#include <avr/io.h>
#include "digital_in.h"
#include "digital_out.h"
#include "timer.h"

class Encoder {
    private:
        Digital_in P1;
        Digital_in P2;
        Digital_out Pout;
        int counter;
        const int rev_res = 700;
        bool P1prevstate;
        bool dir;
        struct History {
            int counter_mem;
            unsigned long timestamps;
        };
        static const int history_length = 3;
        History enc_memory[history_length];
        unsigned long timeout;
        float rpm;
    public:
        Encoder(int pin1, int pin2, int pin_out); 
        void init();
        void update();
        int position();
        bool direction();
        int speed();
};

#endif