#include "xpt2046.h"

void xpt2046_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;           //T_MOSI
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;    //100MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;	   //推挽
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;           //T_CS
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;    //100MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;	   //推挽
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;           //T_CLK
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;        //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;    //100MHz
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;	   //推挽
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;           //T_MISO
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;        //输入模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;    //100MHz
	//GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;	   //推挽
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;		 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

 /*********************************************************
 * 函数名：void Touch_WriteByte(u8 num)
 * 描述  ：向控制芯片写入一个命令数据
 * 输入  ：时序：CS为低电平，CLK上升沿锁存数据； 
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：内部调用 
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
		T_CLK=1;		//上升沿有效	        
	}	
}  

 /*********************************************************
 * 函数名：u16 Touch_ReadAD(u8 cmd)
 * 描述  ：读取X/Y轴坐标，并返回AD值； 2^12 = 4096;
 * 输入  ：
 * 输出  ：无
 * 返回  ： 
 * 调用  ：内部调用 
 **********************************************************/
uint16_t Touch_ReadAD(uint8_t cmd)
{
	uint8_t count=0; 	  
	uint16_t Num=0; 
	T_CLK=0;		//先拉低时钟 	 
	T_MOSI=0; 	    //拉低数据线
	T_CS=0; 		//选中触摸屏IC
	Touch_WriteByte(cmd);//发送命令字
	delay_us(6);  //的转换时间最长为6us
	T_CLK=0; 	     	    
	delay_us(1);  
	T_CLK=1;	   //给1个时钟，清除BUSY	   
	delay_us(1);    
	T_CLK=0; 	     	    
	for(count=0;count<16;count++)//读出16位数据,只有高12位有效 
	{
		Num<<=1; 	 
		T_CLK=0;	//下降沿有效  	    	   
		delay_us(1);
 		T_CLK=1;
 		if(T_MISO)Num++;
	}  	
	Num>>=4;   	//只有高12位有效.
	T_CS=1;		//释放片选	 
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










