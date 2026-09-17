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

Digital_out LED(D13);
Digital_in pinD3(D3); // interrupt

Encoder motor(D2,D4,D7);

Drive bridge(0,D8);

struct record {
  int16_t posrecord;
  float speedrecord;
  uint32_t timerecord;
};

ISR(INT0_vect) { 
  motor.update();
}

ISR(INT1_vect) { 

}

int main() {
  time_init();
  serial_init();
  motor.init(); 
  bridge.init(); 

  LED.init();
  LED.set_lo();
  pinD3.init();

  static const uint16_t recordlength = 120;
  uint16_t recordhead = 0;
  record Record_run[recordlength];
  bool stop_flag = false;

  // INT0 (D2)
  EICRA |= (1 << ISC00);
  EICRA &= ~(1 << ISC01);
  EIMSK |= (1 << INT0);
  // INT1 (D3)
  EICRA |= (1 << ISC11) | (1 << ISC10); 
  EIMSK |= (1 << INT1);
  
  sei();
  serial_print("\n\n\n----START----\n");
  set_loop_ms(10); // Interrrupt setur timer_loop = true á nkvml 100ms fresti, má vera hvað sem er
  bridge.wake();
  bridge.fwd(100);

  while(stop_flag == false) {

    if (timer_loop == true) {
      timer_loop = false;
      Record_run[recordhead].posrecord = motor.position();
      Record_run[recordhead].speedrecord = motor.speed();
      Record_run[recordhead].timerecord = time_mus();
      recordhead++;
    }
    if (recordhead >= recordlength) {
      stop_flag = true;
    }
  }

  bridge.sleep();

  char print_str[64];
  char speed_str[10];

  for (uint16_t i = 0; i < recordlength; i++) {
    dtostrf(Record_run[i].speedrecord, 8, 3, speed_str); // breytir float í string: "XXXXX.XXX"
    sprintf(print_str, "%4d,%s,%lu\n", 
    Record_run[i].posrecord, 
    speed_str, 
    Record_run[i].timerecord);
    serial_print(print_str);
  }
  serial_print("----END----\n");
  LED.set_hi();
  while (stop_flag == true);
  return 0;
}