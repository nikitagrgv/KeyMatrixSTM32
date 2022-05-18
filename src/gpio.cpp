#include "gpio.h"
#include "misc.h"

namespace gpio
{
    Pin::Pin(GPIO_TypeDef *_port, uint8_t _pin, GPIOMode _mode) : port(_port),
                                                                  pin(_pin)
    {
        assert_param(_port == GPIOA || _port == GPIOB || _port == GPIOC);
        assert_param(_pin < 16);

        setMode(_mode);
    }

    Pin::~Pin()
    {
        setMode(GPIOMode::in_Floating);
    }

    void Pin::setMode(GPIOMode _mode)
    {
        mode = _mode;
        if (pin < 8)
        {
            uint32_t crl_tmp = port->CRL;
            // clear the previous mode
            crl_tmp &= ~(0xFu << (pin * 4));
            // write the new mode
            crl_tmp |= (uint32_t)_mode << (pin * 4);
            // apply
            port->CRL = crl_tmp;
        }
        else
        {
            uint32_t crh_tmp = port->CRH;
            // clear the previous mode
            crh_tmp &= ~(0xFu << ((pin - 8) * 4));
            // write the new mode
            crh_tmp |= (uint32_t)_mode << ((pin - 8) * 4);
            // apply
            port->CRH = crh_tmp;
        }
    }

    void Pin::setOutput(Level level)
    {
        // assert output mode
        assert_param((uint8_t)mode & 0x3 != 0x0);

        if (level == Level::High)
        {
            port->BSRR = GPIO_BSRR_BS0 + pin;
        }
        else
        {
            port->BSRR = GPIO_BSRR_BR0 + pin;
        }
    }

    Level Pin::getInput() const
    {
        // read the bit with the input value
        Level level = (Level)((port->IDR >> pin) & 1);
        return level;
    }
}

namespace gpio
{
    void setMultiplePins(Pin *pins, uint32_t count, Level level)
    {
        // GPIOA, GPIOB, GPIOC pins
        uint16_t gpio_pins[3];

        // set bits in corresponding gpio_pins
        for (uint32_t i = 0; i < count; ++i)
        {
            if (pins[i].getPort() == GPIOA)
                gpio_pins[0] |= 1 << pins[i].getPin();
            else if (pins[i].getPort() == GPIOB)
                gpio_pins[1] |= 1 << pins[i].getPin();
            else if (pins[i].getPort() == GPIOC)
                gpio_pins[2] |= 1 << pins[i].getPin();
        }

        if (level == Level::High)
        {
            GPIOA->BSRR = gpio_pins[0];
            GPIOB->BSRR = gpio_pins[1];
            GPIOC->BSRR = gpio_pins[2];
        }
        else
        {
            GPIOA->BRR = gpio_pins[0];
            GPIOB->BRR = gpio_pins[1];
            GPIOC->BRR = gpio_pins[2];
        }
    }
}
