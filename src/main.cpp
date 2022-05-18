#include "stm32f10x.h"
#include "gpio.h"

int main()
{

    gpio::Pin pins[] = {
        {GPIOA, 13, gpio::GPIOMode::out_OpenDrain_50MHz},
        {GPIOB, 5, gpio::GPIOMode::out_PushPull_50MHz},
        {GPIOA, 8, gpio::GPIOMode::outAlt_PushPull_50MHz},
        {GPIOA, 11, gpio::GPIOMode::out_OpenDrain_50MHz}};

    while (1)
        ;
}
