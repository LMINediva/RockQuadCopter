#ifndef __LED_H
#define __LED_H

/******************************************************************************
							∫Í∂®“Â
*******************************************************************************/ 
#define LED_Port	GPIOB
#define LED_Pin		GPIO_Pin_4

void LED_Init(void);
void LED_ON(void);
void LED_OFF(void);
void LED_ON_OFF(void);

#endif
