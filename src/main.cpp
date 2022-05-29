#include "stm32f10x.h"
#include "misc.h"
#include "rcc.h"
#include "gpio.h"
#include "timers.h"
#include "usart.h"
#include "keymatrix.h"

volatile uint16_t GGG = 0;

void ggg(uint16_t g)
{
    GGG = g;
}

int main()
{
    rcc::setClock(rcc::RCCPort::gpioa, rcc::ClockMode::On);

    gpio::Pin *out_pins = new gpio::Pin[3]{{GPIOA, 0, gpio::GPIOMode::out_OpenDrain_50MHz},
                                           {GPIOA, 1, gpio::GPIOMode::out_OpenDrain_50MHz},
                                           {GPIOA, 2, gpio::GPIOMode::out_OpenDrain_50MHz}};
    gpio::Pin *in_pins = new gpio::Pin[3]{{GPIOA, 3, gpio::GPIOMode::in_Floating},
                                          {GPIOA, 4, gpio::GPIOMode::in_Floating},
                                          {GPIOA, 5, gpio::GPIOMode::in_Floating}};

    KeyMatrix::init(out_pins, 3, in_pins, 3, 1);

    KeyMatrix::startProcessing();

    while (KeyMatrix::getState() != KeyMatrixState::finished)
        ;

    rcc::setClock(rcc::RCCPort::usart1, rcc::ClockMode::On);
    usart::Usart usart1(USART1, 9600);
    usart1.setReceiveCallback(ggg);
    usart1.enableReceiver();

    while (1)
        ;
}
