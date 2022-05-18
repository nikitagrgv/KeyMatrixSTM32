#pragma once
#include "stm32f10x.h"

const uint32_t SYS_FREQ = 72e6;

// maximim 233 ms
void waitMs(uint32_t ms)
{
    // clear timer
    SysTick->VAL = 0;
    // set reload value
    SysTick->LOAD = ms * (SYS_FREQ / 1000);
    // start timer
    SysTick->CTRL = SysTick_CTRL_ENABLE |
                    SysTick_CTRL_CLKSOURCE;
    // wait for timer
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG))
        ;
}
