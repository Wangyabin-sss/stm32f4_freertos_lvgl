#include "fsmc.h"


_lcd_dev lcd_dev;

//tftlcd.c �ļ�����������ʾ
 
/****************************************************************************************
* �� �� ��:	TFTLCD_GPIO_Init
* ��������:	tft lcd ��ʾ����ʼ������
* ��     ��:	
			��
* ��     ��:	��
* �޸ļ�¼: 
* ��     ��:
* ��		��:
* ���ܲ��ԣ����ܲ�������
*******************************************************************************************/
 
static void TFTLCD_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|
	RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��PB,PD,PE,PF,PGʱ��  
 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);  //FSMC_D0-FSMC_D15
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);  
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);  //FSMC_NOE
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);  //FSMC_NWE
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC); //FSMC_A6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC); //FSMC_NE4
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 
	                              | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9
								  | GPIO_Pin_10 | GPIO_Pin_14| GPIO_Pin_15;//PD0,1,4,5,8,9,10,14,15 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 
	                              | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
								  | GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15;//PE7~15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��  
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PF12,FSMC_A6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PG12,FSMC_NE4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ�� 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;   //��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}
/****************************************************************************************
* �� �� ��:	TFTLCD_FSMC_Init
* ��������:	tft fsmc ������ʼ������
* ��     ��:	
			��
* ��     ��:	��
* �޸ļ�¼: 
* ��     ��:
* ��		��:
* ���ܲ��ԣ����ܲ�������
*******************************************************************************************/
static void TFTLCD_FSMC_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  FSMC_ReadNORSRAMTiming; 
	FSMC_NORSRAMTimingInitTypeDef  FSMC_WriteNORSRAMTiming;
	
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//ʹ��FSMCʱ��
	
	//��
	FSMC_ReadNORSRAMTiming.FSMC_AddressSetupTime = 0X01;	 //��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/168M=6ns*2=12ns	
	FSMC_ReadNORSRAMTiming.FSMC_AddressHoldTime = 0x00;	 	//��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
	FSMC_ReadNORSRAMTiming.FSMC_DataSetupTime = 0x0f;			//���ݱ���ʱ��Ϊ16��HCLK	��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫��
	FSMC_ReadNORSRAMTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_ReadNORSRAMTiming.FSMC_CLKDivision = 0x00;
	FSMC_ReadNORSRAMTiming.FSMC_DataLatency = 0x00;
	FSMC_ReadNORSRAMTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
 
	//д
	FSMC_WriteNORSRAMTiming.FSMC_AddressSetupTime =0x1;	      //��ַ����ʱ��
	FSMC_WriteNORSRAMTiming.FSMC_AddressHoldTime = 0x2;	 //��ַ����ʱ��
	FSMC_WriteNORSRAMTiming.FSMC_DataSetupTime = 0x4;		 //���ݱ���ʱ��
	FSMC_WriteNORSRAMTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_WriteNORSRAMTiming.FSMC_CLKDivision = 0x00;
	FSMC_WriteNORSRAMTiming.FSMC_DataLatency = 0x00;
	FSMC_WriteNORSRAMTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
 
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ���������ݵ�ַ
	FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit   
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  �洢��дʹ��
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadNORSRAMTiming; //��дʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_WriteNORSRAMTiming;  //дʱ��
 
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // ʹ��BANK1
	
}
/**************************************************************************************
* �� �� ��:	openBackLight
* ��������:	����LCD�ı���
* ��     ��:	
			��
* ��     ��:	��
* �޸ļ�¼: 
* ��     ��:
* ��		��:
* ���ܲ��ԣ����ܲ�������
*******************************************************************************************/
void openLCDBackLight(int i)
{
	if(i==1){
		GPIO_SetBits(GPIOB, GPIO_Pin_15);
	}else{
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	}
}
/**************************************************************************************
* �� �� ��:	LCD_WR_REG
* ��������:	lcd ���Ĵ�����д����
* ��     ��:	
			��
* ��     ��:	��
* �޸ļ�¼: 
* ��     ��:
* ��		��:
* ���ܲ��ԣ����ܲ�������
*******************************************************************************************/
void LCD_WR_REG(uint16_t cmd)
{
	TFTLCD->LCD_CMD=cmd;
}
 
/****************************************************************************************
* �� �� ��:	LCD_WR_DATA
* ��������:	lcd ���Ĵ���������
* ��     ��:	
			��
* ��     ��:	��
* �޸ļ�¼: 
* ��     ��:
* ��		��:
* ���ܲ��ԣ����ܲ�������
*******************************************************************************************/
void LCD_WR_DATA(uint16_t data)
{
	TFTLCD->LCD_DATA=data;
}
/****************************************************************************************
* �� �� ��:	LCD_WriteCmdData
* ��������:	lcd ���Ĵ�����д���������
* ��     ��:	
			��
* ��     ��:	��
* �޸ļ�¼: 
* ��     ��:
* ��		��:
* ���ܲ��ԣ����ܲ�������
*******************************************************************************************/
void LCD_WriteCmdData(uint16_t cmd,uint16_t data)
{
	LCD_WR_REG(cmd);
	LCD_WR_DATA(data);
}
 
 
uint16_t LCD_ReadData(void)
{
	return TFTLCD->LCD_DATA;
}
 

void TFTLCD_Init(void)
{
	//uint16_t i;
	TFTLCD_GPIO_Init();
	TFTLCD_FSMC_Init();
	
	delay_ms(50); 
 
	//Һ������ʼ��
	LCD_WR_REG(0XF1);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0X0F);
	LCD_WR_DATA(0x8F);
	LCD_WR_REG(0XF2);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0xA3);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0XB2);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0xFF);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0XF8);
	LCD_WR_DATA(0x21);
	LCD_WR_DATA(0x04);
	LCD_WR_REG(0XF9);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x08);
	LCD_WR_REG(0x36);
	LCD_WR_DATA(0x08);
	LCD_WR_REG(0xB4);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xB6);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x22);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA(0x41);
	LCD_WR_REG(0xC5);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x18);
	LCD_WR_REG(0xE0);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x1F);
	LCD_WR_DATA(0x1C);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x48);
	LCD_WR_DATA(0x98);
	LCD_WR_DATA(0x37);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x13);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0xE1);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x2E);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x0D);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x47);
	LCD_WR_DATA(0x75);
	LCD_WR_DATA(0x37);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0x24);
	LCD_WR_DATA(0x20);
	LCD_WR_DATA(0x00);
	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);
	LCD_WR_REG(0x11);
	LCD_WR_REG(0x36);
	LCD_WR_DATA(0xC8);
	delay_ms(120);
	LCD_WR_REG(0x29);
	
	LCD_SetDir();      //0������  1������  Ĭ������
	openLCDBackLight(1); //��������
	LCD_Clear(BLACK);
}
 
 
//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:width*height. 
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{ 
	uint16_t twidth,theight;
	twidth=sx+width;
	theight=sy+height;
	
	LCD_WR_REG(lcd_dev.setxcmd);
    LCD_WR_DATA(sx>>8);
    LCD_WR_DATA(sx&0XFF);
    LCD_WR_DATA(twidth>>8);
    LCD_WR_DATA(twidth&0XFF);
    LCD_WR_REG(lcd_dev.setycmd);
    LCD_WR_DATA(sy>>8);
    LCD_WR_DATA(sy&0XFF);
    LCD_WR_DATA(theight>>8);
    LCD_WR_DATA(theight&0XFF);
	
	LCD_WR_REG(lcd_dev.wramcmd);
}
 
//��������
//color:Ҫ���������ɫ
void LCD_Clear(uint16_t color)
{
	uint16_t i, j ;
	
	LCD_Set_Window(0, 0, lcd_dev.width, lcd_dev.height);	 //��������
	
  	for(i=0; i<lcd_dev.width; i++)
	{
		for (j=0; j<lcd_dev.height; j++)
		{
			LCD_WR_DATA(color);
		}
	} 
}
 
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	LCD_WR_REG(lcd_dev.setxcmd);	
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(0x00FF&Xpos);		//����ˮƽ���꣬��λ��ǰ
	LCD_WR_DATA((lcd_dev.width-1)>>8);
	LCD_WR_DATA((lcd_dev.width-1)&0xFF);
	
	LCD_WR_REG(lcd_dev.setycmd);	
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(0x00FF&Ypos);		
	LCD_WR_DATA((lcd_dev.height-1)>>8);
	LCD_WR_DATA((lcd_dev.height-1)&0xFF);
	
	LCD_WR_REG(lcd_dev.wramcmd);	//��ʼд��GRAM		
}

void LCD_SetDir(void)
{ 
	lcd_dev.setxcmd=0x2A;
	lcd_dev.setycmd=0x2B;
	lcd_dev.wramcmd=0x2C;
#if TFTLCD_DIR==1	//ʹ�ú���	  
	lcddev.dir=1;//����
	lcddev.width=480;
	lcddev.height=320;			
	LCD_WriteCmdData(0x36,(1<<3)|(1<<5)|(1<<7));//BGR==1,MY==1,MX==0,MV==1
//	LCD_WR_REG(0x2A); 	
//	LCD_WR_DATA(0x00);	// ����x���꣨������û�����ã�ֻ����Ҫ��д����ѣ�
//	LCD_WR_DATA(0x00); 			
//	LCD_WR_DATA(0x01);   // �������x����Ϊ240
//	LCD_WR_DATA(0xDF); 				
//					   
//	LCD_WR_REG(0x2B);    
//	LCD_WR_DATA(0x00);   // ����y���꣨������û�����ã�ֻ����Ҫ��д����ѣ�
//	LCD_WR_DATA(0x00); 			
//	LCD_WR_DATA(0x01);   // �������y����Ϊ320
//	LCD_WR_DATA(0X3F); 
#else//����
	lcd_dev.dir=0;//����				 	 		
	lcd_dev.width=320;
	lcd_dev.height=480;	
	LCD_WriteCmdData(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
//	LCD_WR_REG(0x2A); 	
//	LCD_WR_DATA(0x00);	// ����x���꣨������û�����ã�ֻ����Ҫ��д����ѣ�
//	LCD_WR_DATA(0x00); 			
//	LCD_WR_DATA(0x01);   // �������x����Ϊ240
//	LCD_WR_DATA(0x3F); 				
//					   
//	LCD_WR_REG(0x2B);    
//	LCD_WR_DATA(0x00);   // ����y���꣨������û�����ã�ֻ����Ҫ��д����ѣ�
//	LCD_WR_DATA(0x00); 			
//	LCD_WR_DATA(0x01);   // �������y����Ϊ320
//	LCD_WR_DATA(0XDF); 
#endif
}


void LCD_DrawPoint(uint16_t xpos,uint16_t ypos,uint16_t color)
{
	LCD_SetCursor(xpos,ypos);
	LCD_WR_DATA(color);
}
