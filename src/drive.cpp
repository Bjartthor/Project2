#include "drive.h"
#include "digital_out.h"
#include <avr/io.h>

Drive::Drive(int TN_arg, int Pslp_arg) : Pslp(Pslp_arg) {
    TN = TN_arg;
    if (TN == 0) { //timer 0 (D5 & D6)
        Pfwd = &OCR0B; // Pin D5
        Prev = &OCR0A; // Pin D6
    } else if (TN == 1) { //timer 1 (d9 & d10)
        Pfwd = &OCR1AL; //d9
        Prev = &OCR1BL; //d10, -L f. 8 bit er annars 16 bit (pwm er þá 0-65535)
    } else if (TN == 2) { //timer 1 (d9 & d10)
        Pfwd = &OCR2B; //d3
        Prev = &OCR2A; //d11
    }
}

void Drive::init() {
    Pslp.init();
    if (TN == 0) {
        DDRD |= (1 << 5) | (1 << 6);
        TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
        TCCR0B = (1 << CS01) | (1 << CS00);
    } else if (TN == 1) {
        DDRB |= (1 << 1) | (1 << 2); //9&11 eru á B, 1 er D9 & 2 er D10
        TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
        TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
    } else if (TN == 2) {
        DDRD |= (1 << 3); //D3
        DDRB |= (1 << 3); //D11 er 3 á B
        TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
        TCCR2B = (1 << CS22);
    }
    *Pfwd = 0;
    *Prev = 0;
}

void Drive::fwd(int speed) {
    *Prev = 0;
    int pwmval = (speed*255)/100;
    *Pfwd = pwmval;
}

void Drive::rev(int speed) {
    *Pfwd = 0;
    int pwmval = (speed*255)/100;
    *Prev = pwmval;
}

void Drive::stop(void) {
    *Pfwd = 0;
    *Prev = 0;
}

void Drive::sleep() {
    *Pfwd = 0;
    *Prev = 0;
    Pslp.set_lo();
}

void Drive::wake() {
    *Pfwd = 0;
    *Prev = 0;
    Pslp.set_hi();   
}