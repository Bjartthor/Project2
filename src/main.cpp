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

Encoder motor(D2,D4,D7); // encoder driver, (encoder in 1, encoder in 2, signal read out)

Drive bridge(0,D8); // motor driver, (timer circuit no., slp pin)

Digital_out timerpin(A1); // pin for tracking main loop frequency on oscilloscope

struct record {
  double speedrecord;
  uint16_t timerecord;
  uint8_t pwmmem;
};

static const uint16_t recordlength = 120;
uint16_t recordhead = 0;
record Record_run[recordlength];
bool stop_flag = false;

ISR(INT0_vect) { 
  motor.update(); // reads position and timestamps on encoder in pin interrupt
}


int main() {
  time_init(); // initalize timer 1 for time tracking use
  serial_init(); // initialize serial comms
  motor.init();  // initalize encoder driver
  bridge.init();  // initalize encoder
  timerpin.init();

  set_loop_ms(5,1000); // sets loop durations in ms, one for controller loop other for serial print
  // uint16_t target = 50; // target rpm
  // double Kp = 16; // constant value for speed controller
  // P_controller P(Kp); // construct controller class
  // double u; // variable for receiving controller output
  uint8_t pwm = 0; // variable to translate countroller output (double) to motor driver input (unsigned 8 bit)
  // char print_str[64];
  // char speed_str[7];
  
  bridge.wake();
  bridge.rev(255);
  sei();
  uint16_t timetrack = 0;

  while (stop_flag == false) {
    if (loop1 == true) {
      loop1 = false;
      // timerpin.toggle(); // for time accuracy validation with oscilloscope
      double currspeed = motor.speed();
      // u = P.update(target,currspeed);
      // if ((u > 254.0) || (u < -254.0)) {
      //   pwm = 254;
      // } else {
      //   if (u < 0) {
      //     pwm = (uint8_t)(-1.0*u);
      //   } else {
      //     pwm = (uint8_t)u;
      //   }
      // }
      // if (u <= 0) {
      //   pwm = 0;
      //   bridge.stop();
      // }  else {
      //   bridge.fwd(pwm);
      // }

      Record_run[recordhead].speedrecord = currspeed;
      Record_run[recordhead].timerecord = timetrack*5;
      Record_run[recordhead].pwmmem = pwm;
      recordhead++;
      timetrack++;
      if (recordhead >= recordlength) {
        stop_flag = true;
      }
    }
    // if (loop2 == true) {
    //   loop2 = false;
    //   dtostrf(motor.speed(), 7, 3, speed_str);
    //   sprintf(print_str,"\rTarget speed: %d rpm    True speed: %s rpm    PWM: %3d        ",
    //     target,speed_str, pwm);
    //   serial_println(print_str);
  }
  bridge.sleep();
  char print_str[64];
  char speed_str[10];

  for (uint16_t i = 0; i < recordlength; i++) {
    dtostrf(Record_run[i].speedrecord, 8, 3, speed_str); // breytir float í string: "XXXXX.XXX"
    sprintf(print_str, "%s,%d,%d\n", speed_str,Record_run[i].timerecord,Record_run[i].pwmmem);
    serial_print(print_str);
  }
  serial_print("----END----\n");
  
  return 0;
}