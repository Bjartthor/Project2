#include <stdint.h>

class Digital_in {
    public:
        Digital_in(int pin) : pinMask(1<<pin) {} //constructor

        void init();

        bool is_hi();

        bool is_lo();

    private:
        uint8_t pinMask;
};