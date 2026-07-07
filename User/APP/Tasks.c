#include "Tasks.h"

// 板级支持包中的硬件驱动是否初始化完成标志
uint8_t BSP_Init_OK = 0;

/**
 * 函数：NVIC初始化
 * 参数：无
 * 返回值：无
 */
void My_NVIC_Init(void)
{
	// NVIC中断分组，配置NVIC为分组2
	// 即抢占优先级范围：0~3，响应优先级范围：0~3
	// 此分组配置在整个工程中仅需调用一次
	// 若有多个中断，可以把此代码放在main函数内，while循环之前
	// 若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	// NVIC配置
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// Timer3，选择配置NVIC的TIM3线
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	// 指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// 指定NVIC线路的抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// 指定NVIC线路的响应优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// 将结构体变量交给NVIC_Init，配置NVIC外设
	NVIC_Init(&NVIC_InitStructure);
	
	// NRF24L01中断，选择配置NVIC的EXTI0线
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	// 指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// 指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// 指定NVIC线路的响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	// 将结构体变量交给NVIC_Init，配置NVIC外设
	NVIC_Init(&NVIC_InitStructure);
	
	// 串口，选择配置NVIC的USART1线
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	// 指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// 指定NVIC线路的抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	// 指定NVIC线路的响应优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	// 将结构体变量交给NVIC_Init，配置NVIC外设
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * 函数：板级支持包中的硬件驱动初始化
 * 参数：无
 * 返回值：无
 */
void BSP_Init(void)
{	
	// LED初始化
	LED_Init();
	// LED闪烁
	LED_ON_OFF();
	// 串口初始化，波特率：115200，8位数据，1位停止位，禁用奇偶校验
	Uart1_Init(115200);
	// Timer3初始化，频率为：1KHZ
	Timer3_Init(1000);
	// NVIC初始化
	My_NVIC_Init();
	// PWM初始化
	Motor_Init();
	// ADC及DMA初始化
	ADC1_Init();
	// NRF24L01初始化
	NRF24L01_Init();
	// 如果MPU6050初始化不成功，则程序不向下运行
	while (MPU6050_Init() != 1);
	
	BSP_Init_OK = 1;
}

/**
 * 函数：主循环中运行频率为1000HZ的任务
 * 参数：无
 * 返回值：无
 */
void Task_1000HZ(void)
{
	// 如果连续读取MPU6050的数据寄存器失败
	if (MPU6050_SequenceRead() == 1)
	{
		// 统计I2C出错的次数
		I2C_Error++;
		// 分次读取MPU6050的数据寄存器
		MPU6050_SingleRead();
	}
	// 合成MPU6050的16位数据
	MPU6050_Compose();
}
