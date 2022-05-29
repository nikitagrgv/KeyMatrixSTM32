#include "usart.h"
#include "rcc.h"
#include "misc.h"

namespace usart
{
    Usart::Usart(USART_TypeDef *_usart_struct, uint32_t _baudrate)
    {
        assert_param(_usart_struct == USART1 && rcc::getClock(rcc::RCCPort::usart1) == rcc::ClockMode::On ||
                     _usart_struct == USART2 && rcc::getClock(rcc::RCCPort::usart2) == rcc::ClockMode::On ||
                     _usart_struct == USART3 && rcc::getClock(rcc::RCCPort::usart3) == rcc::ClockMode::On);

        usart_struct = _usart_struct;
        // set baudrate
        usart_struct->BRR = (uint16_t)(SYS_FREQ / _baudrate);
        // enable USART
        usart_struct->CR1 |= USART_CR1_UE;
    }

    Usart::~Usart()
    {
        // disable USART
        usart_struct->CR1 &= ~USART_CR1_UE;
    }

    inline void Usart::enableTransmitter()
    {
        usart_struct->CR1 |= USART_CR1_TE;
    }
    inline void Usart::enableReceiver()
    {
        usart_struct->CR1 |= USART_CR1_RE;
    }

    inline void Usart::disableTransmitter()
    {
        usart_struct->CR1 &= ~USART_CR1_TE;
    }
    inline void Usart::disableReceiver()
    {
        usart_struct->CR1 &= ~USART_CR1_RE;
    }

    void Usart::setReceiveCallback(CallbackReceive callback)
    {
        if (usart_struct == USART1)
        {
            callback_USART1_receive = callback;
        }
        else if (usart_struct == USART2)
        {
            callback_USART2_receive = callback;
        }
        else if (usart_struct == USART3)
        {
            callback_USART3_receive = callback;
        }
    }

}

namespace usart
{
    static CallbackReceive callback_USART1_receive = nullptr;
    static CallbackReceive callback_USART2_receive = nullptr;
    static CallbackReceive callback_USART3_receive = nullptr;
}

extern "C"
{
    void USART1_IRQHandler(void)
    {
        // receive interrupt
        if (USART1->SR & USART_SR_RXNE)
        {
            usart::callback_USART1_receive(USART1->DR);
        }
    }

    void USART2_IRQHandler(void)
    {
        // receive interrupt
        if (USART2->SR & USART_SR_RXNE)
        {
            usart::callback_USART2_receive(USART2->DR);
        }
    }

    void USART3_IRQHandler(void)
    {
        // receive interrupt
        if (USART3->SR & USART_SR_RXNE)
        {
            usart::callback_USART3_receive(USART3->DR);
        }
    }
}
