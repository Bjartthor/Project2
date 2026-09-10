#include <digital_in.h>
#include <digital_out.h>
#include <timer_msec_p1.h>

class Encoder {
    public:
        Encoder(int pin_c1, int pin_c2, int pin_led, int speed_resolution)
            : c1(pin_c1), c2(pin_c2), led(pin_led), timer(speed_resolution), speed_period(speed_resolution) {} //constructor

        void init();

        void sample();

        int get_position();

        void update_speed();

        int get_speed_rpm();

    private:
        Digital_in c1;
        Digital_in c2;
        Digital_out led;
        Timer_msec timer;

        volatile int _position = 0;
        volatile int last_position = 0;
        int speed_period;
        volatile int speed_rpm = 0;
        bool c1_was_lo = true;
};
