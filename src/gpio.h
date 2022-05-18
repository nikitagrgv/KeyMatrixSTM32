#pragma once
#include "stm32f10x.h"

namespace gpio
{
    enum class GPIOMode : uint8_t
    {
        in_Floating = 0x4,
        out_PushPull_50MHz = 0x3,
        out_OpenDrain_50MHz = 0x7,
        outAlt_PushPull_50MHz = 0xB
    };

    enum class Level : uint8_t
    {
        Low = 0,
        High = 1
    };

    class Pin
    {
    private:
        GPIO_TypeDef *port;
        uint8_t pin;
        GPIOMode mode;

        friend void setMultiplePins(Pin *pins, uint32_t count, Level level);

    public:
        Pin(GPIO_TypeDef *_port, uint8_t _pin, GPIOMode _mode);
        ~Pin();

        void setMode(GPIOMode mode);
        void setOutput(Level level);
        Level getInput() const;
    };
}
