#include "hc_sr04.h"

void sr04_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	//�򿪶˿�B��Ӳ��ʱ�ӣ����ǹ���
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//�򿪶˿�E��Ӳ��ʱ�ӣ����ǹ���
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6; 	//6������
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//���٣��ٶ�Խ�ߣ���ӦԽ�죬���ǹ��Ļ����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��ʹ������������
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6; 	//6������
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//���٣��ٶ�Խ�ߣ���ӦԽ�죬���ǹ��Ļ����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��ʹ������������
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	//ֻҪ�����ģʽ���϶����г�ʼ��ƽ��״̬���������豸��˵����
	PBout(6)=0;

}

int32_t sr04_get_distance(void)
{
	uint32_t t=0;
	PBout(6)=1;
	delay_us(15);
	PBout(6)=0;	

	//�ȴ������ź�,���ȴ��ɹ���PE6����Ϊ�ߵ�ƽ����������ѭ��
	while(PEin(6)==0);
	
	//�����ߵ�ƽ�ĳ���ʱ��
	while(PEin(6))
	{
		t++;
		if(t>2700)    //�������볬��
			return -1;
		delay_us(9);		//�ж��ٸ�9us �������ж��ٸ�3mm
	}
	//��Ϊ�������Ĵ���ʱ���Ƿ���ʱ��+����ʱ�䣬������Ҫ����/2
	t=t/2;
	return 3*t;
}





