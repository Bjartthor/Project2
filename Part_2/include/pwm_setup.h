#ifndef MOTOR_PWM_H
#define MOTOR_PWM_H

#include <stdint.h> 

// Function declarations
void setupPWM_D5_D6();
void setSpeedD6(uint8_t dutyCycle);
void setSpeedD5(uint8_t dutyCycle);

#endif