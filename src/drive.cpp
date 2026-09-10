#include "drive.h"

Drive::Drive(int Pfwd_arg, int Prev_arg) {
    Pfwd = Pfwd_arg;
    Prev = Prev_arg;
}

void Drive::init() {
    gpioSetMode(Pfwd,PI_OUTPUT);
    gpioSetMode(Prev,PI_OUTPUT);
}

void Drive::fwd(int speed) {
    int pwmval = (speed*255)/100;
    gpioWrite(Prev,0);
    gpioPWM(Pfwd,pwmval);
}

void Drive::rev(int speed) {
    int pwmval = (speed*255)/100;
    gpioWrite(Pfwd,0);
    gpioPWM(Prev,pwmval);
}

void Drive::stop(void) {
    gpioPWM(Pfwd,0);
    gpioPWM(Prev,0);
}
