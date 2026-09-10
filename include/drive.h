#ifndef DRIVE_H
#define DRIVE_H
#include "digital_out.h"

class Drive {
    private:
        Digital_out Pfwd;
        Digital_out Prev;
        Digital_out Pslp;
        int speed;
    public:
        Drive(int Pfwd_arg,int Prev_arg, int Pslp_arg);
        void init(void);
        void fwd(int speed);
        void rev(int speed);
        void stop(void);
        void sleep(void);
        void wake(void);
};

#endif