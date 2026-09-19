#ifndef P_CONTROLLER_H
#define P_CONTROLLER_H

class P_controller {
    private:
        double Kp;
    public:
        P_controller(double Kp_arg);
        double update(double ref,double actual);
};

#endif