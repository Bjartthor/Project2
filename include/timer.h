#ifndef TIME_H
#define TIME_H
#include <stdint.h>
void time_init();

unsigned long time_ms();

void set_loop_ms(uint8_t loop_period);

extern volatile bool timer_loop;

#endif