#include "encoder.h"
#include "timer.h"

Encoder::Encoder(int pin1, int pin2, int pin_out) 
  : P1(pin1), P2(pin2), Pout(pin_out) 
{
    counter = 0;
    dir = true;
}

void Encoder::init() {
    P1.init();
    P2.init();
    Pout.init();
    P1prevstate = P1.is_hi();
}

void Encoder::update() {
    bool P1currstate = P1.is_hi();
    if (P1currstate != P1prevstate) {
        bool P2currstate = P2.is_hi();
        if (P1currstate != P2currstate) {
            counter++;
            dir = true;
        } else {
            counter--;
            dir = false;
        }
        P1prevstate = P1currstate;
        for (int i = history_length - 1; i > 0; i--) {
            enc_memory[i] = enc_memory[i - 1]; 
        }
        enc_memory[0].timestamps = time_ms();
        enc_memory[0].counter_mem = counter;
    }
}

bool Encoder::direction() {
    return dir;
}

int Encoder::position() {
    return counter;
}

int Encoder::speed() {
    long delta_ticks = enc_memory[0].counter_mem - enc_memory[history_length - 1].counter_mem;
    unsigned long delta_time = enc_memory[0].timestamps - enc_memory[history_length - 1].timestamps;
    if (delta_time == 0) {
        return rpm; 
    }
    if (time_ms() - enc_memory[0].timestamps > timeout) {
        rpm = 0;
        return rpm;
    } else {
        rpm = (delta_ticks * 60000.0) / (delta_time * rev_res);
    }
    return rpm;
}