#ifndef __MPU6050_H
#define __MPU6050_H
#include "stm32f10x.h"                  // Device header

// MPU6050的I2C从机地址
#define MPU6050_ADDRESS		0xD0

extern uint32_t I2C_Error;

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
uint8_t MPU6050_Init(void);
void MPU6050_SingleRead(void);
uint8_t MPU6050_SequenceRead(void);
void MPU6050_Compose(void);
uint8_t MPU6050_GetID(void);

#endif
