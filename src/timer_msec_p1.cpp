#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer_msec_p1.h>

void Timer_msec::init()
{
    // this code sets up timer1 for a period_ms interval at 16Mhz clock (mode 4)
    // counting 16000000/1024 cycles of a clock prescaled by 1024
    TCCR1A = 0; // set timer1 to normal operation (all bits in control registers A and B set to zero)
    TCCR1B = 0; //
    TCNT1 = 0; // initialize counter value to 0
    OCR1A = uint16_t(long(period_ms) * (16000000 / 1024) / 1000 - 1); // assign target count to compare register A (must be less than 65536)
    TCCR1B |= (1 << WGM12); // clear the timer on compare match A
    TIMSK1 |= (1 << OCIE1A); // set interrupt on compare match A
    TCCR1B |= (1 << CS12) | (1 << CS10); // set prescaler to 1024 and start the timer
    sei(); // enable interrupts
}

