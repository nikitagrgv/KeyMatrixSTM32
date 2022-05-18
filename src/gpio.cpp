#include "gpio.h"

namespace gpio
{
    Pin::Pin(GPIO_TypeDef *_port, uint8_t _pin, GPIOMode _mode) : port(_port),
                                                                  pin(_pin)
    {
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
