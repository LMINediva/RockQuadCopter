#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "Delay.h"

/**
 * 函数：按键初始化
 * 参数：无
 * 返回值：无
 */
void Key_Init(void)
{
	// 开启GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	// 上拉输入IPU（In Pull Up）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * 函数：按键获取键码
 * 参数：无
 * 返回值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
 */
uint8_t Key_GetNum(void)
{
	// 定义变量，默认键码值为0
	uint8_t KeyNum = 0;
	
	// 读取GPIO口B1输入寄存器的状态，如果为0，则代表按键1按下
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) 
	{
		// 延时消抖
		Delay_ms(20);
		// 等待按键松手
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);
		// 延时消抖
		Delay_ms(20);
		// 置键码为1
		KeyNum = 1;
	}
	
	// 读取GPIO口B11输入寄存器的状态，如果为0，则代表按键1按下
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0) 
	{
		// 延时消抖
		Delay_ms(20);
		// 等待按键松手
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);
		// 延时消抖
		Delay_ms(20);
		// 置键码为2
		KeyNum = 2;
	}
	
	return KeyNum;
}
