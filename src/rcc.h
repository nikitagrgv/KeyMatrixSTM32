#pragma once
#include "stm32f10x.h"
#include "misc.h"

namespace rcc
{
    // +0 - APB1
    // +32 - APB2
    // +64 - AHB
    enum class RCCPort : uint8_t
    {
        usart2 = 17,
        usart3 = 18,

        gpioa = 2 + 32,
        gpiob = 3 + 32,
        gpioc = 4 + 32,
        usart1 = 14 + 32
    };

    void setClock(RCCPort port, bool mode);
    bool getClock(RCCPort port);
}
