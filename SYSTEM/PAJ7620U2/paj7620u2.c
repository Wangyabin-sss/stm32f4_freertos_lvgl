#include "paj7620u2.h"
#include "paj7620u2_cfg.h"
//#include "usart.h"


//ѡ��PAJ7620U2 BANK����
void paj7620u2_selectBank(bank_e bank)
{
	switch(bank)
	{
		case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0�Ĵ�������
		case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1�Ĵ�������
	}		
}

//PAJ7620U2����
uint8_t paj7620u2_wakeup(void)
{ 
	uint8_t data=0x0a;
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	data = GS_Read_Byte(0x00);//��ȡ״̬
	if(data!=0x20) return 0; //����ʧ��
	
	return 1;
}

//PAJ7620U2��ʼ��
//����ֵ��0:ʧ�� 1:�ɹ�
uint8_t paj7620u2_init(void)
{
	uint8_t i;
	uint8_t status;
	
	GS_i2c_init();//IIC��ʼ��
    status = paj7620u2_wakeup();//����PAJ7620U2
	if(!status) 
		return 0;
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<INIT_SIZE;i++)
	{
		GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//��ʼ��PAJ7620U2
	}
    paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	
	return 1;
}

void Gesture_init(void)
{
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	
	for(int i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//����ʶ��ģʽ��ʼ��
	}
	
	paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
}

/*
//����ʶ�����
void Gesture_test(void)
{
	uint8_t i;
    uint8_t status;

	uint8_t data[2]={0x00};
	uint16_t gesture_data;
	

	
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//����ʶ��ģʽ��ʼ��
	}
	
	paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	
	i=0;
	while(1)
	{
		//��ȡ����״̬			
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
											   
						break; //����
					case GES_DOWM:                  
               			printf("Dowm\r\n"); 
											   					
						break; //����
					case GES_LEFT:                       
  						printf("Left\r\n");                
						break; //����
					case GES_RIGHT:               
                		printf("Right\r\n");               
						break; //����
					case GES_FORWARD:                 
						printf("Forward\r\n");             
						break; //��ǰ
					case GES_BACKWARD:            
            			printf("Backward\r\n");            
						break; //���
					case GES_CLOCKWISE:             
                		printf("Clockwise\r\n");     
						break; //˳ʱ��
					case GES_COUNT_CLOCKWISE:   
                   		printf("AntiClockwise\r\n");   
						break; //��ʱ��
					case GES_WAVE:                 
						printf("Wave\r\n");                
						break; //�Ӷ�
					default:   break;
					
				}		
			}
		}

		delay_ms(50);
	}
}

//�ӽ�������
void Ps_test(void)
{
	uint8_t i;

	uint8_t data[2]={0x00};
	uint8_t obj_brightness=0;
	uint16_t obj_size=0;
	
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	
	for(i=0;i<PROXIM_SIZE;i++)
	{
		GS_Write_Byte(proximity_arry[i][0],proximity_arry[i][1]);//�ӽ����ģʽ��ʼ��
	}
	
	paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	
	i=0;

	while(1)
	{	

		obj_brightness = GS_Read_Byte(PAJ_GET_OBJECT_BRIGHTNESS);//��ȡ��������
		data[0] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_1);//��ȡ�����С
		data[1] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_2);
		obj_size = ((uint16_t)data[1] & 0x0f)<<8 | data[0];
		printf("obj_brightness: %d\r\n",obj_brightness);
        printf("obj_size: %d\r\n",obj_size);
		
		delay_ms(100);
		
		i++;
	}	
}
*/
