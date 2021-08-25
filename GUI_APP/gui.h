#ifndef _GUI_APP_H
#define _GUI_APP_H

#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

//LVGL显示界面相关宏
#define isit_show_pwd           (1<<0) //登录是否需要密码
#define isit_show_clock         (1<<1) //退出显示钟表


typedef struct LV_STATUS{
	lv_obj_t *clock;            //钟表界面
	lv_obj_t *inputpwd;			//密码输入界面
	lv_obj_t *mainwindow;		//主界面
	lv_obj_t *now; 				//当前界面
	uint8_t isit_show;			//是否显示标志位
}lv_status;

extern lv_status lv_sta;

void lv_gui_init(void);
void analog_update_time(uint8_t Hour,uint8_t Minute,uint8_t Second);//钟表时间更新接口
void lv_change_face(lv_obj_t *lv_now,lv_obj_t *lv_next);
void lv_tab2_add_listbtn(uint8_t dev_num);
void tab2_dev_ser_add_data(uint8_t *data);
void send_messagebox(char *mesg);

#endif
