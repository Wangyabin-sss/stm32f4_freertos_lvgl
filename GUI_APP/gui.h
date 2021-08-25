#ifndef _GUI_APP_H
#define _GUI_APP_H

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

//LVGL��ʾ������غ�
#define isit_show_pwd           (1<<0) //��¼�Ƿ���Ҫ����
#define isit_show_clock         (1<<1) //�˳���ʾ�ӱ�


typedef struct LV_STATUS{
	lv_obj_t *clock;            //�ӱ����
	lv_obj_t *inputpwd;			//�����������
	lv_obj_t *mainwindow;		//������
	lv_obj_t *now; 				//��ǰ����
	uint8_t isit_show;			//�Ƿ���ʾ��־λ
}lv_status;

extern lv_status lv_sta;

void lv_gui_init(void);
void analog_update_time(uint8_t Hour,uint8_t Minute,uint8_t Second);//�ӱ�ʱ����½ӿ�
void lv_change_face(lv_obj_t *lv_now,lv_obj_t *lv_next);
void lv_tab2_add_listbtn(uint8_t dev_num);
void tab2_dev_ser_add_data(uint8_t *data);
void send_messagebox(char *mesg);

#endif
