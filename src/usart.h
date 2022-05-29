#pragma once
#include "stm32f10x.h"

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

        inline void enableTransmitter();
        inline void enableReceiver();

        inline void disableTransmitter();
        inline void disableReceiver();

        void setReceiveCallback(CallbackReceive callback);
    };

}

extern "C"
{
    void USART1_IRQHandler(void);
    void USART2_IRQHandler(void);
    void USART3_IRQHandler(void);
}
