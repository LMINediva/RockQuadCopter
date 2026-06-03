#include "Struct_All.h"
#include "Tasks.h"
#include "OLED.h"
#include "Delay.h"

// volatile uint16_t Num_1ms, Num_2ms, Num_4ms;

int main(void)
{	
	// OLED初始化
	OLED_Init();
	// 板级支持包中的硬件驱动初始化
	BSP_Init();
	
	/* 显示静态字符串 */
	OLED_ShowString(1, 1, "Battery:");
	OLED_ShowString(2, 1, "Vref:");
	OLED_ShowString(3, 1, "Result:");
	
	while (1)
	{	
		// 3.7V锂电池电压
		OLED_ShowNum(1, 9, ADC_Value[0], 4);
		// 内部参考电压（1.2V）
		OLED_ShowNum(2, 6, ADC_Value[1], 4);
		// 计算电池电压的100倍
		Voltage_Printf();
		// 结果：(uint16_t)(2.0f * ADC_Value[0] / ADC_Value[1] * 1.2f * 100);
		// 显示电压值的整数部分
		OLED_ShowNum(3, 8, Battery, 4);
		
		// 延时100ms，手动增加一些转换的间隔时间
		Delay_ms(100);
		
		/**
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
