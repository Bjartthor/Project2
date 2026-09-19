#include <encoder.h>
#include <arduino.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <pwm_setup.h>

//float sampling_rate = 290e-6; // T_s in seconds (sampiling limit = 280micro s)

int speed_time_ms = 5;
Encoder encoder(2, 4, 3, speed_time_ms); // c1 = PD2 (INT0), c2 = PD4, led = pin 3
int time_counter = 0;
int last_pos = 0;
int now_pos = 0; 
bool has_printed = false;
int main()
{
    Serial.begin(9600);
    encoder.init();
    sei();

    //setSpeedD6(255); 
    setSpeedD5(0);

    while(1){
      setSpeedD5(255);
      //_delay_ms(sampling_rate*1000);
      //encoder.sample(); 
        //Serial.print("Postition: ");
        //Serial.println(encoder.get_position());
        //Serial.print("Speed: ");
      //Serial.println(encoder.get_speed_rpm());

      //_delay_ms(1000);
    }
    return 0;
}

ISR(INT0_vect)
{
  encoder.sample();
}

ISR(TIMER1_COMPA_vect)
{
    // called once per timer period (speed_time_ms ms)
    encoder.update_speed();

}