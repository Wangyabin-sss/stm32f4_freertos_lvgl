#include "Sys.h"

#if OS_SUPPORT
#include "FreeRTOS.h"				
#include "task.h"

void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;		//ϵͳ��ʱ��������ֵ    	 
	ticks=nus*(SystemCoreClock/1000000);//��Ҫ�Ľ����� 
	told=SysTick->VAL;        			//�ս���ʱ�ļ�����ֵ
	
	/* ���������[��ѡ,�ᵼ�¸����ȼ������޷���ռ��ǰ���񣬵��ܹ���ߵ�ǰ����ʱ��ľ�ȷ��] */
	vTaskSuspendAll();	
	
	while(1)
	{
		tnow=SysTick->VAL;
		
		if(tnow!=told)
		{	 
			/* SYSTICK��һ���ݼ��ļ����� */
			if(tnow<told)
				tcnt+=told-tnow;		
			else 
				tcnt+=reload-tnow+told;	  
			
			told=tnow;
			
			/* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳���*/
			if(tcnt>=ticks)
				break;			
		}  
	}

	/* �ָ�������[��ѡ] */
	xTaskResumeAll();
}

void delay_ms(uint32_t nms)
{	
	vTaskDelay(nms);
}
#else
void delay_ms(uint16_t nms)
{
	SysTick->CTRL = 0; 			   //�رն�ʱ��
	SysTick->LOAD = SystemCoreClock/1000-1;  //������װ��ֵ
	SysTick->VAL=0;          //�������ֵ
	SysTick->CTRL = 5; 		 //������ʱ��
	while(nms--)
	{
		while((SysTick->CTRL&0x10000)==0); //���flagλ����������ʱ��λ����
	}
	SysTick->CTRL = 0;
}


void delay_us(uint16_t nus)
{
	SysTick->CTRL = 0; 			
	SysTick->LOAD = SystemCoreClock/1000000*nus-1; //168*nus < 2^24
	SysTick->VAL=0;
	SysTick->CTRL = 5; 		
	while((SysTick->CTRL&0x10000)==0);
	SysTick->CTRL = 0;
}
#endif




