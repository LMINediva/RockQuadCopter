#include "Tasks.h"
#include "OLED.h"
#include "Key.h"
#include "Delay.h"

// volatile uint16_t Num_1ms, Num_2ms, Num_4ms;

uint8_t KeyNum;

// 发送成功计次，发送失败计次
uint8_t SendSuccessCount, SendFailedCount;
// 接收成功计次，接收失败计次
uint8_t ReceiveSuccessCount, ReceiveFailedCount;

int main(void)
{	
	// OLED初始化
	OLED_Init();
	// 板级支持包中的硬件驱动初始化
	BSP_Init();
	Key_Init();
	
	/* 显示静态字符串 */
	// 格式为：T:发送成功计次-发送失败计次-发送标志位
	OLED_ShowString(1, 1, "T:000-000-0");
	// 格式为：R:接收成功计次-接收失败计次-接收标志位
	OLED_ShowString(3, 1, "R:000-000-0");
	
	/* 初始化测试数据，此处值为任意设定，便于观察实验现象 */
	NRF24L01_TxPacket[0] = 0x00;
	NRF24L01_TxPacket[1] = 0x01;
	NRF24L01_TxPacket[2] = 0x02;
	NRF24L01_TxPacket[3] = 0x03;
	
	while (1)
	{	
		// 读取按键，获取键码
		KeyNum = Key_GetNum();
		
		// 按键按下
		if (KeyNum == 1)
		{
			/* 变换测试数据，便于观察实验现象 */
			// 实际项目中，可以将待发送的数据赋值给NRF24L01_TxPacket数组
			NRF24L01_TxPacket[0]++;
			NRF24L01_TxPacket[1]++;
			NRF24L01_TxPacket[2]++;
			NRF24L01_TxPacket[3]++;
			
			/* 调用NRF24L01_Send函数，发送数据，
			同时置发送标志位，方便用户了解发送状态 */
			// 发送标志位与发送状态的对应关系，可以转到此函数定义上方查看
			NRF24L01_Send();
			Delay_ms(10);
			// 判断发送标志位
			if (NRF24L01_SendFlag == 1)
			{
				// 发送标志位为1，表示发送成功
				// 发送成功计次变量自增
				SendSuccessCount++;
			}
			else
			{
				// 发送标志位不为1，即2/3/4，表示发送不成功
				// 发送失败计次变量自增
				SendFailedCount++;
			}
			
			// 显示发送成功次数
			OLED_ShowNum(1, 3, SendSuccessCount, 3);
			// 显示发送失败次数
			OLED_ShowNum(1, 7, SendFailedCount, 3);
			// 显示最近一次的发送标志位
			OLED_ShowNum(1, 11, NRF24L01_SendFlag, 1);
			
			/* 显示发送数据 */
			OLED_ShowHexNum(2, 1, NRF24L01_TxPacket[0], 2);
			OLED_ShowHexNum(2, 4, NRF24L01_TxPacket[1], 2);
			OLED_ShowHexNum(2, 7, NRF24L01_TxPacket[2], 2);
			OLED_ShowHexNum(2, 10, NRF24L01_TxPacket[3], 2);
			
			/* TX字符串闪烁一次，表明发送了一次数据 */
			OLED_ShowString(1, 15, "TX");
			Delay_ms(100);
			OLED_ShowString(1, 15, "  ");
		}
		
		/* 主循环内循环执行NRF24L01_Receive函数，接收数据，
		同时返回接收标志位，方便用户了解接收状态*/
		// 接收标志位与接收状态的对应关系，可以转到此函数定义上方查看
		// 判断接收标志位
		if (NRF24L01_ReceiveFlag)
		{
			// 接收标志位不为0，表示收到了一个数据包
			if (NRF24L01_ReceiveFlag == 1)
			{
				// 接收标志位为1，表示接收成功
				// 接收成功计次变量自增
				ReceiveSuccessCount++;
			}
			else
			{
				// 接收标志位不为0也不为1，即2/3，表示此次接收产生了错误，
				// 错误接收的数据不应该使用
				// 接收失败计次变量自增
				ReceiveFailedCount++;
			}
			// 显示接收成功次数
			OLED_ShowNum(3, 3, ReceiveSuccessCount, 3);
			// 显示接收失败次数
			OLED_ShowNum(3, 7, ReceiveFailedCount, 3);
			// 显示最近一次的接收标志位
			OLED_ShowNum(3, 11, NRF24L01_ReceiveFlag, 1);
			
			/* 显示接收数据 */
			OLED_ShowHexNum(4, 1, NRF24L01_RxPacket[0], 2);
			OLED_ShowHexNum(4, 4, NRF24L01_RxPacket[1], 2);
			OLED_ShowHexNum(4, 7, NRF24L01_RxPacket[2], 2);
			OLED_ShowHexNum(4, 10, NRF24L01_RxPacket[3], 2);
			
			// RX字符串闪烁一次，表明接收到了一次数据
			OLED_ShowString(3, 15, "RX");
			Delay_ms(100);
			OLED_ShowString(3, 15, "  ");
			
			// 接收标志位置0，即恢复默认值
			NRF24L01_ReceiveFlag = 0;
		}
		
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
