#include "rcc.h"

namespace rcc
{
    void setClock(RCCPort port, ClockMode mode)
    {
        // APB1 port
        if ((uint8_t)port / 32 == 0)
        {
            setBit((uint32_t) & (RCC->APB1ENR), (uint32_t)port - 0, (bool)mode);
        }
        // APB2 port
        else if ((uint8_t)port / 32 == 1)
        {
            setBit((uint32_t) & (RCC->APB2ENR), (uint32_t)port - 32, (bool)mode);
        }
        // AHB port
        else
        {
            setBit((uint32_t) & (RCC->AHBENR), (uint32_t)port - 64, (bool)mode);
        }
    }

    ClockMode getClock(RCCPort port)
    {
        // APB1 port
        if ((uint8_t)port / 32 == 0)
        {
            return (ClockMode)getBit((uint32_t)&RCC->APB1ENR, (uint32_t)port - 0);
        }
        // APB2 port
        else if ((uint8_t)port / 32 == 1)
        {
            return (ClockMode)getBit((uint32_t)&RCC->APB2ENR, (uint32_t)port - 32);
        }
        // AHB port
        else
        {
            return (ClockMode)getBit((uint32_t)&RCC->AHBENR, (uint32_t)port - 64);
        }
    }
}
