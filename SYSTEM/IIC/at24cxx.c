#include "at24cxx.h"


//at24c02每次仅支持连续写8个字节
int32_t at24c02_write(uint8_t addr,uint8_t *buf,uint8_t len)
{
	uint8_t ack;
	uint8_t *p=buf;
	
	iic_start();
	iic_send_byte(AT24C02|0);	//进行设备寻址，写是0 读是1
	ack=iic_wait_ack();
	if(ack)
		return -1;
	iic_send_byte(addr);           //发送要放问eeprom数据的起始地址
	ack=iic_wait_ack();
	if(ack)
		return -2;
	while(len--)                  //连续发送数据
	{
		iic_send_byte(*p++);
		ack=iic_wait_ack();
		if(ack)
			return -3;
	}
	iic_stop();
//	delay_ms(5);                //数据真正写入最长要5ms
	return 0;
}

int32_t at24c02_read(uint8_t addr,uint8_t *buf,uint8_t len)
{
	uint8_t ack;
	uint8_t *p=buf;
	iic_start();   //发送起始信号
	iic_send_byte(AT24C02|0); //进行设备寻址，写访问地址为0xA0
	ack=iic_wait_ack();
	if(ack)
		return -1;
	iic_send_byte(addr);        //发送地址
	ack=iic_wait_ack();
	if(ack)
		return -2;
	iic_start();
	iic_send_byte(AT24C02|1);
	ack=iic_wait_ack();
	if(ack)
		return -3;
	while(--len)
	{
		*p++=iic_recv_byte();
		iic_ack(0);
	}
	*p=iic_recv_byte();
	iic_ack(1);
	iic_stop();
	return 0;
}









