#include "main.h"

void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	//打开端口A的硬件时钟，就是供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;	//0号引脚 按键1
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;  //输入模式
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出（配置了也不会生效，因为内部结构已经是关闭了）
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;//高速，速度越高，响应越快，但是功耗会更高
	//当你们去做一个矩阵键盘，需要使能内部上拉电阻
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//不使能上下拉电阻(因为外部已经有上拉电阻)
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; //按键2 3 4的引脚
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;        //输入模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;    //25MHz
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;		 //推挽
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line0|EXTI_Line2|EXTI_Line3|EXTI_Line4;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;  //配置为下升沿触发
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;							//外部中断0的中断号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x5;						//响应优先级 0x05
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//NVIC使能
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;							//外部中断2的中断号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x5;						//响应优先级 0x05
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//NVIC使能
	NVIC_Init(&NVIC_InitStructure);		
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;							//外部中断3的中断号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x5;						//响应优先级 0x05
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//NVIC使能
	NVIC_Init(&NVIC_InitStructure);		
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;							//外部中断3的中断号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x5;						//响应优先级 0x05
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//NVIC使能
	NVIC_Init(&NVIC_InitStructure);	
	
}


void EXTI0_IRQHandler(void)
{
	uint32_t ulReturn;
	
	/* 进入临界段，临界段可以嵌套 */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		xEventGroupSetBitsFromISR(event_group,EVENT_GROUP_KEY1_DOWN,NULL);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}

void EXTI2_IRQHandler(void)
{
	uint32_t ulReturn;
	
	/* 进入临界段，临界段可以嵌套 */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(EXTI_Line2)==SET)
	{
		xEventGroupSetBitsFromISR(event_group,EVENT_GROUP_KEY2_DOWN,NULL);
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}

void EXTI3_IRQHandler(void)
{
	uint32_t ulReturn;
	
	/* 进入临界段，临界段可以嵌套 */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(EXTI_Line3)==SET)
	{
		xEventGroupSetBitsFromISR(event_group,EVENT_GROUP_KEY3_DOWN,NULL);
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}
void EXTI4_IRQHandler(void)
{
	uint32_t ulReturn;
	
	/* 进入临界段，临界段可以嵌套 */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(EXTI_Line4)==SET)
	{
		xEventGroupSetBitsFromISR(event_group,EVENT_GROUP_KEY4_DOWN,NULL);
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}


