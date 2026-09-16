#include "encoder.h"
#include "timer.h"
#include <avr/interrupt.h>
#include <stdint.h>

Encoder::Encoder(int pin1, int pin2, int pin_out) 
  : P1(pin1), P2(pin2), Pout(pin_out) 
{
    counter = 0;
    ext_counter = 0;
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
            ext_counter++;
            dir = true;
        } else {
            counter--;
            ext_counter--;
            dir = false;
        }
        P1prevstate = P1currstate;

        history_head++;
        if (history_head >= history_length) {
            history_head=0;
        }
        enc_memory[history_head].timestamps = time_ms();
        enc_memory[history_head].counter_mem = counter;

    if (ext_counter >= rev_res || -rev_res >= ext_counter) {
        ext_counter = 0;
    }
    }
}

bool Encoder::direction() {
    return dir;
}

int16_t Encoder::position() {
    cli();
        int16_t val = ext_counter;
    sei();
    return val;
}

float Encoder::speed() {
    cli(); //stoppa interrupts til að copya >8 bit vals
        int head = history_head;
        int tail = head + 1;
        if (tail >= history_length) {
            tail = 0;
        }
        int32_t new_pos = enc_memory[head].counter_mem;
        int32_t old_pos = enc_memory[tail].counter_mem;
        unsigned long new_time = enc_memory[head].timestamps;
        unsigned long old_time = enc_memory[tail].timestamps;
    sei();

    int32_t delta_pos = new_pos - old_pos;
    unsigned long delta_time = new_time - old_time;
    if (delta_time == 0) {
        return rpm; 
    }
    if (time_ms() - new_time > timeout) {
        rpm = 0;
        return rpm;
    } else {
        rpm = (delta_pos * 60000.0) / (delta_time * rev_res);
    }
    return rpm;
}