#pragma once
#include "stm32f10x.h"
#include "misc.h"

namespace systick
{
    void waitMs(uint32_t ms);
    void waitMsInt(uint32_t ms, bool _is_oneshot);
    void resetCounter();
    void disableTimer();
    void setCallback(CallbackVoid new_callback);
}

extern "C"
{
    void SysTick_Handler(void);
}
