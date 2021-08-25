#include "timer.h"

void Tim1_init(uint16_t psc,uint16_t arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitstruct;
	TIM_OCInitTypeDef       TIM_OCInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_TimeBaseInitstruct.TIM_Period    =  arr;    //重装载值
	TIM_TimeBaseInitstruct.TIM_Prescaler =  psc;   //预分频值
	TIM_TimeBaseInitstruct.TIM_CounterMode= TIM_CounterMode_Up;  //向上计数
	TIM_TimeBaseInitstruct.TIM_ClockDivision=TIM_CKD_DIV1; 
	//TIM_TimeBaseInitstruct.TIM_RepetitionCounter=;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitstruct);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;    //PWM1的工作模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //允许输出脉冲
	TIM_OCInitStructure.TIM_Pulse = 0;                 //比较值0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //有效状态为高电平

	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	//TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM1,ENABLE); //使能定时器1
	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //抢占优先级2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;       //响应优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
}

void Tim3_init(uint16_t psc,uint16_t arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitstruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitstruct.TIM_Period    =  arr;    //重装载值
	TIM_TimeBaseInitstruct.TIM_Prescaler =  psc;   //预分频值
	TIM_TimeBaseInitstruct.TIM_CounterMode= TIM_CounterMode_Up;  //向上计数
	TIM_TimeBaseInitstruct.TIM_ClockDivision=TIM_CKD_DIV1;   //不起作用，硬件没有分频器
	//TIM_TimeBaseInitstruct.TIM_RepetitionCounter=;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitstruct);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,DISABLE);                  //不使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;       //响应优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//void TIM1_UP_TIM10_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //溢出中断
//	{
//		PFout(9)=!PFout(9);
//		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清除中断标志位
//	}
//}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		if(PEin(2)==0)
			PFout(10)=!PFout(10);
		TIM_Cmd(TIM3,DISABLE);
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
	}
}


