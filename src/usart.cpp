#include "usart.h"
#include "rcc.h"
#include "misc.h"

namespace usart
{
    static CallbackReceive callback_USART1_receive = nullptr;
    static CallbackReceive callback_USART2_receive = nullptr;
    static CallbackReceive callback_USART3_receive = nullptr;
}

namespace usart
{
    Usart::Usart(USART_TypeDef *_usart_struct, uint32_t _baudrate)
    {
        assert_param(_usart_struct == USART1 && rcc::getClock(rcc::RCCPort::usart1) == true ||
                     _usart_struct == USART2 && rcc::getClock(rcc::RCCPort::usart2) == true ||
                     _usart_struct == USART3 && rcc::getClock(rcc::RCCPort::usart3) == true);

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

    void Usart::enableTransmitter()
    {
        usart_struct->CR1 |= USART_CR1_TE;
    }
    void Usart::enableReceiver()
    {
        usart_struct->CR1 |= USART_CR1_RE;
    }

    void Usart::disableTransmitter()
    {
        usart_struct->CR1 &= ~USART_CR1_TE;
    }
    void Usart::disableReceiver()
    {
        usart_struct->CR1 &= ~USART_CR1_RE;
    }

    void Usart::setReceiveCallback(CallbackReceive callback)
    {
        if (usart_struct == USART1)
        {
            NVIC_EnableIRQ(USART1_IRQn);
            callback_USART1_receive = callback;
        }
        else if (usart_struct == USART2)
        {
            NVIC_EnableIRQ(USART2_IRQn);
            callback_USART2_receive = callback;
        }
        else if (usart_struct == USART3)
        {
            NVIC_EnableIRQ(USART3_IRQn);
            callback_USART3_receive = callback;
        }

        // enable receive interrupt
        usart_struct->CR1 |= USART_CR1_RXNEIE;
    }

    // ----------- TODO: no waiting-------------------------
    void Usart::transmit(uint8_t *data, uint32_t size)
    {
        for (uint32_t i = 0; i < size; i++)
        {
            while (!(usart_struct->SR & USART_SR_TXE))
                ;
            usart_struct->DR = data[i];
        }
    }

}

extern "C"
{
    void USART1_IRQHandler(void)
    {
        // receive interrupt
        if (USART1->SR & USART_SR_RXNE)
        {
            usart::callback_USART1_receive((uint8_t)USART1->DR);
        }
    }

    void USART2_IRQHandler(void)
    {
        // receive interrupt
        if (USART2->SR & USART_SR_RXNE)
        {
            usart::callback_USART2_receive((uint8_t)USART2->DR);
        }
    }

    void USART3_IRQHandler(void)
    {
        // receive interrupt
        if (USART3->SR & USART_SR_RXNE)
        {
            usart::callback_USART3_receive((uint8_t)USART3->DR);
        }
    }
}
