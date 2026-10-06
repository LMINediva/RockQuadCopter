#ifndef __STRUCT_ALL_H
#define __STRUCT_ALL_H
#include "stm32f10x.h"                  // Device header

#include "Delay.h"
#include "Timer.h"
#include "DMA_ADC.h"
#include "Uart.h"
#include "Store.h"
#include "LED.h"
#include "Motor.h"
#include "MPU6050.h"
#include "NRF24L01.h"

/* MPU6050--加速度计结构体 */
struct _acc
{
	int16_t x;
	int16_t y;
	int16_t z;
};
extern struct _acc acc;

/* MPU6050--陀螺仪结构体 */
struct _gyro
{
	int16_t x;
	int16_t y;
	int16_t z;
};
extern struct _gyro gyro;

#endif
