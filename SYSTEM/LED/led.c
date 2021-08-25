#include "led.h"


void led_init(void)
{
	//声明一个结构体变量
	GPIO_InitTypeDef GPIO_InitStructure;	
	//使能GPIO外设时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);
	//定义一个结构体
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;           //连接LED的引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    //50MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		 //推挽
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOF,GPIO_Pin_9);
	
//	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10; //连接LED的引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_25MHz;    //25MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		 //推挽
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOF,GPIO_Pin_10);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;         //连接BEEP的引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_25MHz;    //25MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		 //推挽
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_8);
	
	//这两个引脚与FSMC复用冲突
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13 | GPIO_Pin_14; //连接LED的引脚
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;         //输出模式
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;     //50MHz
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		 //推挽
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 //下拉
//	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
//	
//	GPIOF->MODER &= 0xfff3ffff;
//	GPIOF->MODER |= 0x00080000;   //输出模式
//	
//	GPIOF->OTYPER &= 0xfffffdff;  //推挽输出
//	
//	GPIOF->OSPEEDR &= 0xfff3ffff;
//	GPIOF->OSPEEDR &= 0xfff8ffff; //25Mhz
//	
//	GPIOF->PUPDR &= 0xfff3ffff;  //无上下拉
//	
//	GPIOF->ODR &= 0xfffffdff;    //9脚输出0
	
}







