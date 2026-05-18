#include "Struct_All.h"
#include "Tasks.h"
#include "OLED.h"

volatile uint16_t Num_1ms, Num_2ms, Num_4ms;

int main(void)
{	
	// OLED初始化
	OLED_Init();
	// 板级支持包中的硬件驱动初始化
	BSP_Init();
	
	// 显示字符串
	OLED_ShowString(1, 1, "1ms:");
	OLED_ShowString(2, 1, "2ms:");
	OLED_ShowString(3, 1, "4ms:");
	
	while (1)
	{
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
	}
}
