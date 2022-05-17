#include "stm32f10x.h"
#include "gpio.h"

int main()
{

    {
        gpio::Pin pin1(GPIOA, 13, gpio::GPIOMode::out_OpenDrain_50MHz);
        gpio::Pin pin2(GPIOB, 5, gpio::GPIOMode::out_PushPull_50MHz);
        gpio::Pin pin3(GPIOA, 8, gpio::GPIOMode::outAlt_PushPull_50MHz);
        gpio::Pin pin4(GPIOA, 11, gpio::GPIOMode::out_OpenDrain_50MHz);
    }

    while (1)
        ;
}
