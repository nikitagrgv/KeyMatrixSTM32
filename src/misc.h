#pragma once
#include "stm32f10x.h"

void setBit(uint32_t address, uint8_t bit, bool value);
bool getBit(uint32_t address, uint8_t bit);




#ifdef USE_FULL_ASSERT
#define assert_param(expr) ((expr) ? (void)0 : assert_failed(__FILE__, __LINE__))
    void assert_failed(const char *file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif

