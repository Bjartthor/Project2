#ifndef ENCODER_H
#define ENCODER_H
#include <avr/io.h>
#include "digital_in.h"
#include "digital_out.h"
#include "timer_msec.h"

class Encoder {
    private:
        Digital_in P1;
        Digital_in P2;
        Digital_out Pout;
        int counter;
        const int rev_res = 700;
        bool P1prevstate;
        bool dir;
        int time
        struct Enc_memory {
            int enc_hist;
            unsigned long timestamp;
        };
        static const int memory_length = 3;
        Enc_memory history[memory_length];
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