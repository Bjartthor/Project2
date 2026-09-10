#include <digital_in.h>
#include <digital_out.h>

class Encoder {
    public:
        Encoder(int pin_c1, int pin_c2, int pin_led)
            : c1(pin_c1), c2(pin_c2), led(pin_led) {} //constructor

        void init();

        void sample();

        int position();

    private:
        Digital_in c1;
        Digital_in c2;
        Digital_out led;
        volatile int _position = 0;
        bool c1_was_lo = true;
};
