#include <avr/io.h>
#include "digital_out.h"
#include "encoder.h"
#include "timer.h"
#include "drive.h"
#include <avr/interrupt.h>

Encoder motor(2,3,4);
Drive bridge(5,6,0);

ISR(INT0_vect) {
  motor.update(); // interrupt update
}

int main() {
  motor.init();
  bridge.init();
  // Serial.begin(9600);

  EICRA |= (1 << ISC00);
  EICRA &= ~(1 << ISC01);
  EIMSK |= (1 << INT0);
  sei();

  while (1) {
    // int currpos = motor.position();
    // Serial.println(currpos);
  }
  return 0;
}