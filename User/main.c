#include "Struct_All.h"
#include "Tasks.h"
#include "OLED.h"

// volatile uint16_t Num_1ms, Num_2ms, Num_4ms;

// 定义用于存放ID号的变量
uint8_t ID;

int main(void)
{	
	// OLED初始化
	OLED_Init();
	// 板级支持包中的硬件驱动初始化
	BSP_Init();
	
	/* 显示静态字符串 */
	OLED_ShowString(1, 1, "ID:");
	// 获取MPU6050的ID号
	ID = MPU6050_GetID();
	// OLED显示ID号
	OLED_ShowHexNum(1, 4, ID, 2);
	
	while (1)
	{	
		// 连续读取MPU6050数据寄存器
		MPU6050_SequenceRead();
		// 合成MPU6050的16位数据
		MPU6050_Compose();
		
		// OLED显示数据
		// 加速度计X、Y和Z轴的16位数据
		OLED_ShowSignedNum(2, 1, acc.x, 5);
		OLED_ShowSignedNum(3, 1, acc.y, 5);
		OLED_ShowSignedNum(4, 1, acc.z, 5);
		
		// 陀螺仪X、Y和Z轴的16位数据
		OLED_ShowSignedNum(2, 8, gyro.x, 5);
		OLED_ShowSignedNum(3, 8, gyro.y, 5);
		OLED_ShowSignedNum(4, 8, gyro.z, 5);
		
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
