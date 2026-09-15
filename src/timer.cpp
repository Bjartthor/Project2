#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

volatile unsigned long ms = 0;
unsigned long period_ms = 1;

volatile bool timer_loop = false;
uint8_t loop_ms=0;
uint8_t loop_cnt=0;

void time_init() {
    TCCR1A = 0; // Timer 1 control register A (set to 0)
    TCCR1B = 0; // T1 control register B
    TCNT1 = 0; // T1 counter = 0 
    TCCR1B |= (1 << WGM12); // clears timer on compare match
    TIMSK1 |= (1 << OCIE1A); // set interrupt on compare match
    TCCR1B |= (1 << CS11) | (1 << CS10); // 64 prescaler
    OCR1A = (uint16_t)((250000UL * period_ms) / 1000) - 1; // Þetta stillir hversu hratt interruptið keyrir, þarf að vera rétt miðað við prescaler
    sei();
}

unsigned long time_ms() {
    unsigned long current_ms;
    uint8_t oldSREG = SREG; // interrupt state save
    cli(); // disable interrupts
    current_ms = ms;
    SREG = oldSREG; // interrupt state restore
    return current_ms;
}

void set_loop_ms(uint8_t loop_period) {
    uint8_t oldSREG = SREG; // interrupt state save
    cli(); // disable interrupts
    loop_ms = loop_period;
    SREG = oldSREG; // interrupt state restore
}

ISR(TIMER1_COMPA_vect) {
    // Þetta er interrupt á timer 1 og keyrir
    ms++;
    if (loop_ms > 0) {
        loop_cnt++;
        if (loop_cnt >= loop_ms) {
            timer_loop = true;
            loop_cnt = 0;
        }
    }
}





