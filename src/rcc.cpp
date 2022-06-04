#include "rcc.h"

namespace rcc
{
    void setClock(RCCPort port, bool mode)
    {
        switch ((uint8_t)port / 32)
        {
        // APB1 port
        case 0:
            setBit((uint32_t) & (RCC->APB1ENR), (uint8_t)port - 0, mode);
            break;
        // APB2 port
        case 1:
            setBit((uint32_t) & (RCC->APB2ENR), (uint8_t)port - 32, mode);
            break;
        // AHB port
        case 2:
            setBit((uint32_t) & (RCC->AHBENR), (uint8_t)port - 64, mode);
            break;
        }
    }

    bool getClock(RCCPort port)
    {
        switch ((uint8_t)port / 32)
        {
        // APB1 port
        case 0:
            return getBit((uint32_t)&RCC->APB1ENR, (uint8_t)port - 0);
        // APB2 port
        case 1:
            return getBit((uint32_t)&RCC->APB2ENR, (uint8_t)port - 32);
        // AHB port
        default:
            return getBit((uint32_t)&RCC->AHBENR, (uint8_t)port - 64);
        }
    }
}
