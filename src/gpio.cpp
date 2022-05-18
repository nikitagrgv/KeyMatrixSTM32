#include "gpio.h"

namespace gpio
{

    void Pin::setMode(GPIOMode _mode)
    {
        mode = _mode;
        if (pin < 8)
        {
            uint32_t crl_tmp = port->CRL;
            // clear current mode
            crl_tmp &= ~(0xFu << (pin * 4));
            // set floating input
            crl_tmp |= (uint32_t)_mode << (pin * 4);
            // write
            port->CRL = crl_tmp;
        }
        else
        {
            uint32_t crh_tmp = port->CRH;
            // clear current mode
            crh_tmp &= ~(0xFu << ((pin - 8) * 4));
            // set floating input
            crh_tmp |= (uint32_t)_mode << ((pin - 8) * 4);
            // write
            port->CRH = crh_tmp;
        }
    }

    Pin::Pin(GPIO_TypeDef *_port, uint8_t _pin, GPIOMode _mode) : port(_port),
                                                                  pin(_pin)
    {
        setMode(_mode);
    }

    Pin::~Pin()
    {
        setMode(GPIOMode::in_Floating);
    }
}
