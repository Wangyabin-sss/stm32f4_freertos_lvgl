#include "dht11.h"

void dht11_init(void)
{
	//����һ���ṹ�����
	GPIO_InitTypeDef GPIO_InitStructure;	
	//ʹ��GPIO����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	//����һ���ṹ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;           //����LED������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //���ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    //50MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;	   //��©
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	PGout(9)=1;
}

static int8_t dht11_bit(void)
{
	uint8_t tim=0;
	while(PGin(9)==1) //�ȴ���Ϊ�͵�ƽ
	{
		tim++;
		if(tim>100)
			return -1;
		delay_us(1);
	}
	tim=0;
	while(PGin(9)==0) //�ȴ���Ϊ�ߵ�ƽ
	{
		tim++;
		if(tim>100)
			return -1;
		delay_us(1);
	}
	delay_us(40);
	if(PGin(9)==1) 
		return 1;
	else 
		return 0;
	
}

//�¶�ʪ�� ����data[5] printf("T=%d.%d,H=%d.%d\n",data[2],data[3],data[0],data[1]);
int8_t dht_getdata(uint8_t *data) 
{
	int8_t i=0;
	uint8_t dat8=0;
	DHT11Pin_OUT();
	PGout(9)=0;    //��ʼ�ź�
	delay_ms(18);
	PGout(9)=1;
	delay_us(30);  
	DHT11Pin_IN();  //��ʼ����
	while(PGin(9)==1) //�ȴ�dht11�����ź�
	{
		i++;
		delay_us(1);
		if(i>100)
			return -1;  //��Ӧ��ʱ
	}
	i=0;
	while(PGin(9)==0)
	{
		i++;
		delay_us(1);
		if(i>100)
			return -2;  //��Ӧ��ʱ
	}
	i=0;
	while(PGin(9)==1)
	{
		i++;
		delay_us(1);
		if(i>100)
			return -3;  //��Ӧ��ʱ
	}
	
	for(int j=0,tmp;j<5;j++)
	{
		dat8=0;   
		for(i=7;i>=0;i--){
			tmp=dht11_bit();
			if(tmp!=-1)
				dat8 |= tmp<<i;
			else
				return -5;
		}
		data[j]=dat8;
	}
	//printf("T=%d.%d,H=%d.%d\n",data[2],data[3],data[0],data[1]);
	if(((data[0]+data[1]+data[2]+data[3])&0xFF)==data[4])
		return 0;
	
	return -4;
}








