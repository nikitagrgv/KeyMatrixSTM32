#include "stm32f10x.h"
#include "misc.h"
#include "rcc.h"
#include "gpio.h"
#include "timers.h"
#include "usart.h"
#include "keymatrix.h"
#include "modbus.h"

int main()
{
    // enable clock for GPIOA port
    rcc::setClock(rcc::RCCPort::gpioa, true);

    // initialize outputs (in rows) and inputs (in columns)
    gpio::Pin *out_pins = new gpio::Pin[3]{{GPIOA, 0, gpio::GPIOMode::out_OpenDrain_50MHz},
                                           {GPIOA, 1, gpio::GPIOMode::out_OpenDrain_50MHz},
                                           {GPIOA, 2, gpio::GPIOMode::out_OpenDrain_50MHz}};
    gpio::Pin *in_pins = new gpio::Pin[3]{{GPIOA, 3, gpio::GPIOMode::in_pullUpDown},
                                          {GPIOA, 4, gpio::GPIOMode::in_pullUpDown},
                                          {GPIOA, 5, gpio::GPIOMode::in_pullUpDown}};

    // pull up for input pins
    in_pins[0].setPullMode(true);
    in_pins[1].setPullMode(true);
    in_pins[2].setPullMode(true);

    // initialize usart RX, TX pins
    gpio::Pin usart_TX{GPIOA, 9, gpio::GPIOMode::outAlt_PushPull_50MHz};
    gpio::Pin usart_RX{GPIOA, 10, gpio::GPIOMode::in_pullUpDown};
    usart_RX.setPullMode(true); // pull up

    // enable clock and initialize usart
    rcc::setClock(rcc::RCCPort::usart1, true);
    usart::Usart usart1(USART1, 9600);

    // initialize 3x3 key matrix
    KeyMatrix::init(out_pins, 3, in_pins, 3, 10);

    // initialize modbus with an address 0x01, communication via usart1
    Modbus::init(0x01, &usart1, 32, 32);

    while (1)
    {
        // when the packet is received, go to process packet 
        if (Modbus::getState() == ModbusState::received)
        {
            Modbus::processPacket();
        }
        // when the packet is processed, go to transmit packet
        if (Modbus::getState() == ModbusState::processed)
        {
            Modbus::transmitPacket();
        }
    }
}
