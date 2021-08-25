#include "keyboard.h"

void key_board_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	//使能GPIO外设时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);
	//定义一个结构体
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;           //连接LED的引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    //50MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		 //推挽
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;  
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;  
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;  
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    //50MHz
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;		
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;  
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;  
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;  
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}


uint32_t key_board_scan(void)
{
	line1=0;
	line2=1;
	line3=1;
	line4=1;
	delay_ms(2);
	if(line5==0) return 1;
	else if(line6==0) return 2;
	else if(line7==0) return 3;
	else if(line8==0) return 4;
	
	line1=1;
	line2=0;
	line3=1;
	line4=1;
	delay_ms(2);
	if(line5==0) return 5;
	else if(line6==0) return 6;
	else if(line7==0) return 7;
	else if(line8==0) return 8;
	
	line1=1;
	line2=1;
	line3=0;
	line4=1;
	delay_ms(2);
	if(line5==0) return 9;
	else if(line6==0) return 10;
	else if(line7==0) return 11;
	else if(line8==0) return 12;
	
	line1=1;
	line2=1;
	line3=1;
	line4=0;
	delay_ms(2);
	if(line5==0) return 13;
	else if(line6==0) return 14;
	else if(line7==0) return 15;
	else if(line8==0) return 16;
	
	return 0;
}



















