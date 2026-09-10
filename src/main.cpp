#include <encoder.h>
#include <arduino.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//float sampling_rate = 290e-6; // T_s in seconds (sampiling limit = 280micro s)


Encoder encoder(2, 4, 3); // c1 = PD2 (INT0), c2 = PD4, led = pin 3


int main()
{

    Serial.begin(9600);

    encoder.init();
    sei();

    int last_position = 0;

    while(1){
      //_delay_ms(sampling_rate*1000);
      //encoder.sample();      
      int position = encoder.position();
      if (position != last_position) {
        last_position = position;
        Serial.println(position);
      }
    }
    return 0;
}

ISR(INT0_vect)
{
  encoder.sample();
}