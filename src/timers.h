#pragma once
#include "stm32f10x.h"
#include "misc.h"

const uint32_t SYS_FREQ = 72e6;

namespace systick
{
    void waitMs(uint32_t ms);
    void waitMsInt(uint32_t ms);
    void setCallback(callbackFunction new_callback);
}

extern "C"
{
    void SysTick_Handler(void);
}
