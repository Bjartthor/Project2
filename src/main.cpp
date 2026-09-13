#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"
#include "timer.h"
#include "digital_in.h"
#include "digital_out.h"
#include "encoder.h"
#include "drive.h"

char pos_str[16];
const float time_constant=0.005;

Encoder motor(D2,D4,D7);
Drive bridge(0,D8);
Digital_in pinA2(A2);
Digital_out pinA1(A1);

ISR(INT0_vect) {
  motor.update(); // interrupt update
}

int main() {
  time_init();
  serial_init();
  motor.init();
  bridge.init();
  pinA2.init();
  pinA1.init();
  set_loop_ms(100); // Interrrupt setur timer_loop = true á nkvml 100ms fresti

  

  EICRA |= (1 << ISC00);
  EICRA &= ~(1 << ISC01);
  EIMSK |= (1 << INT0);
  sei();

  pinA1.set_hi();

  while (1) {
    if (timer_loop) {
      timer_loop = false;
      sprintf(pos_str, "%d", motor.position());
      serial_println(pos_str);
      if (pinA2.is_hi() == true) {
        bridge.fwd(100);
      } else {
        bridge.rev(100);
      }
    }
  }
    
  return 0;
  }