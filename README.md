## 2017.8.13
* ���ͼƬ
```c
LV_IMG_DECLARE(watch_bg);
LV_IMG_DECLARE(hour);
LV_IMG_DECLARE(minute);
LV_IMG_DECLARE(second);
```
## 2021.8.14
* ��gui.c��Ϊutf-8���룬������ʾ����
* ����������壨font_chinese.c��
[���幤��](https://lvgl.io/tools/fontconverter)
```c
//�������������  [22���� 4bpp]
�����й��Ƿ���
```
## 2021.8.16
* tabview�в�����lv_btnmatrix_create(tab2, NULL);
## 2021.8.19
* ��������ɵ�
* ������ʾ�ȽϿ�,�������ö���ʱ��
* ��ʱ��Ῠ��
```c
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
	uint8_t sta,i=0;
 	//SPI1_SetSpeed(SPI_BaudRatePrescaler_8);
	NRF24L01_CE=0;
	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);
 	NRF24L01_CE=1;	   
	while(NRF24L01_IRQ!=0);               ������
	//delay_ms(10);
	sta=NRF24L01_Read_Reg(STATUS);  
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); 
	if(sta&MAX_TX)
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}
```
* esp8266 wifi����δ�Ż�
* RFID�����ɾ��δд
* ����δд
