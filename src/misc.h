#pragma once
#include "stm32f10x.h"

const uint32_t SYS_FREQ = 72e6;

void waitMs(uint32_t ms);

#ifdef USE_FULL_ASSERT
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif
