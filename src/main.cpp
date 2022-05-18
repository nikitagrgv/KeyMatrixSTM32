#include "stm32f10x.h"
#include "misc.h"
#include "rcc.h"
#include "gpio.h"
#include "timers.h"

enum class KeyMatrixState : uint8_t
{
    idle,
    processing,
    finished
};

class KeyMatrix
{
private:
    gpio::Pin out_pins[3];
    gpio::Pin in_pins[3];
    uint8_t current_out_pin;
    KeyMatrixState state;

public:
    KeyMatrix() : out_pins({{GPIOA, 0, gpio::GPIOMode::out_OpenDrain_50MHz},
                            {GPIOA, 1, gpio::GPIOMode::out_OpenDrain_50MHz},
                            {GPIOA, 2, gpio::GPIOMode::out_OpenDrain_50MHz}}),
                  in_pins({{GPIOA, 3, gpio::GPIOMode::in_Floating},
                           {GPIOA, 4, gpio::GPIOMode::in_Floating},
                           {GPIOA, 5, gpio::GPIOMode::in_Floating}}),
                  current_out_pin(0),
                  state(KeyMatrixState::idle)
    {
        gpio::setMultiplePins(out_pins, 3, gpio::Level::High);
    }

    ~KeyMatrix();
};

int main()
{
    rcc::setClock(rcc::RCCPort::gpioa, rcc::ClockMode::On);
    KeyMatrix keymatrix;

    gpio::Pin pin(GPIOA, 6, gpio::GPIOMode::out_PushPull_50MHz);

    auto invert = [](void) -> void
    {
        GPIOA->ODR ^= GPIO_ODR_ODR6;
    };

    systick::setCallback(invert);

    systick::waitMsInt(12);


    while (1)
        ;
}
