#include "Tasks.h"
#include "OLED.h"
#include "Key.h"

// volatile uint16_t Num_1ms, Num_2ms, Num_4ms;

// 定义用于接收按键键码的变量
uint8_t KeyNum;

int main(void)
{	
	// 板级支持包中的硬件驱动初始化
	BSP_Init();
	// OLED初始化
	OLED_Init();
	// 按键初始化
	Key_Init();
	
	/* 显示静态字符串 */
	OLED_ShowString(1, 1, "Flag:");
	OLED_ShowString(2, 1, "Data:");
	
	while (1)
	{	
		// 获取按键键码
		KeyNum = Key_GetNum();
		
		// 按键1按下
		if (KeyNum == 1)
		{
			// 变换测试数据
			Store_Data[1] ++;
			Store_Data[2] += 2;
			Store_Data[3] += 3;
			Store_Data[4] += 4;
			// 将Store_Data的数据备份保存到闪存，实现掉电不丢失
			Store_Save();
		}
		
		// 按键2按下
		if (KeyNum == 2)
		{
			// 将Store_Data的数据全部清0
			Store_Clear();
		}
		
		// 显示Store_Data的第一位标志位
		OLED_ShowHexNum(1, 6, Store_Data[0], 4);
		// 显示Store_Data的有效存储数据
		OLED_ShowHexNum(3, 1, Store_Data[1], 4);
		OLED_ShowHexNum(3, 6, Store_Data[2], 4);
		OLED_ShowHexNum(4, 1, Store_Data[3], 4);
		OLED_ShowHexNum(4, 6, Store_Data[4], 4);
		
		/**
		if (Count_1ms >= 1)
		{
			Num_1ms++;
			Count_1ms = 0;
			Task_1000HZ();
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
			OLED_ShowNum(2, 5, Num_1ms / 1000, 5);
		}
		if (Num_2ms % 1000 == 0)
		{
			OLED_ShowNum(3, 5, Num_2ms / 1000, 5);
		}
		if (Num_4ms % 1000 == 0)
		{			
			OLED_ShowNum(4, 5, Num_4ms / 1000, 5);
		}
		**/
	}
}
