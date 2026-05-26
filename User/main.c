#include "Struct_All.h"
#include "Tasks.h"
#include "OLED.h"
#include "Key.h"

// volatile uint16_t Num_1ms, Num_2ms, Num_4ms;

// 定义用于接收按键键码的变量
uint8_t KeyNum;
// 定义速度变量
int16_t Speed;

int main(void)
{	
	// OLED初始化
	OLED_Init();
	// 板级支持包中的硬件驱动初始化
	BSP_Init();
	// 按键初始化
	Key_Init();
	
	// 1行1列显示字符串Speed:
	OLED_ShowString(1, 1, "Speed:");
	
	while (1)
	{
		// 获取按键键码
		KeyNum = Key_GetNum();
		// 按键1按下
		if (KeyNum == 1)
		{
			Speed += 200;
			// 速度变量超过1000后
			if (Speed > 1000)
			{
				Speed = -1000;
			}
		}
		
		// 设置直流电机的速度为速度变量
		Motor_Out(Speed, Speed, Speed, Speed);
		// OLED显示速度变量
		OLED_ShowSignedNum(1, 7, Speed, 4);
		
		/*
		if (Count_1ms >= 1)
		{
			Num_1ms++;
			Count_1ms = 0;
		}
		if (Count_2ms >= 2)
		{
			Num_2ms++;
			Count_2ms = 0;
		}
		if (Count_4ms >= 4)
		{
			Num_4ms++;
			Count_4ms = 0;
		}
		
		if (Num_1ms % 1000 == 0)
		{			
			OLED_ShowNum(1, 5, Num_1ms / 1000, 5);
		}
		if (Num_2ms % 1000 == 0)
		{
			OLED_ShowNum(2, 5, Num_2ms / 1000, 5);
		}
		if (Num_4ms % 1000 == 0)
		{			
			OLED_ShowNum(3, 5, Num_4ms / 1000, 5);
		}
		*/
	}
}
