#include "w25qxx.h"


uint16_t W25QXX_Read_Id(void)
{
	uint16_t ID=0;
	W25QXX_CS=0;
	SPI1_ReadWriteByte(W25X_ManufactDeviceID);
	SPI1_ReadWriteByte(0x0);
	SPI1_ReadWriteByte(0x0);
	SPI1_ReadWriteByte(0x0);
	ID  = SPI1_ReadWriteByte(0xff)<<8;
	ID |= SPI1_ReadWriteByte(0xff);
	W25QXX_CS=1;
	return ID;
}


void W25QXX_Read_Data(uint8_t *data,uint32_t addr,uint32_t num)
{
	W25QXX_CS=0;
	SPI1_ReadWriteByte(W25X_ReadData);
	SPI1_ReadWriteByte((uint8_t)(addr>>16));
	SPI1_ReadWriteByte((uint8_t)(addr>>8));
	SPI1_ReadWriteByte((uint8_t) addr);
	for(int i=0;i<num;i++)
    {
        data[i]=SPI1_ReadWriteByte(0xff);
    }
	W25QXX_CS=1;
}

void W25QXX_Write_Enable(void)
{
	W25QXX_CS=0;
	SPI1_ReadWriteByte(W25X_WriteEnable);
	W25QXX_CS=1;
}

void W25QXX_Write_Disable(void)
{
	W25QXX_CS=0;
	SPI1_ReadWriteByte(W25X_WriteDisable);
	W25QXX_CS=1;
}
//读取W25QXX的状态寄存器
//BIT7  6    5    4    3    2    1    0
//SPR   RV  TB   BP2  BP1  BP0  WEL  BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
static uint8_t W25QXX_ReadSR(void)   
{  
	uint8_t byte=0;   
	W25QXX_CS=0;                             //使能器件   
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=SPI1_ReadWriteByte(0Xff);             //读取一个字节  
	W25QXX_CS=1;                            //取消片选     
	return byte;   
} 

//等待空闲
static void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
} 

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
    W25QXX_Write_Enable();                  //SET WEL 
	W25QXX_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X_PageProgram);      //发送写页命令   
    SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>16)); //发送24bit地址    
    SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>8));   
    SPI1_ReadWriteByte((uint8_t)  WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//循环写数  
	W25QXX_CS=1;                            //取消片选 
	delay_us(5);
	W25QXX_Wait_Busy();					   //等待写入结束
	W25QXX_Write_Disable();
} 
//扇区擦除，一个扇区4096字节
void W25X_Sector_Erase(uint32_t addr)
{
	W25QXX_Write_Enable();
	W25QXX_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X_SectorErase);      //发送扇区擦除命令   
    SPI1_ReadWriteByte((uint8_t)((addr)>>16)); //发送24bit地址    
    SPI1_ReadWriteByte((uint8_t)((addr)>>8));   
    SPI1_ReadWriteByte((uint8_t)  addr);   
	W25QXX_CS=1;                            //取消片选 
	delay_us(5);
	W25QXX_Wait_Busy();					   //等待擦除结束
	W25QXX_Write_Disable();
}

