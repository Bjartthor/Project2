#include <pwm_setup.h>
#include <avr/io.h>

// Set up Timer 0 for Fast PWM on pins D5 and D6
void setupPWM_D5_D6() {
    // 1. Set pins D5 (PD5) and D6 (PD6) as outputs
    DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD7);

    // turn off sleep
    PORTD |= (1 << PORTD7);
    
    // 2. Set Timer 0 to Fast PWM Mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    
    // 3. Set non-inverting mode for both channels
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
    
    // 4. Set a prescaler of 64 and start the timer
    TCCR0B |= (1 << CS01) | (1 << CS00);
}

// Control D6 speed (0 to 255)
void setSpeedD6(uint8_t dutyCycle) {
    OCR0A = dutyCycle; 
}

// Control D5 speed (0 to 255)
void setSpeedD5(uint8_t dutyCycle) {
    OCR0B = dutyCycle; 
}