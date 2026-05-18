#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"                  // Device header

extern uint8_t Count_1ms, Count_2ms, Count_4ms;

void Timer3_Init(uint16_t Handler_Frequency);

#endif
