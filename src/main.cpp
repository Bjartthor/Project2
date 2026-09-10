#include <encoder.h>
#include <arduino.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//float sampling_rate = 290e-6; // T_s in seconds (sampiling limit = 280micro s)


Encoder encoder(2, 4, 3, 100); // c1 = PD2 (INT0), c2 = PD4, led = pin 3

int main()
{

    Serial.begin(9600);

    encoder.init();
    sei();

    while(1){
      //_delay_ms(sampling_rate*1000);
      //encoder.sample(); 
        Serial.print("Postition: ");
        Serial.println(encoder.get_position());
        Serial.print("Speed: ");
        Serial.println(encoder.get_speed_rpm());
      _delay_ms(1000);
    }
    return 0;
}

ISR(INT0_vect)
{
  encoder.sample();
}

ISR(TIMER1_COMPA_vect)
{
    // called once per timer period (100 ms); Do something
    encoder.update_speed();
}