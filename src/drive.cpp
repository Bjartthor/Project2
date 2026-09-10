#include "drive.h"
#include "digital_out.h"

Drive::Drive(int Pfwd_arg, int Prev_arg, int Pslp_arg) : Pfwd(Pfwd_arg), Prev(Prev_arg), Pslp(Pslp_arg) {
}

void Drive::init() {
    Pfwd.init();
    Pfwd.set_lo();
    Prev.init();
    Prev.set_lo();
    Pslp.init();
    Pslp.set_hi();
}

void Drive::fwd(int speed) {
    int pwmval = (speed*255)/100;
    // for later
}

void Drive::rev(int speed) {
    int pwmval = (speed*255)/100;
    // for later
}

void Drive::stop(void) {
    Pfwd.set_lo();
    Prev.set_lo();
}

void Drive::sleep() {
    Pfwd.set_lo();
    Prev.set_lo();
    Pslp.set_lo();
}

void Drive::wake() {
    Pfwd.set_lo();
    Prev.set_lo();
    Pslp.set_hi();   
}