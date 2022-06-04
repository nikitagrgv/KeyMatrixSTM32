#pragma once
#include "stm32f10x.h"

namespace gpio
{
    enum class GPIOMode : uint8_t
    {
        in_Floating = 0x4,
        in_pullUpDown = 0x8,
        out_PushPull_50MHz = 0x3,
        out_OpenDrain_50MHz = 0x7,
        outAlt_PushPull_50MHz = 0xB
    };

    class Pin
    {
    private:
        GPIO_TypeDef *port;
        uint8_t pin;
        GPIOMode mode;

    public:
        Pin(GPIO_TypeDef *_port, uint8_t _pin, GPIOMode _mode);
        ~Pin();

        void setMode(GPIOMode mode);
        void setOutput(bool level);
        
        bool getInput() const;

        // getters
        GPIO_TypeDef *getPort() const { return port; }
        uint8_t getPin() const { return pin; }
        GPIOMode getMode() const { return mode; }
    };
}

namespace gpio
{
    void setMultiplePins(Pin *pins, uint32_t count, bool level);
}
