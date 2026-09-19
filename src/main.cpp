#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "timer.h"
#include "digital_in.h"
#include "digital_out.h"
#include "encoder.h"
#include "drive.h"
#include "P_controller.h"

Encoder motor(D2,D4,D7);

Drive bridge(0,D8);

double Kp = 2;
P_controller P(Kp);

ISR(INT0_vect) { 
  motor.update();
}

int main() {
  time_init();
  serial_init();
  motor.init(); 
  bridge.init();
  set_loop_ms(6,500);

  uint16_t target = 75; // target rpm
  double u;
  uint8_t pwm;
  char print_str[64];
  char speed_str[10];
  
  bridge.wake();

  // INT0 (D2)
  EICRA |= (1 << ISC00);
  EICRA &= ~(1 << ISC01);
  EIMSK |= (1 << INT0);
  sei();

  bridge.rev(5);

  while (1) {

    if (loop1 == true) {
      loop1 = false;
      u = P.update(target,motor.speed());
      if ((u > 100.0) || (u < -100.0)) {
        pwm = 100;
      } else {
        if (u < 0) {
          pwm = (uint8_t)(-1.0*u);
        } else {
          pwm = (uint8_t)u;
        }
        
      }
      if (u <= 0) {
        bridge.stop();
      }  else {
        bridge.fwd(pwm);
      }
    }
    if (loop2 == true) {
      loop2 = false;
      dtostrf(motor.speed(), 8, 3, speed_str);
      sprintf(print_str,"Target speed: %d    True speed: %s\n",target,speed_str);
      serial_print(print_str);
    }
  }
}