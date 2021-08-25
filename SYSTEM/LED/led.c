#include "led.h"


void led_init(void)
{
	//����һ���ṹ�����
	GPIO_InitTypeDef GPIO_InitStructure;	
	//ʹ��GPIO����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);
	//����һ���ṹ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;           //����LED������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //���ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    //50MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		 //����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOF,GPIO_Pin_9);
	
//	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10; //����LED������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //���ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_25MHz;    //25MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		 //����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOF,GPIO_Pin_10);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;         //����BEEP������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //���ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_25MHz;    //25MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		 //����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
	
	//������������FSMC���ó�ͻ
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13 | GPIO_Pin_14; //����LED������
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;         //���ģʽ
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;     //50MHz
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		 //����
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 //����
//	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
//	
//	GPIOF->MODER &= 0xfff3ffff;
//	GPIOF->MODER |= 0x00080000;   //���ģʽ
//	
//	GPIOF->OTYPER &= 0xfffffdff;  //�������
//	
//	GPIOF->OSPEEDR &= 0xfff3ffff;
//	GPIOF->OSPEEDR &= 0xfff8ffff; //25Mhz
//	
//	GPIOF->PUPDR &= 0xfff3ffff;  //��������
//	
//	GPIOF->ODR &= 0xfffffdff;    //9�����0
	
}







