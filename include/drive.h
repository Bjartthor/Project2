#ifndef DRIVE_H
#define DRIVE_H

#include "digital_out.h"
#include <stdint.h>

class Drive {
    private:
        volatile uint8_t *Pfwd;
        volatile uint8_t *Prev;
        volatile uint8_t *CTRLreg;
        int TN;
        Digital_out Pslp;
        int speed;
    public:
        Drive(int TN_arg, int Pslp_arg);
        void init(void);
        void fwd(int speed);
        void rev(int speed);
        void stop(void);
        void sleep(void);
        void wake(void);
};

#endif