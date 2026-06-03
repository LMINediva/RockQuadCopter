#ifndef __DMA_ADC_H
#define __DMA_ADC_H
#include "stm32f10x.h"                  // Device header

#define Battery_Port	GPIOB
#define Battery_Pin		GPIO_Pin_1
#define M 2

extern uint16_t ADC_Value[M];
extern uint16_t Battery;

void ADC1_Init(void);
void Voltage_Printf(void);

#endif
