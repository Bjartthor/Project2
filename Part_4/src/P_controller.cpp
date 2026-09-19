#include "P_controller.h"

P_controller::P_controller(double Kp_arg) {
    Kp = Kp_arg;
}

double P_controller::update(double ref,double actual) {
    return Kp*(ref-actual);
}