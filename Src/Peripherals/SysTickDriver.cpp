#include "Peripherals/SysTickDriver.h"
#include "stm32f103xb.h"

// static variable
volatile uint32_t SysTickDriver::msTicks = 0;

// SysTick interrupt handler
extern "C" void SysTick_Handler(void) {
    SysTickDriver::msTicks++;
}

void SysTickDriver::init(uint32_t clockHz) {
    systemClockHz = clockHz;

    // how many ticks for 1 ms
    uint32_t ticks_per_ms = clockHz / 1000U;
    if (ticks_per_ms == 0) ticks_per_ms = 1;

    // set reload value
    SysTick->LOAD = (ticks_per_ms - 1) & SysTick_LOAD_RELOAD_Msk;

    // clear current value
    SysTick->VAL = 0;

    // enable SysTick with interrupt
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}

// delay in microseconds (busy wait)
void SysTickDriver::delay_us(uint32_t microseconds) {
    uint32_t ticks = (systemClockHz / 1000000U) * microseconds;
    if (ticks == 0) return;

    while (ticks) {
        uint32_t batch = ticks;
        if (batch > SysTick_LOAD_RELOAD_Msk) batch = SysTick_LOAD_RELOAD_Msk;

        SysTick->LOAD = (batch - 1) & SysTick_LOAD_RELOAD_Msk;
        SysTick->VAL = 0;
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

        while (SysTick->VAL != 0) {
            // wait
        }

        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                        SysTick_CTRL_TICKINT_Msk |
                        SysTick_CTRL_ENABLE_Msk;

        ticks -= batch;
    }
}

// return ms counter
uint32_t SysTickDriver::millis() {
    return msTicks;
}

// delay in ms (uses millis counter)
void SysTickDriver::delay_ms(uint32_t milliseconds) {
    uint32_t start = millis();
    while ((millis() - start) < milliseconds) {
        // wait
    }
}
