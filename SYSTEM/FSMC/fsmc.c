#include "fsmc.h"


_lcd_dev lcd_dev;

//tftlcd.c 文件程序如下所示
 
/****************************************************************************************
* 函 数 名:	TFTLCD_GPIO_Init
* 函数功能:	tft lcd 显示屏初始化函数
* 输     入:	
			无
* 输     出:	无
* 修改记录: 
* 作     者:
* 日		期:
* 功能测试：功能测试正常
*******************************************************************************************/
 
static void TFTLCD_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|
	RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//使能PB,PD,PE,PF,PG时钟  
 
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 
	                              | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
								  | GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15;//PE7~15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化  
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PF12,FSMC_A6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化  
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PG12,FSMC_NE4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;   //开启背光
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}
/****************************************************************************************
* 函 数 名:	TFTLCD_FSMC_Init
* 函数功能:	tft fsmc 参数初始化函数
* 输     入:	
			无
* 输     出:	无
* 修改记录: 
* 作     者:
* 日		期:
* 功能测试：功能测试正常
*******************************************************************************************/
static void TFTLCD_FSMC_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  FSMC_ReadNORSRAMTiming; 
	FSMC_NORSRAMTimingInitTypeDef  FSMC_WriteNORSRAMTiming;
	
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//使能FSMC时钟
	
	//读
	FSMC_ReadNORSRAMTiming.FSMC_AddressSetupTime = 0X01;	 //地址建立时间（ADDSET）为2个HCLK 1/168M=6ns*2=12ns	
	FSMC_ReadNORSRAMTiming.FSMC_AddressHoldTime = 0x00;	 	//地址保持时间（ADDHLD）模式A未用到	
	FSMC_ReadNORSRAMTiming.FSMC_DataSetupTime = 0x0f;			//数据保存时间为16个HCLK	因为液晶驱动IC的读数据的时候，速度不能太快
	FSMC_ReadNORSRAMTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_ReadNORSRAMTiming.FSMC_CLKDivision = 0x00;
	FSMC_ReadNORSRAMTiming.FSMC_DataLatency = 0x00;
	FSMC_ReadNORSRAMTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
 
	//写
	FSMC_WriteNORSRAMTiming.FSMC_AddressSetupTime =0x1;	      //地址建立时间
	FSMC_WriteNORSRAMTiming.FSMC_AddressHoldTime = 0x2;	 //地址保持时间
	FSMC_WriteNORSRAMTiming.FSMC_DataSetupTime = 0x4;		 //数据保存时间
	FSMC_WriteNORSRAMTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_WriteNORSRAMTiming.FSMC_CLKDivision = 0x00;
	FSMC_WriteNORSRAMTiming.FSMC_DataLatency = 0x00;
	FSMC_WriteNORSRAMTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 
 
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
	FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit   
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadNORSRAMTiming; //读写时序
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_WriteNORSRAMTiming;  //写时序
 
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // 使能BANK1
	
}
/**************************************************************************************
* 函 数 名:	openBackLight
* 函数功能:	开启LCD的背光
* 输     入:	
			无
* 输     出:	无
* 修改记录: 
* 作     者:
* 日		期:
* 功能测试：功能测试正常
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
* 函 数 名:	LCD_WR_REG
* 函数功能:	lcd 往寄存器中写命令
* 输     入:	
			无
* 输     出:	无
* 修改记录: 
* 作     者:
* 日		期:
* 功能测试：功能测试正常
*******************************************************************************************/
void LCD_WR_REG(uint16_t cmd)
{
	TFTLCD->LCD_CMD=cmd;
}
 
/****************************************************************************************
* 函 数 名:	LCD_WR_DATA
* 函数功能:	lcd 往寄存器中数据
* 输     入:	
			无
* 输     出:	无
* 修改记录: 
* 作     者:
* 日		期:
* 功能测试：功能测试正常
*******************************************************************************************/
void LCD_WR_DATA(uint16_t data)
{
	TFTLCD->LCD_DATA=data;
}
/****************************************************************************************
* 函 数 名:	LCD_WriteCmdData
* 函数功能:	lcd 往寄存器中写命令和数据
* 输     入:	
			无
* 输     出:	无
* 修改记录: 
* 作     者:
* 日		期:
* 功能测试：功能测试正常
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
 
	//液晶屏初始化
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
	
	LCD_SetDir();      //0：竖屏  1：横屏  默认竖屏
	openLCDBackLight(1); //开启背光
	LCD_Clear(BLACK);
}
 
 
//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height. 
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
 
//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint16_t color)
{
	uint16_t i, j ;
	
	LCD_Set_Window(0, 0, lcd_dev.width, lcd_dev.height);	 //作用区域
	
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
	LCD_WR_DATA(0x00FF&Xpos);		//设置水平坐标，高位在前
	LCD_WR_DATA((lcd_dev.width-1)>>8);
	LCD_WR_DATA((lcd_dev.width-1)&0xFF);
	
	LCD_WR_REG(lcd_dev.setycmd);	
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(0x00FF&Ypos);		
	LCD_WR_DATA((lcd_dev.height-1)>>8);
	LCD_WR_DATA((lcd_dev.height-1)&0xFF);
	
	LCD_WR_REG(lcd_dev.wramcmd);	//开始写入GRAM		
}

void LCD_SetDir(void)
{ 
	lcd_dev.setxcmd=0x2A;
	lcd_dev.setycmd=0x2B;
	lcd_dev.wramcmd=0x2C;
#if TFTLCD_DIR==1	//使用横屏	  
	lcddev.dir=1;//横屏
	lcddev.width=480;
	lcddev.height=320;			
	LCD_WriteCmdData(0x36,(1<<3)|(1<<5)|(1<<7));//BGR==1,MY==1,MX==0,MV==1
//	LCD_WR_REG(0x2A); 	
//	LCD_WR_DATA(0x00);	// 设置x坐标（在这里没有作用，只是需要先写入而已）
//	LCD_WR_DATA(0x00); 			
//	LCD_WR_DATA(0x01);   // 设置最大x坐标为240
//	LCD_WR_DATA(0xDF); 				
//					   
//	LCD_WR_REG(0x2B);    
//	LCD_WR_DATA(0x00);   // 设置y坐标（在这里没有作用，只是需要先写入而已）
//	LCD_WR_DATA(0x00); 			
//	LCD_WR_DATA(0x01);   // 设置最大y坐标为320
//	LCD_WR_DATA(0X3F); 
#else//竖屏
	lcd_dev.dir=0;//竖屏				 	 		
	lcd_dev.width=320;
	lcd_dev.height=480;	
	LCD_WriteCmdData(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
//	LCD_WR_REG(0x2A); 	
//	LCD_WR_DATA(0x00);	// 设置x坐标（在这里没有作用，只是需要先写入而已）
//	LCD_WR_DATA(0x00); 			
//	LCD_WR_DATA(0x01);   // 设置最大x坐标为240
//	LCD_WR_DATA(0x3F); 				
//					   
//	LCD_WR_REG(0x2B);    
//	LCD_WR_DATA(0x00);   // 设置y坐标（在这里没有作用，只是需要先写入而已）
//	LCD_WR_DATA(0x00); 			
//	LCD_WR_DATA(0x01);   // 设置最大y坐标为320
//	LCD_WR_DATA(0XDF); 
#endif
}


void LCD_DrawPoint(uint16_t xpos,uint16_t ypos,uint16_t color)
{
	LCD_SetCursor(xpos,ypos);
	LCD_WR_DATA(color);
}
