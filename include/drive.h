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
        void stop(void); // .stop() stoppar snúning
        void sleep(void); // .sleep() stoppar snúning og slekkur á driver/brú
        void wake(void); // .wake() kveikir á driver/brú
};

#endif