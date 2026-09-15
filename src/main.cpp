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
Digital_out LED(D13);
Digital_out pinA1(A1);
Digital_in pinA2(A2);
Digital_in pinD3(D3);
volatile bool on_off_toggle = false;
volatile unsigned long antibounce = 0;

ISR(INT0_vect) {
  motor.update(); // interrupt update
  if (timer_loop) {
    timer_loop = false;
    sprintf(pos_str, "%d", motor.position());
    serial_println(pos_str);
  }
}
ISR(INT1_vect) {
  if ((time_ms() - antibounce) > 250UL) {
    LED.toggle();
    on_off_toggle = !on_off_toggle;
    antibounce = time_ms();
    if (on_off_toggle == true) {
      bridge.wake();
    } else {
      bridge.stop();
      bridge.sleep();
    }
  }
}

int main() {
  time_init();
  serial_init();
  motor.init();
  bridge.init();
  LED.init();
  pinA1.init();
  pinA2.init();
  pinD3.init();
  set_loop_ms(100); // Interrrupt setur timer_loop = true á nkvml 100ms fresti
  
  
  // INT0 (D2)
  EICRA |= (1 << ISC00);
  EICRA &= ~(1 << ISC01);
  EIMSK |= (1 << INT0);
  // INT1 (D3)
  EICRA |= (1 << ISC11) | (1 << ISC10); 
  EIMSK |= (1 << INT1);

  sei();

  pinA1.set_hi();
  bridge.wake();

  while (1) {
    if (on_off_toggle == true) {
      if (pinA2.is_hi() == true) {
        bridge.fwd(100);
      } else {
        bridge.rev(100);
      }
    }
  }
    
  return 0;
}