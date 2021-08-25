#include "main.h"


struct RX_DATA RX1_data={"",0,0};
struct RX_DATA RX2_data={"",0,0};
struct RX_DATA RX3_data={"",0,0};


void usart1_init(uint32_t band)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); //����1Ӳ��ʱ�Ӵ�
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9 | GPIO_Pin_10; //����LED������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;        //����ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;    //100MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		 //����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��������֧�ֺܶ๦�ܣ���Ҫָ�������ŵĹ��ܣ���ǰҪ�ƶ�֧��USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	USART_InitStructure.USART_BaudRate = band;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8λ����λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;       //1��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;        //����ҪУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Ӳ�������ƹ��ܲ���Ҫ
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //���������ͺͽ�������
    USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // Enable USART 
    USART_Cmd(USART1, ENABLE);
}

void usart2_init(uint32_t band)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_3; //����LED������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;        //����ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;    //100MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		 //����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//��������֧�ֺܶ๦�ܣ���Ҫָ�������ŵĹ��ܣ���ǰҪ�ƶ�֧��USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART1);
	
	USART_InitStructure.USART_BaudRate = band;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8λ����λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;       //1��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;        //����ҪУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Ӳ�������ƹ��ܲ���Ҫ
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //���������ͺͽ�������
    USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // Enable USART 
    USART_Cmd(USART2,ENABLE);
}

void usart3_init(uint32_t band)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //����1Ӳ��ʱ�Ӵ�
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_11; //����LED������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;        //����ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;    //100MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;		 //����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//��������֧�ֺܶ๦�ܣ���Ҫָ�������ŵĹ��ܣ���ǰҪ�ƶ�֧��USART1
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	USART_InitStructure.USART_BaudRate = band;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8λ����λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;       //1��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;        //����ҪУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Ӳ�������ƹ��ܲ���Ҫ
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //���������ͺͽ�������
    USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // Enable USART 
    USART_Cmd(USART3, ENABLE);
}

/*********************************************************************************************************************************/

void USART1_IRQHandler(void)
{
	uint8_t data;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		data=USART_ReceiveData(USART1);
		if(data=='\n'||data=='#'){                //������ֻ��ͬʱ��һ����Ȼ����ܿ�����
			RX1_data.data[RX1_data.a]=0;
			RX1_data.b=usartdown;                //��1��ʾ���ݽ������
		}else{
			if(RX1_data.b!=usarting)             //���ڲ��ڹ���״̬
			{
				RX1_data.b=usarting;RX1_data.a=0;
			}
			RX1_data.data[RX1_data.a++]=data;   //�������ݵ��ṹ������
			if(RX1_data.a==RXMAX-1){
				RX1_data.data[RX1_data.a]=data; //���չ����ַ���
			}
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

void USART2_IRQHandler(void)
{
	uint8_t data;
	uint32_t ulReturn;
	
	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		data=USART_ReceiveData(USART2);
		if(data=='#')
		{
			RX2_data.data[RX1_data.a]=0;
			xQueueSendFromISR(queue_usart2,&RX2_data,NULL);
			RX2_data.a=0;
		}else{
			RX2_data.data[RX2_data.a++]=data;
			if(RX2_data.a==RXMAX-1){
				RX2_data.data[RX2_data.a]=data; //���չ����ַ���
			}
		}
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}

void USART3_IRQHandler(void)
{
	uint8_t data;
	uint32_t ulReturn;
	
	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		data=USART_ReceiveData(USART3);
		RX3_data.data[RX3_data.a++]=data;
		if(RX3_data.a==RXMAX-1){
			RX3_data.data[RX3_data.a]=data; //���չ����ַ���
		}
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}

/**************************************************************************************************************************************/

void usart1_send_str(char *str)
{
	char *p = str;
	
	while(*p!='\0')
	{
		/* ����һ���ֽ� */
		USART_SendData(USART1,*p++);
		/* �ȴ�������� */
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	}
}

void usart2_send_str(char *str)
{
	char *p = str;
	
	while(*p!='\0')
	{
		/* ����һ���ֽ� */
		USART_SendData(USART2,*p++);
		/* �ȴ�������� */
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
	}
}

void usart3_send_str(char *str)
{
	char *p = str;
	
	while(*p!='\0')
	{
		/* ����һ���ֽ� */
		USART_SendData(USART3,*p++);
		/* �ȴ�������� */
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
	}

}

void usart1_send_hex(uint8_t *data,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		USART_SendData(USART1,data[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	}
}

void usart2_send_hex(uint8_t *data,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		USART_SendData(USART2,data[i]);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
	}
}

void usart3_send_hex(uint8_t *data,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		USART_SendData(USART3,data[i]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
	}
}

/*****************************************************************************************************************************/

char* find_data(char *str,char *start,char *end,uint16_t *len) 
{
	char *sp,*ep;
	sp=strstr(str,start);
	if(sp==NULL)
		return NULL;
	ep=strstr(str,end);
	if(ep==NULL)
		return NULL;
	sp=sp+strlen(start);
	*len=ep-sp;
	return sp;
}


/****************************************************************************************************************************/
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int c, FILE *f) 
{
	/* ����һ���ֽ� */
	USART_SendData(Uprintf,c);

	/* �ȴ�������� */
	while(USART_GetFlagStatus(Uprintf,USART_FLAG_TXE) == RESET);

	/* ���ط��͵��ַ� */
	return c;	
}


