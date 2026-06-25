#include "MyI2C.h"
#include "Delay.h"

/**
 * 函数：I2C写SCL引脚电平
 * 参数：BitValue 协议层传入当前需要写入SCL的电平，范围：0~1
 * 返回值：无
 * 注意事项：此函数需要用户实现内容，
 * 当BitValue为0时，需要置SCL为低电平
 * 当BitValue为1时，需要置SCL为高电平
 */
void MyI2C_W_SCL(uint8_t BitValue)
{
	// 根据BitValue，设置SCL引脚的电平
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
	// 延时10us，防止时序频率超过要求
	Delay_us(10);
}

/**
 * 函数：I2C写SDA引脚电平
 * 参数：BitValue 协议层传入当前需要写入SDA的电平，范围：0~1
 * 返回值：无
 * 注意事项：此函数需要用户实现内容，
 * 当BitValue为0时，需要置SDA为低电平
 * 当BitValue为1时，需要置SDA为高电平
 */
void MyI2C_W_SDA(uint8_t BitValue)
{
	// 根据BitValue，设置SDA引脚的电平
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	// 延时10us，防止时序频率超过要求
	Delay_us(10);
}

/**
 * 函数：I2C读SDA引脚电平
 * 参数：无
 * 返回值：协议层需要得到当前SDA的电平，范围：0~1
 * 注意事项：此函数需要用户实现内容，
 * 当SDA为低电平时，返回0
 * 当SDA为高电平时，返回1
 */
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	// 读取SDA电平
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	// 延时10us，防止时序频率超过要求
	Delay_us(10);
	// 返回SDA电平
	return BitValue;
}

/**
 * 函数：I2C初始化
 * 参数：无
 * 返回值：无
 * 注意事项：此函数需要用户实现内容，实现SCL和SDA引脚的初始化
 */
void MyI2C_Init(void)
{
	/* 开启时钟 */
	// 开启GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* GPIO初始化 */
	// 将PB10和PB11引脚初始化为开漏输出
	GPIO_InitTypeDef GPIO_InitStructure;
	// 开漏输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	// PB10：SCL引脚，PB11：SDA引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	// 输出速度为50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO初始化
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* 设置默认电平 */
	// 设置PB10和PB11引脚初始化后默认为高电平（释放总线状态）
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

/**
 * 函数：I2C起始
 * 参数：无
 * 返回值：无
 */
void MyI2C_Start(void)
{
	// 释放SDA，确保SDA为高电平
	MyI2C_W_SDA(1);
	// 释放SCL，确保SCL为高电平
	MyI2C_W_SCL(1);
	// 在SCL高电平期间，拉低SDA，产生起始信号
	MyI2C_W_SDA(0);
	// 起始后，把SCL也拉低，既为了占用总线，也为了方便总线时序的拼接
	MyI2C_W_SCL(0);
}

/**
 * 函数：I2C终止
 * 参数：无
 * 返回值：无
 */
void MyI2C_Stop(void)
{
	// 拉低SDA，确保SDA为低电平
	MyI2C_W_SDA(0);
	// 释放SCL，使SCL呈现高电平
	MyI2C_W_SCL(1);
	// 在SCL高电平期间，释放SDA，产生终止信号
	MyI2C_W_SDA(1);
}

/**
 * 函数：I2C发送一个字节
 * 参数：Byte 要发送的一个字节数据，范围：0x00~0xFF
 * 返回值：无
 */
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	// 循环8次，主机依次发送数据的每一位
	for (i = 0; i < 8; i++)
	{
		// 使用掩码的方式取出Byte的指定一位数据并写入到SDA线
		MyI2C_W_SDA(Byte & (0x80 >> i));
		// 释放SCL，从机在SCL高电平期间读取SDA
		MyI2C_W_SCL(1);
		// 拉低SCL，主机开始发送下一位数据
		MyI2C_W_SCL(0);
	}
}

/**
 * 函数：I2C接收一个字节
 * 参数：无
 * 返回值：接收到的一个字节数据，范围：0x00~0xFF
 */
uint8_t MyI2C_ReceiveByte(void)
{
	// 定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
	uint8_t i, Byte = 0x00;
	// 接收前，主机先确保释放SDA，避免干扰从机的数据发送
	MyI2C_W_SDA(1);
	// 循环8次，主机依次接收数据的每一位
	for (i = 0; i < 8; i++)
	{
		// 释放SCL，主机在SCL高电平期间读取SDA
		MyI2C_W_SCL(1);
		// 读取SDA数据，并存储到Byte变量中
		if (MyI2C_R_SDA() == 1)
		{
			// 当SDA为1时，置变量指定位为1，
			// 当SDA为0时，不做处理，指定位为默认的初值0
			Byte |= (0x80 >> i);
		}
		// 拉低SCL，从机在SCL低电平期间写入SDA
		MyI2C_W_SCL(0);
	}
	// 返回接收到的一个字节数据
	return Byte;
}

/**
 * 函数：I2C发送应答位
 * 参数：AckBit 要发送的应答位，范围：0~1，0表示应答，1表示非应答
 * 返回值：无
 */
void MyI2C_SendAck(uint8_t AckBit)
{
	// 主机把应答位数据放到SDA线
	MyI2C_W_SDA(AckBit);
	// 释放SCL，从机在SCL高电平期间，读取应答位
	MyI2C_W_SCL(1);
	// 拉低SCL，开始下一个时序模块
	MyI2C_W_SCL(0);
}

/**
 * 函数：I2C接收应答位
 * 参数：无
 * 返回值：接收到的应答位，范围：0~1，0表示应答，1表示非应答
 */
uint8_t MyI2C_ReceiveAck(void)
{
	// 定义应答位变量
	uint8_t AckBit;
	// 接收前，主机先确保释放SDA，避免干扰从机的数据发送
	MyI2C_W_SDA(1);
	// 释放SCL，主机在SCL高电平期间读取SDA
	MyI2C_W_SCL(1);
	// 将应答位存储到变量里
	AckBit = MyI2C_R_SDA();
	// 拉低SCL，开始下一个时序模块
	MyI2C_W_SCL(0);
	// 返回定义应答位变量
	return AckBit;
}
