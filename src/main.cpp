#include "stm32f10x.h"
#include "misc.h"
#include "gpio.h"



int main()
{

    gpio::Pin pins[] = {
        {GPIOA, 2, gpio::GPIOMode::out_OpenDrain_50MHz},
        {GPIOB, 5, gpio::GPIOMode::out_PushPull_50MHz},
        {GPIOA, 23, gpio::GPIOMode::outAlt_PushPull_50MHz}, // assert failed
        {GPIOA, 11, gpio::GPIOMode::in_Floating}};


    pins[0].setOutput(gpio::Level::High);
    pins[1].setOutput(gpio::Level::High);
    pins[2].setOutput(gpio::Level::High);
    pins[3].setOutput(gpio::Level::High); // assert failed
    pins[2].setOutput(gpio::Level::High);


    while (1)
        ;
}
