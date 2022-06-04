#include "stm32f10x.h"
#include "timers.h"
#include "misc.h"

namespace systick
{
    // callback function for SysTick
    static CallbackVoid callback = nullptr;
    static bool is_oneshot = true;

    // maximum 233 ms
    void waitMs(uint32_t ms)
    {
        // assert 24-bit value and non zero value
        assert_param(ms <= 233 && ms != 0);

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
    void waitMsInt(uint32_t ms, bool _is_oneshot)
    {
        is_oneshot = _is_oneshot;

        // assert 24-bit value and non zero value
        assert_param(ms <= 233 && ms != 0);
        // assert callback function
        assert_param(callback != nullptr);

        // set reload value
        SysTick->LOAD = ms * (SYS_FREQ / 1000);
        // start timer
        SysTick->CTRL = SysTick_CTRL_ENABLE |
                        SysTick_CTRL_CLKSOURCE |
                        SysTick_CTRL_TICKINT;
    }
    
    void resetCounter()
    {
        SysTick->VAL = 0;
    }
    
    void disableTimer()
    {
        SysTick->CTRL = 0;
    }

    void setCallback(CallbackVoid new_callback)
    {
        assert_param((SysTick->CTRL & SysTick_CTRL_ENABLE) == 0);
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

        if (systick::is_oneshot)
        {
            // disable timer
            SysTick->CTRL = 0;
        }

        // call callback function
        systick::callback();
    }
}
