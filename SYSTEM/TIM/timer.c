#include "timer.h"

void Tim1_init(uint16_t psc,uint16_t arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitstruct;
	TIM_OCInitTypeDef       TIM_OCInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_TimeBaseInitstruct.TIM_Period    =  arr;    //��װ��ֵ
	TIM_TimeBaseInitstruct.TIM_Prescaler =  psc;   //Ԥ��Ƶֵ
	TIM_TimeBaseInitstruct.TIM_CounterMode= TIM_CounterMode_Up;  //���ϼ���
	TIM_TimeBaseInitstruct.TIM_ClockDivision=TIM_CKD_DIV1; 
	//TIM_TimeBaseInitstruct.TIM_RepetitionCounter=;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitstruct);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;    //PWM1�Ĺ���ģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�����������
	TIM_OCInitStructure.TIM_Pulse = 0;                 //�Ƚ�ֵ0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //��Ч״̬Ϊ�ߵ�ƽ

	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	//TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM1,ENABLE); //ʹ�ܶ�ʱ��1
	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //��ռ���ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;       //��Ӧ���ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
}

void Tim3_init(uint16_t psc,uint16_t arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitstruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitstruct.TIM_Period    =  arr;    //��װ��ֵ
	TIM_TimeBaseInitstruct.TIM_Prescaler =  psc;   //Ԥ��Ƶֵ
	TIM_TimeBaseInitstruct.TIM_CounterMode= TIM_CounterMode_Up;  //���ϼ���
	TIM_TimeBaseInitstruct.TIM_ClockDivision=TIM_CKD_DIV1;   //�������ã�Ӳ��û�з�Ƶ��
	//TIM_TimeBaseInitstruct.TIM_RepetitionCounter=;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitstruct);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,DISABLE);                  //��ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;       //��Ӧ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//void TIM1_UP_TIM10_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET) //����ж�
//	{
//		PFout(9)=!PFout(9);
//		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //����жϱ�־λ
//	}
//}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		if(PEin(2)==0)
			PFout(10)=!PFout(10);
		TIM_Cmd(TIM3,DISABLE);
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
	}
}


