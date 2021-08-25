#include "spi.h"

void spi1_init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//使能端口硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	//配置PB3~PB5为复用功能模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;        //复用模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//将PB3~PB5连接到SPI1硬件
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5复用为 SPI1
#if 0	
	//配置PB14为输出模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;          //CS片选引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//只要是输出模式，要给予它初始电平状态
	W25QXX_CS=1;

	//初始化SPI1相关参数,模式3
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工通信
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//主机角色
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				//看从机的数据手册，通信数据每次大小为8bit
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						//看从机的数据手册，CPOL=1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;					//看从机的数据手册，CPHA=1 ，目前为模式3
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						//看从机的数据手册，片选引脚由软件代码控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//看从机的数据手册，SPI的时钟=84MHz/8=10.5MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				//看从机的数据手册
	//SPI_InitStructure.SPI_CRCPolynomial = 7;						//这个是用于两个M4进行通信，spi flash不支持
	SPI_Init(SPI1, &SPI_InitStructure);
#else
	//初始化SPI1相关参数,模式0
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工通信
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//主机角色
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				//看从机的数据手册，通信数据每次大小为8bit
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						//看从机的数据手册，CPOL=0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					//看从机的数据手册，CPHA=0 ，目前为模式0
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						//看从机的数据手册，片选引脚由软件代码控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//看从机的数据手册，SPI的时钟=84MHz/16=5.25MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				//看从机的数据手册
	SPI_InitStructure.SPI_CRCPolynomial = 7;						//这个是用于两个M4进行通信，spi flash不支持
	SPI_Init(SPI1, &SPI_InitStructure);
	
 #endif
	//使能SPI1硬件工作
	SPI_Cmd(SPI1, ENABLE);

}

//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
} 

 /*********************************************************
 * 函数名：uint8_t SPI1_ReadWriteByte(uint8_t TxData)
 * 描述  ：向spi1读写一个字节
 * 输入  ：读时：0xff,     写时：cmd
 * 输出  ：读时：8位数据   写时：无
 * 返回  ：无 
 * 调用  ：无
 **********************************************************/
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{		 			 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据
		
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
}


void Soft_Spi_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_6 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
#if SSPI_MODE==0
	SSPI_SCLK=0;
	RC522_CS=1;
#endif
#if SSPI_MODE==3
	SSPI_SCLK=1;
	RC522_CS=1;
#endif
	
}

#if SSPI_MODE==0
uint8_t Soft_Spi_ReadWriteByte(uint8_t data)
{
	int32_t i=0;
	uint8_t d=0;
	for(i=7; i>=0; i--)
	{
		//MSB，最高有效位发送数据
		if(data & (1<<i))   //先准备好数据
			SSPI_MOSI =1;
		else
			SSPI_MOSI =0;
		//设置SCLK引脚为高电平,主机采集MISO引脚电平
		SSPI_SCLK=1;
		SSPI_delay();
 		//主机采集MISO引脚电平
		if(SSPI_MISO)
			d|=1<<i; 
		//设置SCLK引脚为低电平，从机读取MOSI引脚电平
		SSPI_SCLK=0;
		SSPI_delay();
	}
	return d;
}
#endif


#if SSPI_MODE==3
uint8_t Soft_Spi_ReadWriteByte(uint8_t data)
{
	int i=0;
	uint8_t d=0;
	for(i=0;i<8;i++)
	{
		d=d<<1;
		if(data&0x80)
			SSPI_MOSI=1;
		else
			SSPI_MOSI=0;
		data=data<<1;
		SSPI_SCLK=0;  //设置SCLK引脚为低电平，这个时候，从机读取MOSI引脚电平
		SSPI_delay();
		SSPI_SCLK=1;
		SSPI_delay();
		if(SSPI_MISO)  //主机采集MISO引脚电平
			d++;
	}
	return d;
}
#endif




