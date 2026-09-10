#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned long ms = 0;
unsigned long period_ms = 1;

void time_init()
{
    TCCR1A = 0; // set timer1 to normal operation (all bits in control registers A and B set to zero)
    TCCR1B = 0; //
    TCNT1 = 0; // initialize counter value to 0
    OCR1A = (uint16_t)((15625UL * period_ms) / 1000) - 1; // assign target count to compare register A (must be less than 65536)
    TCCR1B |= (1 << WGM12); // clear the timer on compare match A
    TIMSK1 |= (1 << OCIE1A); // set interrupt on compare match A
    TCCR1B |= (1 << CS12) | (1 << CS10); // set prescaler to 1024 and start the timer
    sei(); // enable interrupts
}

ISR(TIMER1_COMPA_vect) {
    ms++;
}

unsigned long time_ms() {
    unsigned long current_ms;
    
    uint8_t oldSREG = SREG; 
    cli(); // disable interrupts
    
    current_ms = ms; // read safely
    
    SREG = oldSREG; // restore interrupts
    
    return current_ms;
}

