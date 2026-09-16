#ifndef TIME_H
#define TIME_H
#include <stdint.h>
void time_init();

uint32_t time_ms();

uint32_t time_mus();

void set_loop_ms(uint8_t loop_period);

extern volatile bool timer_loop;

#endif