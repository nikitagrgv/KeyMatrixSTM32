#include "gpio.h"
#include "misc.h"
#include "rcc.h"

namespace gpio
{
    Pin::Pin(GPIO_TypeDef *_port, uint8_t _pin, GPIOMode _mode) : port(_port),
                                                                  pin(_pin)
    {
        assert_param(_port == GPIOA && rcc::getClock(rcc::RCCPort::gpioa) == true ||
                     _port == GPIOB && rcc::getClock(rcc::RCCPort::gpiob) == true ||
                     _port == GPIOC && rcc::getClock(rcc::RCCPort::gpioc) == true);

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

    void Pin::setOutput(bool level)
    {
        // assert output mode
        assert_param(((uint8_t)mode & 0b0011) != 0);

        if (level == true)
        {
            port->BSRR = 1u << pin;
        }
        else
        {
            port->BRR = 1u << pin;
        }
    }

    void Pin::setPullMode(bool level)
    {
        // assert pull up/down mode
        assert_param(mode == GPIOMode::in_pullUpDown);
        // set output bit to choose pull up/down
        if (level == true)
        {
            port->BSRR = 1u << pin;
        }
        else
        {
            port->BRR = 1u << pin;
        }
    }

    bool Pin::getInput() const
    {
        // read the bit with the input value
        return ((port->IDR >> pin) & 1);
    }
}

namespace gpio
{
    void setMultiplePins(Pin *pins, uint32_t count, bool level)
    {
        // GPIOA, GPIOB, GPIOC pins
        uint16_t gpio_pins[3] = {0, 0, 0};

        // set bits in corresponding gpio_pins
        for (uint32_t i = 0; i < count; ++i)
        {
            if (pins[i].getPort() == GPIOA)
                gpio_pins[0] |= 1u << pins[i].getPin();
            else if (pins[i].getPort() == GPIOB)
                gpio_pins[1] |= 1u << pins[i].getPin();
            else if (pins[i].getPort() == GPIOC)
                gpio_pins[2] |= 1u << pins[i].getPin();
        }

        if (level == true)
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
