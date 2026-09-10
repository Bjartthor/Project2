#include "encoder.h"
#include "time.h"

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
        for (int i = memory_length - 1; i > 0; i--) {
            history[i] = history[i - 1]; 
        }
        history.timestamp[0] = time_ms();
        history.enc_hist[0] = counter;
    }
}

bool Encoder::direction() {
    return dir;
}

int Encoder::position() {
    return counter;
}

int Encoder::speed() {
    long delta_ticks = history[0].enc_hist - history[memory_length - 1].enc_hist;
    unsigned long delta_time = history[0].timestamp - history[memory_length - 1].timestamp;
    if (delta_time == 0) {
        return rpm; 
    }
    if (time_ms() - history[0].timestamp > timeout) {
        rpm = 0;
        return rpm;
    } else {
        rpm = (delta_ticks * 60000.0) / (delta_time * rev_res);
    }
    return rpm;
}