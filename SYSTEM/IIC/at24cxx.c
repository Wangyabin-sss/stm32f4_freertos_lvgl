#include "at24cxx.h"


//at24c02ÿ�ν�֧������д8���ֽ�
int32_t at24c02_write(uint8_t addr,uint8_t *buf,uint8_t len)
{
	uint8_t ack;
	uint8_t *p=buf;
	
	iic_start();
	iic_send_byte(AT24C02|0);	//�����豸Ѱַ��д��0 ����1
	ack=iic_wait_ack();
	if(ack)
		return -1;
	iic_send_byte(addr);           //����Ҫ����eeprom���ݵ���ʼ��ַ
	ack=iic_wait_ack();
	if(ack)
		return -2;
	while(len--)                  //������������
	{
		iic_send_byte(*p++);
		ack=iic_wait_ack();
		if(ack)
			return -3;
	}
	iic_stop();
//	delay_ms(5);                //��������д���Ҫ5ms
	return 0;
}

int32_t at24c02_read(uint8_t addr,uint8_t *buf,uint8_t len)
{
	uint8_t ack;
	uint8_t *p=buf;
	iic_start();   //������ʼ�ź�
	iic_send_byte(AT24C02|0); //�����豸Ѱַ��д���ʵ�ַΪ0xA0
	ack=iic_wait_ack();
	if(ack)
		return -1;
	iic_send_byte(addr);        //���͵�ַ
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









