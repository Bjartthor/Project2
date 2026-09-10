#pragma once

class Timer_msec
{
public:
    Timer_msec(int period_ms) : period_ms(period_ms) {}

    void init();

private:
    int period_ms;
};