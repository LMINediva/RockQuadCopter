#ifndef __MYSPI_H
#define __MYSPI_H
#include "stm32f10x.h"                  // Device header

#define CSN_Port 	GPIOA
#define CSN_Pin		GPIO_Pin_4
#define SCK_Port	GPIOA
#define SCK_Pin		GPIO_Pin_5
#define MOSI_Port	GPIOA
#define MOSI_Pin	GPIO_Pin_7
#define MISO_Port	GPIOA
#define MISO_Pin	GPIO_Pin_6

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif
