#pragma once
#include "stm32f10x.h"
#include "misc.h"

namespace usart
{
    typedef void (*CallbackReceive)(uint8_t data);

    class Usart
    {
    private:
        USART_TypeDef *usart_struct;

    public:
        Usart(USART_TypeDef *_usart_struct, uint32_t _baudrate);
        ~Usart();

        void enableTransmitter();
        void enableReceiver();

        void disableTransmitter();
        void disableReceiver();

        void setReceiveCallback(CallbackReceive callback);

        void transmit(uint8_t *data, uint32_t size);

        inline uint32_t getBaudrate() { return SYS_FREQ / usart_struct->BRR; }
    };

}

extern "C"
{
    void USART1_IRQHandler(void);
    void USART2_IRQHandler(void);
    void USART3_IRQHandler(void);
}
