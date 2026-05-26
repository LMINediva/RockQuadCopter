#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x.h"                  // Device header

#define Motor_Port		GPIOA
#define Motor1_Pin		GPIO_Pin_0
#define Motor2_Pin		GPIO_Pin_1
#define Motor3_Pin		GPIO_Pin_2
#define Motor4_Pin		GPIO_Pin_3

void Motor_Init(void);
void Motor_Out(int16_t duty1, int16_t duty2, int16_t duty3, int16_t duty4);

#endif
