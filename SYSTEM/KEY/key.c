#include "main.h"

void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	//�򿪶˿�A��Ӳ��ʱ�ӣ����ǹ���
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;	//0������ ����1
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;  //����ģʽ
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//���������������Ҳ������Ч����Ϊ�ڲ��ṹ�Ѿ��ǹر��ˣ�
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;//���٣��ٶ�Խ�ߣ���ӦԽ�죬���ǹ��Ļ����
	//������ȥ��һ��������̣���Ҫʹ���ڲ���������
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��ʹ������������(��Ϊ�ⲿ�Ѿ�����������)
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; //����2 3 4������
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;        //����ģʽ
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;    //25MHz
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;		 //����
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line0|EXTI_Line2|EXTI_Line3|EXTI_Line4;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;  //����Ϊ�����ش���
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;							//�ⲿ�ж�0���жϺ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x5;						//��Ӧ���ȼ� 0x05
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//NVICʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;							//�ⲿ�ж�2���жϺ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x5;						//��Ӧ���ȼ� 0x05
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//NVICʹ��
	NVIC_Init(&NVIC_InitStructure);		
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;							//�ⲿ�ж�3���жϺ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x5;						//��Ӧ���ȼ� 0x05
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//NVICʹ��
	NVIC_Init(&NVIC_InitStructure);		
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;							//�ⲿ�ж�3���жϺ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x5;						//��Ӧ���ȼ� 0x05
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//NVICʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
}


void EXTI0_IRQHandler(void)
{
	uint32_t ulReturn;
	
	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		xEventGroupSetBitsFromISR(event_group,EVENT_GROUP_KEY1_DOWN,NULL);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}

void EXTI2_IRQHandler(void)
{
	uint32_t ulReturn;
	
	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(EXTI_Line2)==SET)
	{
		xEventGroupSetBitsFromISR(event_group,EVENT_GROUP_KEY2_DOWN,NULL);
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}

void EXTI3_IRQHandler(void)
{
	uint32_t ulReturn;
	
	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(EXTI_Line3)==SET)
	{
		xEventGroupSetBitsFromISR(event_group,EVENT_GROUP_KEY3_DOWN,NULL);
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}
void EXTI4_IRQHandler(void)
{
	uint32_t ulReturn;
	
	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(EXTI_GetITStatus(EXTI_Line4)==SET)
	{
		xEventGroupSetBitsFromISR(event_group,EVENT_GROUP_KEY4_DOWN,NULL);
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}


