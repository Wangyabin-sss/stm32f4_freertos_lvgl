#include "spi.h"

void spi1_init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʹ�ܶ˿�Ӳ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	//����PB3~PB5Ϊ���ù���ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;        //����ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//��PB3~PB5���ӵ�SPI1Ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4����Ϊ SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5����Ϊ SPI1
#if 0	
	//����PB14Ϊ���ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;          //CSƬѡ����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //���ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//ֻҪ�����ģʽ��Ҫ��������ʼ��ƽ״̬
	W25QXX_CS=1;

	//��ʼ��SPI1��ز���,ģʽ3
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //ȫ˫��ͨ��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//������ɫ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				//���ӻ��������ֲᣬͨ������ÿ�δ�СΪ8bit
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						//���ӻ��������ֲᣬCPOL=1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;					//���ӻ��������ֲᣬCPHA=1 ��ĿǰΪģʽ3
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						//���ӻ��������ֲᣬƬѡ����������������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//���ӻ��������ֲᣬSPI��ʱ��=84MHz/8=10.5MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				//���ӻ��������ֲ�
	//SPI_InitStructure.SPI_CRCPolynomial = 7;						//�������������M4����ͨ�ţ�spi flash��֧��
	SPI_Init(SPI1, &SPI_InitStructure);
#else
	//��ʼ��SPI1��ز���,ģʽ0
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //ȫ˫��ͨ��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//������ɫ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				//���ӻ��������ֲᣬͨ������ÿ�δ�СΪ8bit
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						//���ӻ��������ֲᣬCPOL=0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					//���ӻ��������ֲᣬCPHA=0 ��ĿǰΪģʽ0
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						//���ӻ��������ֲᣬƬѡ����������������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//���ӻ��������ֲᣬSPI��ʱ��=84MHz/16=5.25MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				//���ӻ��������ֲ�
	SPI_InitStructure.SPI_CRCPolynomial = 7;						//�������������M4����ͨ�ţ�spi flash��֧��
	SPI_Init(SPI1, &SPI_InitStructure);
	
 #endif
	//ʹ��SPI1Ӳ������
	SPI_Cmd(SPI1, ENABLE);

}

//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
} 

 /*********************************************************
 * ��������uint8_t SPI1_ReadWriteByte(uint8_t TxData)
 * ����  ����spi1��дһ���ֽ�
 * ����  ����ʱ��0xff,     дʱ��cmd
 * ���  ����ʱ��8λ����   дʱ����
 * ����  ���� 
 * ����  ����
 **********************************************************/
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{		 			 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ��byte  ����
		
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
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
		//MSB�������Чλ��������
		if(data & (1<<i))   //��׼��������
			SSPI_MOSI =1;
		else
			SSPI_MOSI =0;
		//����SCLK����Ϊ�ߵ�ƽ,�����ɼ�MISO���ŵ�ƽ
		SSPI_SCLK=1;
		SSPI_delay();
 		//�����ɼ�MISO���ŵ�ƽ
		if(SSPI_MISO)
			d|=1<<i; 
		//����SCLK����Ϊ�͵�ƽ���ӻ���ȡMOSI���ŵ�ƽ
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
		SSPI_SCLK=0;  //����SCLK����Ϊ�͵�ƽ�����ʱ�򣬴ӻ���ȡMOSI���ŵ�ƽ
		SSPI_delay();
		SSPI_SCLK=1;
		SSPI_delay();
		if(SSPI_MISO)  //�����ɼ�MISO���ŵ�ƽ
			d++;
	}
	return d;
}
#endif




