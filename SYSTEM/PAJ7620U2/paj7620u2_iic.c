#include "paj7620u2_iic.h"
#include "paj7620u2.h"


//PAJ2670 I2C��ʼ��
void GS_i2c_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//�˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
    GPIO_Init(GPIOE, &GPIO_InitStructure); //��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//�˿�����
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��

	GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);//PB10,PB11 �����	
	
}

//����IIC��ʼ�ź�
static void GS_IIC_Start(void)
{
	GS_SDA_OUT();//sda�����
	GS_IIC_SDA=1;	  	  
	GS_IIC_SCL=1;
	delay_us(4);
 	GS_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	GS_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

//����IICֹͣ�ź�
static void GS_IIC_Stop(void)
{
	GS_SDA_OUT();//sda�����
	GS_IIC_SCL=0;
	GS_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	GS_IIC_SCL=1; 
	GS_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
static uint8_t GS_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	GS_SDA_IN();  //SDA����Ϊ����  
	GS_IIC_SDA=1;delay_us(3);	   
	GS_IIC_SCL=1;delay_us(3);	 
	while(GS_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			GS_IIC_Stop();
			return 1;
		}
	}
	GS_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
}

//����ACKӦ��
static void GS_IIC_Ack(void)
{
	GS_IIC_SCL=0;
	GS_SDA_OUT();
	GS_IIC_SDA=0;
	delay_us(3);
	GS_IIC_SCL=1;
	delay_us(3);
	GS_IIC_SCL=0;
}

//������ACKӦ��		    
static void GS_IIC_NAck(void)
{
	GS_IIC_SCL=0;
	GS_SDA_OUT();
	GS_IIC_SDA=1;
	delay_us(2);
	GS_IIC_SCL=1;
	delay_us(2);
	GS_IIC_SCL=0;
}

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
static void GS_IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	GS_SDA_OUT(); 	    
    GS_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			GS_IIC_SDA=1;
		else
			GS_IIC_SDA=0;
		txd<<=1; 	  
		delay_us(5);  
		GS_IIC_SCL=1;
		delay_us(5); 
		GS_IIC_SCL=0;	
		delay_us(5);
    }	 
} 

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
static uint8_t GS_IIC_Read_Byte(uint8_t ack)
{
	uint8_t i,receive=0;
	GS_SDA_IN();//SDA����Ϊ����
	for(i=0;i<8;i++ )
	{
		GS_IIC_SCL=0; 
		delay_us(4);
	  GS_IIC_SCL=1;
		receive<<=1;
		if(GS_READ_SDA)receive++;   
	  delay_us(4); 
	}					 
	if (!ack)
		GS_IIC_NAck();//����nACK
	else
		GS_IIC_Ack(); //����ACK   
	return receive;
}

//PAJ7620U2дһ���ֽ�����
uint8_t GS_Write_Byte(uint8_t REG_Address,uint8_t REG_data)
{
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);
	if(GS_IIC_Wait_Ack())
	{
		GS_IIC_Stop();//�ͷ�����
		return 1;//ûӦ�����˳�

	}
	GS_IIC_Send_Byte(REG_Address);
	GS_IIC_Wait_Ack();	
	GS_IIC_Send_Byte(REG_data);
	GS_IIC_Wait_Ack();	
	GS_IIC_Stop();

	return 0;
}

//PAJ7620U2��һ���ֽ�����
uint8_t GS_Read_Byte(uint8_t REG_Address)
{
	uint8_t REG_data;
	
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);//��д����
	if(GS_IIC_Wait_Ack())
	{
		 GS_IIC_Stop();//�ͷ�����
		 return 0;//ûӦ�����˳�
	}		
	GS_IIC_Send_Byte(REG_Address);
	GS_IIC_Wait_Ack();
	GS_IIC_Start(); 
	GS_IIC_Send_Byte(PAJ7620_ID|0x01);//��������
	GS_IIC_Wait_Ack();
	REG_data = GS_IIC_Read_Byte(0);
	GS_IIC_Stop();

	return REG_data;
}
//PAJ7620U2��n���ֽ�����
uint8_t GS_Read_nByte(uint8_t REG_Address,uint16_t len,uint8_t *buf)
{
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);//��д����
	if(GS_IIC_Wait_Ack()) 
	{
		GS_IIC_Stop();//�ͷ�����
		return 1;//ûӦ�����˳�
	}
	GS_IIC_Send_Byte(REG_Address);
	GS_IIC_Wait_Ack();

	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID|0x01);//��������
	GS_IIC_Wait_Ack();
	while(len)
	{
		if(len==1)
		{
			*buf = GS_IIC_Read_Byte(0);
		}
		else
		{
			*buf = GS_IIC_Read_Byte(1);
		}
		buf++;
		len--;
	}
	GS_IIC_Stop();//�ͷ�����

	return 0;
	
}
//PAJ7620����
void GS_WakeUp(void)
{
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);//��д����
	GS_IIC_Stop();//�ͷ�����
}


