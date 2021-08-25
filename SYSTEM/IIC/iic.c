#include "iic.h"

uint32_t IIC_DT=5;

void iic_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;		//第8 9根引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				//设置输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;				//开漏模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//不需要上拉电阻
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SCL_W=1;
	SDA_W=1;
}

static void sda_mode(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		    //第9根引脚
	GPIO_InitStructure.GPIO_Mode = mode;				//设置模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//开漏模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//不需要上拉电阻
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void iic_start(void)
{
	sda_mode(GPIO_Mode_OUT);
	SCL_W=1;
	SDA_W=1;
	IIC_delay(IIC_DT);
	SDA_W=0;
	IIC_delay(IIC_DT);
	SCL_W=0;
	IIC_delay(IIC_DT);
}

void iic_stop(void)
{
	sda_mode(GPIO_Mode_OUT);
	SCL_W=1;
	SDA_W=0;
	IIC_delay(IIC_DT);
	SDA_W=1;
	IIC_delay(IIC_DT);
}

void iic_send_byte(uint8_t data)
{
	int32_t i;
	sda_mode(GPIO_Mode_OUT);
	SCL_W=0;
	SDA_W=0;
	IIC_delay(IIC_DT);
	for(i=7;i>=0;i--)
	{
		if(data&(1<<i))
			SDA_W=1;
		else
			SDA_W=0;
		IIC_delay(IIC_DT);
		SCL_W=1;
		IIC_delay(IIC_DT);
		SCL_W=0;
		IIC_delay(IIC_DT);
	}
}

uint8_t iic_recv_byte(void)
{
	uint8_t data=0;
	int32_t i;
	sda_mode(GPIO_Mode_IN);
	
	for(i=7;i>=0;i--)
	{
		SCL_W=1;
		IIC_delay(IIC_DT);
		if(SDA_R)
			data|=1<<i;
		SCL_W=0;
		IIC_delay(IIC_DT);
	}
	return data;
}

uint8_t iic_wait_ack(void)
{
	uint8_t ack;
	sda_mode(GPIO_Mode_IN);
	SCL_W=1;
	IIC_delay(IIC_DT);
	if(SDA_R)
		ack=1;
	else
		ack=0;
	SCL_W=0;
	IIC_delay(IIC_DT);
	return ack;
}

void iic_ack(uint8_t ack)
{
	sda_mode(GPIO_Mode_OUT);
	SDA_W=0;
	SCL_W=0;
	IIC_delay(IIC_DT);
	if(ack)
		SDA_W=1;
	else 
		SDA_W=0;
	IIC_delay(IIC_DT);
	SCL_W=1;
	IIC_delay(IIC_DT);
	SCL_W=0;
	IIC_delay(IIC_DT);
}











