#ifndef __PAJ7620U2_H
#define __PAJ7620U2_H

#include "paj7620u2_iic.h"

//BANK�Ĵ���ö��
typedef enum{
	BANK0 = 0, //BANK0�Ĵ���
	BANK1,     //BANK1�Ĵ���
}bank_e;

#define PAJ7620_ID           0x73<<1 //�豸��ַ
#define PAJ_REGITER_BANK_SEL 0XEF    //BANKѡ��Ĵ���
#define PAJ_BANK0            0X00    //BANK0
#define PAJ_BANK1            0X01    //BANK1

//BANK0 �Ĵ�����
#define PAJ_SUSPEND_CMD            0X03 //�����豸����
#define PAJ_SET_INT_FLAG1          0X41 //�������Ƽ���жϼĴ���1
#define PAJ_SET_INT_FLAG2          0X42 //�������Ƽ���жϼĴ���2
#define PAJ_GET_INT_FLAG1          0X43 //��ȡ���Ƽ���жϱ�־�Ĵ���1(��ȡ���ƽ��)
#define PAJ_GET_INT_FLAG2          0X44 //��ȡ���Ƽ���жϱ�־�Ĵ���2(��ȡ���ƽ��)
#define PAJ_GET_STATE              0X45 //��ȡ���Ƽ�⹤��״̬
#define PAJ_SET_HIGH_THRESHOLD     0x69 //�����ͺ�߷�ֵ�����ڽӽ����ģʽ�£�
#define PAJ_SET_LOW_THRESEHOLD     0X6A //�����ͺ�ͷ�ֵ
#define PAJ_GET_APPROACH_STATE     0X6B //��ȡ�ӽ�״̬ ��1��PS data>= PS threshold ,0:PS data<= Low threshold��
#define PAJ_GET_GESTURE_DATA       0X6C //��ȡ�ӽ�����
#define PAJ_GET_OBJECT_BRIGHTNESS  0XB0 //��ȡ�����������ȣ����255��
#define PAJ_GET_OBJECT_SIZE_1      0XB1 //��ȡ���������С�Ͱ�λ��bit7:0��(���900)
#define PAJ_GET_OBJECT_SIZE_2      0XB2 //��ȡ���������С����λ��bit3:0��

//BANK1 �Ĵ�����
#define PAJ_SET_PS_GAIN         0X44 //���ü�������С (0:1x gain 1:2x gain)
#define PAJ_SET_IDLE_S1_STEP_0  0x67 //����S1����Ӧ����
#define PAJ_SET_IDLE_S1_STEP_1  0x68 
#define PAJ_SET_IDLE_S2_STEP_0  0X69 //����S2����Ӧ����
#define PAJ_SET_IDLE_S2_STEP_1  0X6A 
#define PAJ_SET_OP_TO_S1_STEP_0 0X6B //����OP��S1�Ĺ���ʱ��
#define PAJ_SET_OP_TO_S1_STEP_1 0X6C
#define PAJ_SET_S1_TO_S2_STEP_0 0X6D //����S1��S2�Ĺ���ʱ��
#define PAJ_SET_S1_TO_S2_STEP_1 0X6E
#define PAJ_OPERATION_ENABLE    0X72 //����PAJ7620U2ʹ�ܼĴ���

//����ʶ��Ч��
#define BIT(x) 1<<(x)

#define GES_UP              BIT(0) //����
#define GES_DOWM            BIT(1) //����
#define GES_LEFT            BIT(2) //����
#define GES_RIGHT           BIT(3) //����
#define GES_FORWARD         BIT(4) //��ǰ
#define GES_BACKWARD        BIT(5) //���
#define GES_CLOCKWISE       BIT(6) //˳ʱ��
#define GES_COUNT_CLOCKWISE BIT(7) //��ʱ��
#define GES_WAVE            BIT(8) //�Ӷ�

uint8_t paj7620u2_init(void);
void Gesture_init(void);

#endif

