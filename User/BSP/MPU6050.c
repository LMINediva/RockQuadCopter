#include "MPU6050.h"
#include "MPU6050_Reg.h"
#include "MyI2C.h"
#include "Struct_All.h"

// I2C错误
uint32_t I2C_Error = 0;
// I2C读取MPU6050数据缓存
static uint8_t MPU6050_Buffer[14];

/**
 * 函数：MPU6050写寄存器
 * 参数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
 * 参数：Data 要写入寄存器的数据，范围：0x00~0xFF
 * 返回值：无
 */
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	// I2C起始
	MyI2C_Start();
	// 发送从机地址，读写位为0，表示即将写入
	MyI2C_SendByte(MPU6050_ADDRESS);
	// 接收应答
	MyI2C_ReceiveAck();
	// 发送寄存器地址
	MyI2C_SendByte(RegAddress);
	// 接收应答
	MyI2C_ReceiveAck();
	// 发送要写入寄存器的数据
	MyI2C_SendByte(Data);
	// 接收应答
	MyI2C_ReceiveAck();
	// I2C终止
	MyI2C_Stop();
}

/**
 * 函数：MPU6050读寄存器
 * 参数：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
 * 返回值：读取寄存器的数据，范围：0x00~0xFF
 */
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	// I2C起始
	MyI2C_Start();
	// 发送从机地址，读写位为0，表示即将写入
	MyI2C_SendByte(MPU6050_ADDRESS);
	// 接收应答
	MyI2C_ReceiveAck();
	// 发送寄存器地址
	MyI2C_SendByte(RegAddress);
	// 接收应答
	MyI2C_ReceiveAck();
	
	// I2C重复起始
	MyI2C_Start();
	// 发送从机地址，读写位为1，表示即将读取
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	// 接收应答
	MyI2C_ReceiveAck();
	// 接收指定寄存器的数据
	Data = MyI2C_ReceiveByte();
	// 发送应答，给从机非应答，终止从机的数据输出
	MyI2C_SendAck(1);
	// I2C终止
	MyI2C_Stop();
	
	return Data;
}

/**
 * 函数：MPU6050初始化
 * 参数：无
 * 返回值：0 出错；1 成功
 */
uint8_t MPU6050_Init(void)
{
	// 先初始化底层的I2C
	MyI2C_Init();
	
	// 检查MPU6050是否正常
	if (MPU6050_ReadReg(MPU6050_WHO_AM_I) != 0x68)
	{
		return 0;
	}
	
	/* MPU6050寄存器初始化，需要对照MPU6050手册的寄存器描述配置，
	此处仅配置了部分重要的寄存器 */
	// 电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	// 电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	// 采样率分频寄存器，配置采样率为1，不分频（8KHZ）
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x00);
	// 配置数字低通滤波器，不启用数字低通滤波器
	MPU6050_WriteReg(MPU6050_CONFIG, 0x00);
	// 陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	// 加速度计配置寄存器，选择满量程为±16g，禁用加速度计的数字高通滤波器
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x1F);
	
	return 1;
}

/**
 * 函数：分次读MPU6050数据寄存器
 * 参数：无
 * 返回值：无
 */
void MPU6050_SingleRead(void)
{
	// 加速度计X、Y和Z轴的高8位和低8位数据
	MPU6050_Buffer[0] = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	MPU6050_Buffer[1] = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	MPU6050_Buffer[2] = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	MPU6050_Buffer[3] = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	MPU6050_Buffer[4] = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	MPU6050_Buffer[5] = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	
	// 陀螺仪X、Y和Z轴的高8位和低8位数据
	MPU6050_Buffer[8] = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	MPU6050_Buffer[9] = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	MPU6050_Buffer[10] = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	MPU6050_Buffer[11] = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	MPU6050_Buffer[12] = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	MPU6050_Buffer[13] = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
}

/**
 * 函数：连续读MPU6050数据寄存器
 * 参数：无
 * 返回值：0 出错；1 成功
 */
uint8_t MPU6050_SequenceRead(void)
{
	
	uint8_t i, ack;
	
	// I2C起始
	MyI2C_Start();
	// 发送从机地址，读写位为0，表示即将写入
	MyI2C_SendByte(MPU6050_ADDRESS);
	// 接收应答
	ack = MyI2C_ReceiveAck();
	// 判断应答是否为非应答
	if (ack == 1)
	{
		// I2C终止
		MyI2C_Stop();
		return 0;
	}
	// 发送寄存器地址
	MyI2C_SendByte(MPU6050_ACCEL_XOUT_H);
	// 接收应答
	MyI2C_ReceiveAck();
	
	// I2C重复起始
	MyI2C_Start();
	// 发送从机地址，读写位为1，表示即将读取
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	// 接收应答
	MyI2C_ReceiveAck();
	
	// 连续读取数据
	for (i = 0; i < 14; i++)
	{
		// 读取一个字节
		MPU6050_Buffer[i] = MyI2C_ReceiveByte();
		// 如果是最后一个字节
		if (i == 13)
		{
			// 给从机发送非应答，终止从机的数据输出
			MyI2C_SendAck(1);
		}
		else
		{
			// 给从机发送应答，继续接收从机的数据输出
			MyI2C_SendAck(0);
		}
	}
	
	// I2C终止
	MyI2C_Stop();
	
	return 1;
}

/**
 * 函数：合成MPU6050的16位数据
 * 参数：无
 * 返回值：无
 */
void MPU6050_Compose(void)
{
	// 数据拼接，合成加速度计X、Y和Z轴的16位数据
	acc.x = ((((int16_t)MPU6050_Buffer[0]) << 8) | MPU6050_Buffer[1]);
	acc.y = ((((int16_t)MPU6050_Buffer[2]) << 8) | MPU6050_Buffer[3]);
	acc.z = ((((int16_t)MPU6050_Buffer[4]) << 8) | MPU6050_Buffer[5]);
	
	// 数据拼接，合成陀螺仪X、Y和Z轴的16位数据
	gyro.x = ((((int16_t)MPU6050_Buffer[8]) << 8) | MPU6050_Buffer[9]);
	gyro.y = ((((int16_t)MPU6050_Buffer[10]) << 8) | MPU6050_Buffer[11]);
	gyro.z = ((((int16_t)MPU6050_Buffer[12]) << 8) | MPU6050_Buffer[13]);
}

/**
 * 函数：MPU6050获取ID号
 * 参数：无
 * 返回值：MPU6050的ID号
 */
uint8_t MPU6050_GetID(void)
{
	// 返回WHO_AM_I寄存器的值
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}
