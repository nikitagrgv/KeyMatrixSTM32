#pragma once
#include "stm32f10x.h"

const uint32_t SYS_FREQ = 72e6;

namespace systick
{
    void waitMs(uint32_t ms);
    void waitMsInt(uint32_t ms);
    void setCallback(void (*new_callback)(void));
}

extern "C"
{
    void SysTick_Handler(void);
}
