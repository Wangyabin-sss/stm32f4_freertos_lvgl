#include "xpt2046.h"

void xpt2046_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;           //T_MOSI
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //���ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;    //100MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;	   //����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;           //T_CS
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //���ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;    //100MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;	   //����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;           //T_CLK
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //���ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;    //100MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;	   //����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;           //T_MISO
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;        //����ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;    //100MHz
	//GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;	   //����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

 /*********************************************************
 * ��������void Touch_WriteByte(u8 num)
 * ����  �������оƬд��һ����������
 * ����  ��ʱ��CSΪ�͵�ƽ��CLK�������������ݣ� 
 * ���  ����
 * ����  ���� 
 * ����  ���ڲ����� 
 **********************************************************/
void Touch_WriteByte(uint8_t num)
{
	uint8_t count=0;   
	for(count=0;count<8;count++)  
	{
		if(num&0x80)
            T_MOSI=1;
		else
            T_MOSI=0;   
		num<<=1;    
		T_CLK=0;
		delay_us(1);
		T_CLK=1;		//��������Ч	        
	}	
}  

 /*********************************************************
 * ��������u16 Touch_ReadAD(u8 cmd)
 * ����  ����ȡX/Y�����꣬������ADֵ�� 2^12 = 4096;
 * ����  ��
 * ���  ����
 * ����  �� 
 * ����  ���ڲ����� 
 **********************************************************/
uint16_t Touch_ReadAD(uint8_t cmd)
{
	uint8_t count=0; 	  
	uint16_t Num=0; 
	T_CLK=0;		//������ʱ�� 	 
	T_MOSI=0; 	    //����������
	T_CS=0; 		//ѡ�д�����IC
	Touch_WriteByte(cmd);//����������
	delay_us(6);  //��ת��ʱ���Ϊ6us
	T_CLK=0; 	     	    
	delay_us(1);  
	T_CLK=1;	   //��1��ʱ�ӣ����BUSY	   
	delay_us(1);    
	T_CLK=0; 	     	    
	for(count=0;count<16;count++)//����16λ����,ֻ�и�12λ��Ч 
	{
		Num<<=1; 	 
		T_CLK=0;	//�½�����Ч  	    	   
		delay_us(1);
 		T_CLK=1;
 		if(T_MISO)Num++;
	}  	
	Num>>=4;   	//ֻ�и�12λ��Ч.
	T_CS=1;		//�ͷ�Ƭѡ	 
	return(Num);   
}


void touch_getxy(int16_t *posx,int16_t *posy)
{
	uint32_t xall=0,yall=0,i=0;
	for(i=0;i<10;i++)
	{
		xall+=Touch_ReadAD(Tcmd_X);
		yall+=Touch_ReadAD(Tcmd_Y);
	}
	xall=xall/10;yall=yall/10;
	*posx=(int16_t)(-0.0851*xall+333.82);
	*posy=(int16_t)(0.1296*yall-35.352);
}


int32_t is_Touchdown(void)
{
	if(Touch_ReadAD(Tcmd_X)==0||Touch_ReadAD(Tcmd_Y)==4095)
		return -1;
	else
		return 0;
}










