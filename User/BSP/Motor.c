#include "Motor.h"

/**
 * 函数：定时器2输出比较初始化
 * 参数：无
 * 返回值：无
 */
static void Timer2_Init(void)
{
	// 开启TIM2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/* 配置时钟源 */
	// 选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟
	TIM_InternalClockConfig(TIM2);
	
	/* 时基单元初始化 */
	// PWM频率 = 72000000 / 4 / 1000 = 18KHZ
	// 定义结构体变量
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	// 时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	// 计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// 计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;
	// 预分频器，即PSC的值，PWM输出18KHZ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 4 - 1;
	// 重复计数器，高级定时器才会用到
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	// 配置TIM2的时基单元
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	/* 输出比较初始化 */
	// 定义结构体变量
	TIM_OCInitTypeDef TIM_OCInitStructure;
	// 结构体初始化，若结构体没有完整赋值
	// 则最好执行此函数，给结构体所有成员都赋一个默认值
	// 避免结构体初值不确定的问题
	TIM_OCStructInit(&TIM_OCInitStructure);
	// 输出比较模式，选择PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出极性，选择为高，若选择极性为低，则输出高低电平取反
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 初始的CCR值
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	// 初始化定时器2的通道1
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	// 使能TIM2在CCR1上的预装载寄存器
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	// 初始化定时器2的通道2
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	// 使能TIM2在CCR2上的预装载寄存器
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	// 初始化定时器2的通道3
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	// 使能TIM2在CCR3上的预装载寄存器
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	// 初始化定时器2的通道4
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	// 使能TIM2在CCR4上的预装载寄存器
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	/* TIM使能 */
	// 使能TIM2在ARR上的预装载寄存器
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	// 使能TIM2，定时器开始运行
	TIM_Cmd(TIM2, ENABLE);
}

/**
 * 电机分布：
 * Motor1	Motor2
 * Motor4	Motor3
 * 函数：PWM初始化
 * 参数：无
 * 返回值：无
 */
void Motor_Init(void)
{
	/* 开启时钟 */
	// 开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/* GPIO初始化 */
	GPIO_InitTypeDef GPIO_InitStructure;
	// 复用推挽输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	// Motor1：电机1，Motor2：电机2，Motor3：电机3，Motor4：电机4
	GPIO_InitStructure.GPIO_Pin = Motor1_Pin | Motor2_Pin | Motor3_Pin | Motor4_Pin;
	// 输出速度为50MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO初始化
	GPIO_Init(Motor_Port, &GPIO_InitStructure);
	
	// 定时器2输出比较初始化
	Timer2_Init();
}

/**
 * 函数：电机驱动
 * 参数：duty1~4：电机1~4要写入CCR的值，范围：0~1000
 * 返回值：无
 * 注意事项：CCR和ARR共同决定占空比，此函数仅设置CCR的值，并不直接是占空比
 * 占空比Duty = CCR / (ARR + 1)
 */
void Motor_Out(int16_t duty1, int16_t duty2, int16_t duty3, int16_t duty4)
{
	// 限制CCR的值的范围为0~1000
	/*
	if (duty1 > 1000)
	{
		duty1 = 1000;
	}
	if (duty1 < 0)
	{
		duty1 = 0;
	}
	if (duty2 > 1000)
	{
		duty2 = 1000;
	}
	if (duty2 < 0)
	{
		duty2 = 0;
	}
	if (duty3 > 1000)
	{
		duty3 = 1000;
	}
	if (duty3 < 0)
	{
		duty3 = 0;
	}
	if (duty4 > 1000)
	{
		duty4 = 1000;
	}
	if (duty4 < 0)
	{
		duty4 = 0;
	}
	*/
	
	if (duty1 > 0)
	{		
		// 设置CCR1~4的值
		TIM_SetCompare1(TIM2, duty1);
		TIM_SetCompare2(TIM2, duty2);
		TIM_SetCompare3(TIM2, duty3);
		TIM_SetCompare4(TIM2, duty4);
	}
	else
	{
		// 设置CCR1~4的值
		TIM_SetCompare1(TIM2, -duty1);
		TIM_SetCompare2(TIM2, -duty2);
		TIM_SetCompare3(TIM2, -duty3);
		TIM_SetCompare4(TIM2, -duty4);
	}
}
