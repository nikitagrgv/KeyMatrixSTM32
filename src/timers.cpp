#include "timers.h"
#include "misc.h"

namespace systick
{
    // maximim 233 ms
    void waitMs(uint32_t ms)
    {
        // calculate reload value of the timer
        uint32_t reload_value = ms * (SYS_FREQ / 1000);

        // assert 24-bit value
        assert_param(reload_value > 0xFFFFFF);

        // clear timer
        SysTick->VAL = 0;
        // set reload value
        SysTick->LOAD = reload_value;
        // start timer
        SysTick->CTRL = SysTick_CTRL_ENABLE |
                        SysTick_CTRL_CLKSOURCE;
        // wait for timer
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG))
            ;
    }
}
