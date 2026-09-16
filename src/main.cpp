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

// ónotað, líka ekki réttur time constant
const float time_constant=0.005;

// Digital_out & Digital_in taka núna á móti merkingunum á pinnunum. S.S. það sem stendur á arduinoinu sjálfu
// Setur á rétt port sjálft. Annars eru klassarnir eins nema engin .cpp files, allt í .h
Digital_out LED(D13);
Digital_out pinA1(A1);
Digital_in pinA2(A2);
Digital_in pinD3(D3);

// Encoder & Drive taka líka á móti merkingunum á pinnum. #define línur þýða "D4" (t.d.) í rétt númer

// Encoder tekur input í röð: 
// (encoder output 1, encoder output 2, þessi er output pinni en gerir ekki neitt eins og er)
Encoder motor(D2,D4,D7);

// Þessi tekur: (timer númer, sleep pinni til að kveikja og slökkva)
// það eru til 3 timerar, timer 0, timer 1 og timer 2. Timer 1 er aðeins öðruvísi og er notaður í timer.cpp s.s. ekki nota
// Stendur í drive.cpp hvaða pinnar eru fwd or reverse fyrir hvaða timera
Drive bridge(0,D8);

// Drasl fyrir takkann/D3 interrupt
volatile bool on_off_toggle = false;
volatile unsigned long antibounce = 0;

int16_t curr_pos = 0;

ISR(INT0_vect) { 
// Interrupt á pinna D2
  motor.update();
  curr_pos = motor.position();
}

ISR(INT1_vect) { 
// Interrupt á pinna D3, kveikir og slekkur á driver/brú ef D3 fær spennu
// D3 þarf pulldown resistor í jörð. LED á arduino (L - "pinni D13") sýnir stöðu 
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
  time_init(); // timer driver notar ekki klassa. Þarf bara keyra þessa línu einu sinni
  serial_init(); // fyrir serial print
  motor.init(); 

  // Þetta initializar sleep pinnan sem Digital_out og gerir .set_lo
  // þarf að gera .wake() til að kveikva á driver/bru
  bridge.init(); 

  LED.init();

  // nota þessa með rofa til að flippa átt á mótor. A2 er input A1 er output alltaf .set_hi, nota A1 líka fyrir D3 interrupt
  pinA1.init();
  pinA2.init();

  // Þetta þarf fyrir D3 interrupt
  pinD3.init();

  //Stilla interrupts, INT0 er f. encoder/D2 & INT1 er fyrir takka/D3
  // INT0 (D2)
  EICRA |= (1 << ISC00);
  EICRA &= ~(1 << ISC01);
  EIMSK |= (1 << INT0);
  // INT1 (D3)
  EICRA |= (1 << ISC11) | (1 << ISC10); 
  EIMSK |= (1 << INT1);
  
  sei();

  pinA1.set_hi();

  set_loop_ms(100); // Interrrupt setur timer_loop = true á nkvml 100ms fresti, má vera hvað sem er
  
  // Þetta er bara til að prenta í serial
  char print_str[64];
  char speed_str[10];

  while (1) {
    // Checkar hvort A2 sé hi eða lo til að breyta snúningsátt
    if (on_off_toggle == true) {
      if (pinA2.is_hi() == true) {
        bridge.fwd(100);
      } else {
        bridge.rev(100);
      }
    }
    if (timer_loop) {
      timer_loop = false; // timer_loop
      dtostrf(motor.speed(), 8, 3, speed_str);
      sprintf(print_str, "\rPosition: %4d   Speed: %s rpm   Direction: %s", 
        curr_pos, speed_str, 
        motor.direction() ? "forward" : "reverse");
      serial_print(print_str);
    }
  }
    
  return 0;
}