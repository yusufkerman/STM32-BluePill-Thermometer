#ifndef PERIPHERALS_TIMERDRIVER_H_
#define PERIPHERALS_TIMERDRIVER_H_

#include <stdint.h>

// Simple SysTick driver
class SysTickDriver {
private:
    uint32_t systemClockHz;

public:
    SysTickDriver() : systemClockHz(0) {}

    // init with CPU clock frequency (Hz)
    void init(uint32_t clockHz);

    // blocking delays
    void delay_ms(uint32_t milliseconds);
    void delay_us(uint32_t microseconds);

    // return uptime in ms
    uint32_t millis();

    static volatile uint32_t msTicks; // millisecond counter

};

#endif // PERIPHERALS_TIMERDRIVER_H_
