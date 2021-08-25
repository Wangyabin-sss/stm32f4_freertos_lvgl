#include "hc_sr04.h"

void sr04_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	//打开端口B的硬件时钟，就是供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//打开端口E的硬件时钟，就是供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6; 	//6号引脚
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//高速，速度越高，响应越快，但是功耗会更高
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//不使能上下拉电阻
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6; 	//6号引脚
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//高速，速度越高，响应越快，但是功耗会更高
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//不使能上下拉电阻
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	//只要有输出模式，肯定会有初始电平的状态，看连接设备的说明书
	PBout(6)=0;

}

int32_t sr04_get_distance(void)
{
	uint32_t t=0;
	PBout(6)=1;
	delay_us(15);
	PBout(6)=0;	

	//等待回响信号,若等待成功，PE6引脚为高电平，则跳出该循环
	while(PEin(6)==0);
	
	//测量高电平的持续时间
	while(PEin(6))
	{
		t++;
		if(t>2700)    //测量距离超过
			return -1;
		delay_us(9);		//有多少个9us ，就是有多少个3mm
	}
	//因为超声波的传输时间是发射时间+返回时间，所以需要除以/2
	t=t/2;
	return 3*t;
}





