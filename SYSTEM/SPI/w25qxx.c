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
//��ȡW25QXX��״̬�Ĵ���
//BIT7  6    5    4    3    2    1    0
//SPR   RV  TB   BP2  BP1  BP0  WEL  BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
static uint8_t W25QXX_ReadSR(void)   
{  
	uint8_t byte=0;   
	W25QXX_CS=0;                             //ʹ������   
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI1_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	W25QXX_CS=1;                            //ȡ��Ƭѡ     
	return byte;   
} 

//�ȴ�����
static void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
} 

//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
    W25QXX_Write_Enable();                  //SET WEL 
	W25QXX_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_PageProgram);      //����дҳ����   
    SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>16)); //����24bit��ַ    
    SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>8));   
    SPI1_ReadWriteByte((uint8_t)  WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	W25QXX_CS=1;                            //ȡ��Ƭѡ 
	delay_us(5);
	W25QXX_Wait_Busy();					   //�ȴ�д�����
	W25QXX_Write_Disable();
} 
//����������һ������4096�ֽ�
void W25X_Sector_Erase(uint32_t addr)
{
	W25QXX_Write_Enable();
	W25QXX_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_SectorErase);      //����������������   
    SPI1_ReadWriteByte((uint8_t)((addr)>>16)); //����24bit��ַ    
    SPI1_ReadWriteByte((uint8_t)((addr)>>8));   
    SPI1_ReadWriteByte((uint8_t)  addr);   
	W25QXX_CS=1;                            //ȡ��Ƭѡ 
	delay_us(5);
	W25QXX_Wait_Busy();					   //�ȴ���������
	W25QXX_Write_Disable();
}

