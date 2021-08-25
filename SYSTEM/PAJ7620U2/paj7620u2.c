#include "paj7620u2.h"
#include "paj7620u2_cfg.h"
//#include "usart.h"


//选择PAJ7620U2 BANK区域
void paj7620u2_selectBank(bank_e bank)
{
	switch(bank)
	{
		case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0寄存器区域
		case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1寄存器区域
	}		
}

//PAJ7620U2唤醒
uint8_t paj7620u2_wakeup(void)
{ 
	uint8_t data=0x0a;
	GS_WakeUp();//唤醒PAJ7620U2
	delay_ms(5);//唤醒时间>400us
	GS_WakeUp();//唤醒PAJ7620U2
	delay_ms(5);//唤醒时间>400us
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	data = GS_Read_Byte(0x00);//读取状态
	if(data!=0x20) return 0; //唤醒失败
	
	return 1;
}

//PAJ7620U2初始化
//返回值：0:失败 1:成功
uint8_t paj7620u2_init(void)
{
	uint8_t i;
	uint8_t status;
	
	GS_i2c_init();//IIC初始化
    status = paj7620u2_wakeup();//唤醒PAJ7620U2
	if(!status) 
		return 0;
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	for(i=0;i<INIT_SIZE;i++)
	{
		GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//初始化PAJ7620U2
	}
    paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
	
	return 1;
}

void Gesture_init(void)
{
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	
	for(int i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//手势识别模式初始化
	}
	
	paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
}

/*
//手势识别测试
void Gesture_test(void)
{
	uint8_t i;
    uint8_t status;

	uint8_t data[2]={0x00};
	uint16_t gesture_data;
	

	
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//手势识别模式初始化
	}
	
	paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
	
	i=0;
	while(1)
	{
		//读取手势状态			
        status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);
		
		if(!status)
		{   
			gesture_data =(uint16_t)data[1]<<8 | data[0];
			
			if(gesture_data) 
			{
				switch(gesture_data)
				{
					case GES_UP:               
						printf("Up\r\n");
											   
						break; //向上
					case GES_DOWM:                  
               			printf("Dowm\r\n"); 
											   					
						break; //向下
					case GES_LEFT:                       
  						printf("Left\r\n");                
						break; //向左
					case GES_RIGHT:               
                		printf("Right\r\n");               
						break; //向右
					case GES_FORWARD:                 
						printf("Forward\r\n");             
						break; //向前
					case GES_BACKWARD:            
            			printf("Backward\r\n");            
						break; //向后
					case GES_CLOCKWISE:             
                		printf("Clockwise\r\n");     
						break; //顺时针
					case GES_COUNT_CLOCKWISE:   
                   		printf("AntiClockwise\r\n");   
						break; //逆时针
					case GES_WAVE:                 
						printf("Wave\r\n");                
						break; //挥动
					default:   break;
					
				}		
			}
		}

		delay_ms(50);
	}
}

//接近检测测试
void Ps_test(void)
{
	uint8_t i;

	uint8_t data[2]={0x00};
	uint8_t obj_brightness=0;
	uint16_t obj_size=0;
	
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	
	for(i=0;i<PROXIM_SIZE;i++)
	{
		GS_Write_Byte(proximity_arry[i][0],proximity_arry[i][1]);//接近检测模式初始化
	}
	
	paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
	
	i=0;

	while(1)
	{	

		obj_brightness = GS_Read_Byte(PAJ_GET_OBJECT_BRIGHTNESS);//读取物体亮度
		data[0] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_1);//读取物体大小
		data[1] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_2);
		obj_size = ((uint16_t)data[1] & 0x0f)<<8 | data[0];
		printf("obj_brightness: %d\r\n",obj_brightness);
        printf("obj_size: %d\r\n",obj_size);
		
		delay_ms(100);
		
		i++;
	}	
}
*/
