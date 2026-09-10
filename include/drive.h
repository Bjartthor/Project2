#ifndef DRIVE_H
#define DRIVE_H

class Drive {
    private:
        int Pfwd;
        int Prev;
        int speed;
        int pwmval;
    public:
        Drive(int Pfwd_arg,int Prev_arg);
        void init(void);
        void fwd(int speed);
        void rev(int speed);
        void stop(void);
};

#endif