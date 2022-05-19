#include "stm32f10x.h"
#include "timers.h"
#include "misc.h"

namespace systick
{
    // callback function for SysTick
    static callbackFunction callback = nullptr;

    // maximim 233 ms
    void waitMs(uint32_t ms)
    {
        // assert 24-bit value
        assert_param(ms <= 233);

        // set reload value
        SysTick->LOAD = ms * (SYS_FREQ / 1000);
        // start timer
        SysTick->CTRL = SysTick_CTRL_ENABLE |
                        SysTick_CTRL_CLKSOURCE;
        // wait for timer
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG))
            ;
        // disable timer
        SysTick->CTRL = 0;
    }

    // wait with interrupt
    void waitMsInt(uint32_t ms)
    {
        // assert 24-bit value
        assert_param(ms <= 233);
        // assert callback function
        assert_param(callback != nullptr);

        // set reload value
        SysTick->LOAD = ms * (SYS_FREQ / 1000);
        // start timer
        SysTick->CTRL = SysTick_CTRL_ENABLE |
                        SysTick_CTRL_CLKSOURCE |
                        SysTick_CTRL_TICKINT;
    }

    void setCallback(callbackFunction new_callback)
    {
        assert_param((SysTick->CTRL & SysTick_CTRL_ENABLE) != 0);
        callback = new_callback;
    }
}

extern "C"
{
    // SysTick interrupt handler
    void SysTick_Handler(void)
    {
        // clear interrupt flag
        SysTick->VAL = 0;
        // disable timer
        SysTick->CTRL = 0;

        // call callback function
        systick::callback();
    }
}
