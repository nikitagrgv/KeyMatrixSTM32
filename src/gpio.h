#pragma once
#include "stm32f10x.h"

namespace gpio
{
    enum class GPIOMode
    {
        inFloating = 0x4,
        outPushPull_50MHz = 0x3
    };

    class Pin
    {
    private:
        GPIO_TypeDef *port;
        uint8_t pin;
        GPIOMode mode;

        void setMode(GPIOMode mode);

    public:
        Pin(GPIO_TypeDef *_port, uint8_t _pin, GPIOMode _mode);
        ~Pin();
    };
}