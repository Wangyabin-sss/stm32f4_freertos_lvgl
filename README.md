## 2017.8.13
* 添加图片
```c
LV_IMG_DECLARE(watch_bg);
LV_IMG_DECLARE(hour);
LV_IMG_DECLARE(minute);
LV_IMG_DECLARE(second);
```
## 2021.8.14
* 将gui.c改为utf-8编码，中文显示正常
* 添加中文字体（font_chinese.c）
[字体工具](https://lvgl.io/tools/fontconverter)
```c
//已添加文字如下  [22像素 4bpp]
设置中国是否开启
```
## 2021.8.16
* tabview中不能用lv_btnmatrix_create(tab2, NULL);
## 2021.8.19
* 触摸屏会飞点
* 日历显示比较卡,可以设置动画时长
* 有时候会卡在
```c
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
	uint8_t sta,i=0;
 	//SPI1_SetSpeed(SPI_BaudRatePrescaler_8);
	NRF24L01_CE=0;
	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);
 	NRF24L01_CE=1;	   
	while(NRF24L01_IRQ!=0);               卡在这
	//delay_ms(10);
	sta=NRF24L01_Read_Reg(STATUS);  
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); 
	if(sta&MAX_TX)
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}
```
* esp8266 wifi连接未优化
* RFID卡添加删除未写
* 蓝牙未写
