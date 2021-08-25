#include "main.h"

//LV_FONT_DECLARE(font_chinese);
//static lv_style_t font_chinese_style;
LV_IMG_DECLARE(watch_bg);
LV_IMG_DECLARE(hour);
LV_IMG_DECLARE(minute);
LV_IMG_DECLARE(second);

static char *tab2_dev_list[10]={"flame","distance","toxic gas"};             //tab2设备列表
static char *tab3_set_list[10]={"SYSTEM","WIFI","HOME LOCK","TIME SET","DATE SET","CHANGE ID_CARD"};		//tab3设置列表

static char *settime23   ="0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23";
static char *settime59   ="0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59";
static char *setdateyear ="2020\n2021\n2022\n2023\n2024\n2025\n2026\n2027\n2028\n2029";
static char *setdatemonth="1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12";
static char *setdatedata ="1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31";
static char *setdateweek ="1\n2\n3\n4\n5\n6\n7";
//界面状态全局变量
lv_status lv_sta={NULL,NULL,NULL,NULL,0};
//主界面相关变量
static lv_obj_t *tab1;
static lv_obj_t *calendar;
static lv_calendar_date_t today;
static lv_obj_t *tab2;
static lv_obj_t *tab2_list;
static lv_obj_t *tab2_list_btn;
static lv_obj_t *tab3;
static lv_obj_t *tab3_list;
static lv_obj_t *tab3_list_btn;
/******************tab2/3 list对话框**********************/
static lv_obj_t *listpage=NULL;
static lv_obj_t *listbtn1;
static lv_obj_t *listbtn2;
static uint32_t tab_list_page_now=0;               //保存当前显示哪个对话框 这个值要大于 _LV_EVENT_LAST
/***************************一些公用函数声明*******************************/
static void tab3_list_handler(lv_obj_t * obj, lv_event_t event);
static void tab2_list_handler(lv_obj_t * obj, lv_event_t event);
static void ta_event_cb(lv_obj_t * ta_local, lv_event_t e);
/******************31**************************/
static lv_obj_t *tab2_dev1_chart;
static lv_chart_series_t *tab2_dev1_ser1;
/******************32**************************/
static lv_obj_t *tab2_dev2_chart;
static lv_chart_series_t *tab2_dev2_ser1;
/******************33**************************/
static lv_obj_t *tab2_dev3_chart;
static lv_chart_series_t *tab2_dev3_ser1;
/******************41**************************/
static lv_obj_t *tab3_set1_cb;
static lv_obj_t *tab3_set1_cb1;
/******************42***************************/
static lv_obj_t *tab3_set2_ta1;
static lv_obj_t *tab3_set2_ta2;
/******************43***************************/
static lv_obj_t *tab3_set3_ta1;
/****************44**45*************************/
static lv_obj_t *roller1;
static lv_obj_t *roller2;
static lv_obj_t *roller3;
static lv_obj_t *roller4;
/****************46******************************/
static lv_obj_t *tab3_set5_tabel;
//公用键盘
static lv_obj_t * kb;

//钟表相关变量
static lv_obj_t * img;
static lv_obj_t * lvMinute;
static lv_obj_t * lvHour;
static lv_obj_t * lvSecond ;

//密码输入相关变量
static lv_obj_t *input_password;
static uint8_t wifi_name[16]={0};
static uint8_t wifi_pwd[16]={0};
/********************************************************************************************************/

static void mbox_create(char *msg)
{
	static const char * btns2[] = {"Ok",""};

	lv_obj_t* mbox = lv_msgbox_create(lv_scr_act(), NULL);
    lv_msgbox_set_text(mbox, msg);
    lv_msgbox_add_btns(mbox, btns2);
    lv_msgbox_stop_auto_close(mbox);
	lv_msgbox_set_anim_time(mbox, 100);
    lv_obj_set_pos(mbox, 0, 200);
	
}
/********************************************************************************************************/
static void kb_event_cb(lv_obj_t * keyboard, lv_event_t e)
{
	int i;
    lv_keyboard_def_event_cb(kb, e);
    if(e == LV_EVENT_CANCEL) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_del(kb);
        kb = NULL;
    }
	else if(e==LV_EVENT_APPLY){
		if(lv_sta.now==lv_sta.inputpwd) //如果当前界面是密码输入界面
		{
			const char * txt = lv_textarea_get_text(input_password);
			for(i=0;i<8;i++)
			{
				if(txt[i]-'0'!=PASSWORD[i])
					break;
			}
			if(i==8){
				lv_textarea_set_text(input_password,"");                //初始内容为空
				lv_change_face(lv_sta.now,lv_sta.mainwindow);
			}else{
				mbox_create("password\nerror");
			}
		}
	}
}

void kb_create(lv_obj_t *lvobj)
{
    kb = lv_keyboard_create(lv_scr_act(), NULL);
    lv_keyboard_set_cursor_manage(kb, true);
    lv_obj_set_event_cb(kb, kb_event_cb);
    lv_keyboard_set_textarea(kb, lvobj);
}
/*********************************************主界面********************************************************/
static void listpage_btn_handler(lv_obj_t * obj, lv_event_t event)      //列表对话框确认取消键回调
{
    if(event == LV_EVENT_CLICKED) {
        if(obj==listbtn1)
		{
			xEventGroupClearBits(event_group,EVENT_GROUP_NRF_CHACK);
			if(tab_list_page_now==46){
				xEventGroupClearBits(event_group,EVENT_GROUP_RFID_CARD_CHANGE);
				vTaskResume(app_task_rfid_hander);
				rfid_change_flag=0;
			}
			lv_obj_del(listpage);
			listpage=NULL;
			tab_list_page_now=0;
		}
		else if(obj==listbtn2)
		{
			if(tab_list_page_now>=40&&tab_list_page_now<50){       //判断是哪个tab的list-btn 以便调用相应回调函数
				lv_event_send_func(tab3_list_handler,NULL,tab_list_page_now,NULL);   //点击ok将当前的对话框序号发送给回调函数
			}
			if(tab_list_page_now>=30&&tab_list_page_now<40){
				lv_event_send_func(tab2_list_handler,NULL,tab_list_page_now,NULL);   //点击ok将当前的对话框序号发送给回调函数
			}
		}	
    }
}

static void lv_list_page_create(lv_obj_t *tab,uint8_t setnum)            //列表对话框创建
{
	if(listpage==NULL)
	{
		lv_obj_t *lable;
		tab_list_page_now=setnum;                    //设置当前是哪个list的哪个btn
		listpage=lv_obj_create(tab, NULL);
		lv_obj_set_size(listpage, 300, 240);
		lv_obj_align(listpage, NULL, LV_ALIGN_CENTER, 0 ,-60);
		listbtn1=lv_btn_create(listpage,NULL);        //取消按钮
		lv_obj_set_size(listbtn1, 80, 40);
		lv_obj_align(listbtn1, NULL, LV_ALIGN_CENTER, -100 ,80);
		lable=lv_label_create(listbtn1,NULL);
		lv_label_set_text(lable,"Cancel");
		lv_obj_set_event_cb(listbtn1, listpage_btn_handler);
		listbtn2=lv_btn_create(listpage,NULL);        //确认按钮
		lv_obj_set_size(listbtn2, 80, 40);
		lv_obj_align(listbtn2, NULL, LV_ALIGN_CENTER, 100 ,80);
		lable=lv_label_create(listbtn2,NULL);
		lv_label_set_text(lable,"Ok");
		lv_obj_set_event_cb(listbtn2, listpage_btn_handler);
		switch(setnum)
		{
			case 31:
				tab2_dev1_chart = lv_chart_create(listpage, NULL);
				lv_obj_set_size(tab2_dev1_chart, 240, 150);
				lv_obj_align(tab2_dev1_chart, NULL, LV_ALIGN_CENTER, 0, -25);
				lv_chart_set_type(tab2_dev1_chart, LV_CHART_TYPE_LINE);
				tab2_dev1_ser1 = lv_chart_add_series(tab2_dev1_chart, LV_COLOR_RED);
				lv_chart_set_y_range(tab2_dev1_chart,LV_CHART_AXIS_PRIMARY_Y,0,4000);
				break;
			case 32:
				tab2_dev2_chart = lv_chart_create(listpage, NULL);
				lv_obj_set_size(tab2_dev2_chart, 240, 150);
				lv_obj_align(tab2_dev2_chart, NULL, LV_ALIGN_CENTER, 0, -25);
				lv_chart_set_type(tab2_dev2_chart, LV_CHART_TYPE_LINE);
				tab2_dev2_ser1 = lv_chart_add_series(tab2_dev2_chart, LV_COLOR_RED);
				lv_chart_set_y_range(tab2_dev2_chart,LV_CHART_AXIS_PRIMARY_Y,0,4000);
				break;
			case 33:
				tab2_dev3_chart = lv_chart_create(listpage, NULL);
				lv_obj_set_size(tab2_dev3_chart, 240, 150);
				lv_obj_align(tab2_dev3_chart, NULL, LV_ALIGN_CENTER, 0, -25);
				lv_chart_set_type(tab2_dev3_chart, LV_CHART_TYPE_LINE);
				tab2_dev3_ser1 = lv_chart_add_series(tab2_dev3_chart, LV_COLOR_RED);
				lv_chart_set_y_range(tab2_dev3_chart,LV_CHART_AXIS_PRIMARY_Y,0,4000);
				break;
			case 41:
				tab3_set1_cb = lv_checkbox_create(listpage, NULL);
				lv_checkbox_set_text(tab3_set1_cb, "Need login password");
				lv_obj_align(tab3_set1_cb , NULL, LV_ALIGN_IN_TOP_LEFT, 60 ,60);
				tab3_set1_cb1 = lv_checkbox_create(listpage, NULL);
				lv_checkbox_set_text(tab3_set1_cb1, "Exit display clock");
				lv_obj_align(tab3_set1_cb1, NULL, LV_ALIGN_IN_TOP_LEFT, 60 ,100);
				if(lv_sta.isit_show & isit_show_pwd)           //是否显示登录密码界面
					lv_checkbox_set_checked(tab3_set1_cb,true);
				else
					lv_checkbox_set_checked(tab3_set1_cb,false);
				if(lv_sta.isit_show & isit_show_clock)         //是否显示关屏钟表
					lv_checkbox_set_checked(tab3_set1_cb1,true);
				else
					lv_checkbox_set_checked(tab3_set1_cb1,false);
				break;
			case 42:
				tab3_set2_ta1=lv_textarea_create(listpage,NULL);
				lv_textarea_set_text(tab3_set2_ta1,	(char *)WIFI_NAME);
				lv_textarea_set_cursor_hidden(tab3_set2_ta1, true);
				lv_obj_set_width(tab3_set2_ta1, 260);
				lv_textarea_set_one_line(tab3_set2_ta1, true);
				lv_textarea_set_max_length(tab3_set2_ta1, 16);
				lv_obj_align(tab3_set2_ta1, NULL, LV_ALIGN_CENTER, 0 ,-30);
				lv_obj_set_event_cb(tab3_set2_ta1, ta_event_cb);
				tab3_set2_ta2=lv_textarea_create(listpage,NULL);
				lv_textarea_set_text(tab3_set2_ta2, (char *)WIFI_PWD);
				lv_textarea_set_cursor_hidden(tab3_set2_ta2, true);
				lv_obj_set_width(tab3_set2_ta2, 260);
				lv_textarea_set_one_line(tab3_set2_ta2, true);
				lv_textarea_set_max_length(tab3_set2_ta2, 16);
				lv_obj_align(tab3_set2_ta2, NULL, LV_ALIGN_CENTER, 0 ,10);
				lv_obj_set_event_cb(tab3_set2_ta2, ta_event_cb);
				break;
			case 43:
				tab3_set3_ta1=lv_textarea_create(listpage,NULL);
				lv_textarea_set_text(tab3_set3_ta1, "password");
				lv_textarea_set_cursor_hidden(tab3_set3_ta1, true);
				lv_obj_set_width(tab3_set3_ta1, 260);
				lv_textarea_set_one_line(tab3_set3_ta1, true);
				lv_textarea_set_accepted_chars(tab3_set3_ta1, "0123456789");
				lv_textarea_set_max_length(tab3_set3_ta1, 8);
				lv_obj_align(tab3_set3_ta1, NULL, LV_ALIGN_CENTER, 0 ,-20);
				lv_obj_set_event_cb(tab3_set3_ta1, ta_event_cb);
				break;
			case 44:
				roller1 = lv_roller_create(listpage, NULL);
				lv_roller_set_options(roller1,settime23,LV_ROLLER_MODE_INFINITE);
				lv_roller_set_visible_row_count(roller1, 3);
				lv_obj_align(roller1, NULL, LV_ALIGN_CENTER, -60, -20);
				roller2 = lv_roller_create(listpage, NULL);
				lv_roller_set_options(roller2,settime59,LV_ROLLER_MODE_INFINITE);
				lv_roller_set_visible_row_count(roller2, 3);
				lv_obj_align(roller2, NULL, LV_ALIGN_CENTER,  0,  -20);
				roller3 = lv_roller_create(listpage, NULL);
				lv_roller_set_options(roller3,settime59,LV_ROLLER_MODE_INFINITE);
				lv_roller_set_visible_row_count(roller3, 3);
				lv_obj_align(roller3, NULL, LV_ALIGN_CENTER, 60,  -20);
				break;
			case 45:
				roller1 = lv_roller_create(listpage, NULL);
				lv_roller_set_options(roller1,setdateyear,LV_ROLLER_MODE_INFINITE);
				lv_roller_set_visible_row_count(roller1, 3);
				lv_obj_align(roller1, NULL, LV_ALIGN_CENTER, -80, -20);
				roller2 = lv_roller_create(listpage, NULL);
				lv_roller_set_options(roller2,setdatemonth,LV_ROLLER_MODE_INFINITE);
				lv_roller_set_visible_row_count(roller2, 3);
				lv_obj_align(roller2, NULL, LV_ALIGN_CENTER,  -20,  -20);
				roller3 = lv_roller_create(listpage, NULL);
				lv_roller_set_options(roller3,setdatedata,LV_ROLLER_MODE_INFINITE);
				lv_roller_set_visible_row_count(roller3, 3);
				lv_obj_align(roller3, NULL, LV_ALIGN_CENTER, 40,  -20);
				roller4 = lv_roller_create(listpage, NULL);
				lv_roller_set_options(roller4,setdateweek,LV_ROLLER_MODE_INFINITE);
				lv_roller_set_visible_row_count(roller4, 3);
				lv_obj_align(roller4, NULL, LV_ALIGN_CENTER, 100,  -20);
				break;
			case 46:
				tab3_set5_tabel= lv_label_create(listpage, NULL);  //在ta上方显示Password
				lv_label_set_text(tab3_set5_tabel, "PUT YOUR CARD");
				lv_obj_align(tab3_set5_tabel, input_password, LV_ALIGN_OUT_TOP_LEFT, 20, 0);
				break;
			default:
				break;
		}
	}
}
static void tab2_list_handler(lv_obj_t * obj, lv_event_t event)    //tab2  list 按钮回调
{
    if(event == LV_EVENT_CLICKED) {
		//vTaskResume(app_task_nrf24l01_hander);
		xEventGroupSetBits(event_group,EVENT_GROUP_NRF_CHACK);
        if(strcmp(lv_list_get_btn_text(obj),tab2_dev_list[0])==0)
		{
			lv_list_page_create(tab2,31);
		}
		else if(strcmp(lv_list_get_btn_text(obj),tab2_dev_list[1])==0)
		{
			lv_list_page_create(tab2,32);
		}
		else if(strcmp(lv_list_get_btn_text(obj),tab2_dev_list[2])==0)
		{
			lv_list_page_create(tab2,33);
		}
    }
	else if(event==31)
	{
		lv_obj_del(listpage);          //保存完成后删除对话框
		listpage=NULL;
		tab_list_page_now=0;
		//vTaskSuspend(app_task_nrf24l01_hander);
		xEventGroupClearBits(event_group,EVENT_GROUP_NRF_CHACK);
	}
	else if(event==32)
	{
		lv_obj_del(listpage);          //保存完成后删除对话框
		listpage=NULL;
		tab_list_page_now=0;
		//vTaskSuspend(app_task_nrf24l01_hander);
		xEventGroupClearBits(event_group,EVENT_GROUP_NRF_CHACK);
	}
	else if(event==33)
	{
		lv_obj_del(listpage);          //保存完成后删除对话框
		listpage=NULL;
		tab_list_page_now=0;
		//vTaskSuspend(app_task_nrf24l01_hander);
		xEventGroupClearBits(event_group,EVENT_GROUP_NRF_CHACK);
	}
}

static void tab3_list_handler(lv_obj_t * obj, lv_event_t event)    //tab3  list  按钮回调
{
	eeprom_t et;
    if(event == LV_EVENT_CLICKED) {
        if(strcmp(lv_list_get_btn_text(obj),tab3_set_list[0])==0)
		{
			lv_list_page_create(tab3,41);
		}
		else if(strcmp(lv_list_get_btn_text(obj),tab3_set_list[1])==0)
		{
			lv_list_page_create(tab3,42);
		}
		else if(strcmp(lv_list_get_btn_text(obj),tab3_set_list[2])==0)
		{
			lv_list_page_create(tab3,43);
		}
		else if(strcmp(lv_list_get_btn_text(obj),tab3_set_list[3])==0)
		{
			lv_list_page_create(tab3,44);
		}
		else if(strcmp(lv_list_get_btn_text(obj),tab3_set_list[4])==0)
		{
			lv_list_page_create(tab3,45);
		}
		else if(strcmp(lv_list_get_btn_text(obj),tab3_set_list[5])==0)
		{
			lv_list_page_create(tab3,46);
			vTaskSuspend(app_task_rfid_hander);
			xEventGroupSetBits(event_group,EVENT_GROUP_RFID_CARD_CHANGE);
		}
    }
	
	else if(event==41)               //41表示   tab3 set1
	{
		if(lv_checkbox_is_checked(tab3_set1_cb)){
			lv_sta.isit_show |= isit_show_pwd;		//标志位置一
		}else{
			lv_sta.isit_show &= ~isit_show_pwd;		//标志位清零
		}
		if(lv_checkbox_is_checked(tab3_set1_cb1)){
			lv_sta.isit_show |= isit_show_clock;		//标志位置一
		}else{
			lv_sta.isit_show &= ~isit_show_clock;		//标志位清零
		}
		et.addr=8;                                  //lv_sta.isit_show存放地址
		et.data=&lv_sta.isit_show;
		et.len=1;
        xQueueSend(queue_eeprom,&et,100);
		lv_obj_del(listpage);          //保存完成后删除对话框
		listpage=NULL;
		tab_list_page_now=0;
	}
	else if(event==42)              
	{
		const char *name=lv_textarea_get_text(tab3_set2_ta1);
		const char *pwd =lv_textarea_get_text(tab3_set2_ta2);
		strcpy((char *)wifi_name,name);
		strcpy((char *)wifi_pwd ,pwd);
		//printf("%s,%s\n",wifi_name,wifi_pwd);
		//保存wifi名
		et.addr=wifi_name_addr;
		et.data=wifi_name;
		et.len=8;
        xQueueSend(queue_eeprom,&et,100);
		et.addr=wifi_name_addr+8;
		et.data=&wifi_name[7];
		et.len=8;
        xQueueSend(queue_eeprom,&et,100);
		//保存WiFi密码
		et.addr=wifi_pwd_addr;
		et.data=wifi_pwd;
		et.len=8;
        xQueueSend(queue_eeprom,&et,100);
		et.addr=wifi_pwd_addr+8;
		et.data=&wifi_pwd[7];
		et.len=8;
        xQueueSend(queue_eeprom,&et,100);
		lv_obj_del(listpage);          //保存完成后删除对话框
		listpage=NULL;
		tab_list_page_now=0;
	}
	else if(event==43)
	{
		const char *pwd=lv_textarea_get_text(tab3_set3_ta1);
		if(strlen(pwd)<8){
			mbox_create("password\nis too short");
		}else{
			for(int i=0;i<8;i++)
			{
				PASSWORD[i]=pwd[i]-'0';
			}
			et.addr=wifi_name_addr;
			et.data=PASSWORD;
			et.len=8;
			xQueueSend(queue_eeprom,&et,100);
			lv_obj_del(listpage);          //保存完成后删除对话框
			listpage=NULL;
			tab_list_page_now=0;
		}
	}
	else if(event==44)
	{
		RTC_TimeStructure.RTC_Hours   = DEC2BCD(lv_roller_get_selected(roller1));
		RTC_TimeStructure.RTC_Minutes = DEC2BCD(lv_roller_get_selected(roller2));
		RTC_TimeStructure.RTC_Seconds = DEC2BCD(lv_roller_get_selected(roller3));
		RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
		lv_obj_del(listpage);          //保存完成后删除对话框
		listpage=NULL;
		tab_list_page_now=0;
	}
	else if(event==45)
	{
		RTC_DateStructure.RTC_Year    = DEC2BCD(lv_roller_get_selected(roller1)+20);
		RTC_DateStructure.RTC_Month   = DEC2BCD(lv_roller_get_selected(roller2)+1);
		RTC_DateStructure.RTC_Date    = DEC2BCD(lv_roller_get_selected(roller3)+1);
		RTC_DateStructure.RTC_WeekDay = DEC2BCD(lv_roller_get_selected(roller4)+1);
		RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
		lv_obj_del(listpage);          //保存完成后删除对话框
		listpage=NULL;
		tab_list_page_now=0;
	}
	else if(event==46)
	{
		xEventGroupClearBits(event_group,EVENT_GROUP_RFID_CARD_CHANGE);
		vTaskResume(app_task_rfid_hander);
		rfid_change_flag=0;
		lv_obj_del(listpage);          //保存完成后删除对话框
		listpage=NULL;
		tab_list_page_now=0;
	}
}

static void ta_event_cb(lv_obj_t * ta_local, lv_event_t e)     
{
	if(e == LV_EVENT_CLICKED && kb == NULL) {
		kb_create(ta_local);
	}
}

void lv_ex_tabview(lv_obj_t *tabview)
{
	lv_obj_t * label;
	//lv_obj_add_style(tabview, LV_LABEL_PART_MAIN, &font_chinese_style);
	lv_tabview_set_anim_time(tabview, 60);
	//lv_tabview_set_btns_pos(tabview, LV_TABVIEW_TAB_POS_NONE);//不显示顶部btns
    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    tab1 = lv_tabview_add_tab(tabview, "This dev");
    tab2 = lv_tabview_add_tab(tabview, "Online dev");
    tab3 = lv_tabview_add_tab(tabview, "SET");
	
	/***tab1***/
    calendar = lv_calendar_create(tab1, NULL);
	lv_obj_set_size(calendar, 250, 250);
	lv_obj_align(calendar, NULL, LV_ALIGN_CENTER, 0, 0);
	//lv_obj_set_style_local_text_font(calendar, LV_CALENDAR_PART_DATE, LV_STATE_DEFAULT, lv_theme_get_font_small());
	
	/***tab2***/
    tab2_list = lv_list_create(tab2, NULL);
    lv_obj_set_size(tab2_list, 300, 380);
    lv_obj_align(tab2_list, NULL, LV_ALIGN_CENTER, 0, 0);
	tab2_list_btn = lv_list_add_btn(tab2_list, LV_SYMBOL_REFRESH,"Refresh");
	lv_obj_set_event_cb(tab2_list_btn, tab2_list_handler);
	
	/***tab3***/
    tab3_list = lv_list_create(tab3, NULL);
    lv_obj_set_size(tab3_list, 300, 380);
    lv_obj_align(tab3_list, NULL, LV_ALIGN_CENTER, 0, 0);
	tab3_list_btn = lv_list_add_btn(tab3_list,LV_SYMBOL_EYE_OPEN,tab3_set_list[0]); 
	lv_obj_set_event_cb(tab3_list_btn, tab3_list_handler);
	tab3_list_btn = lv_list_add_btn(tab3_list,LV_SYMBOL_WIFI,tab3_set_list[1]);
	lv_obj_set_event_cb(tab3_list_btn, tab3_list_handler); 
	tab3_list_btn = lv_list_add_btn(tab3_list,LV_SYMBOL_HOME,tab3_set_list[2]);
	lv_obj_set_event_cb(tab3_list_btn, tab3_list_handler); 
	tab3_list_btn = lv_list_add_btn(tab3_list,LV_SYMBOL_SETTINGS,tab3_set_list[3]);
	lv_obj_set_event_cb(tab3_list_btn, tab3_list_handler); 
	tab3_list_btn = lv_list_add_btn(tab3_list,LV_SYMBOL_SETTINGS,tab3_set_list[4]);
	lv_obj_set_event_cb(tab3_list_btn, tab3_list_handler); 
	tab3_list_btn = lv_list_add_btn(tab3_list,LV_SYMBOL_SETTINGS,tab3_set_list[5]);
	lv_obj_set_event_cb(tab3_list_btn, tab3_list_handler); 
	
}
/**********************************************密码输入界面*********************************************************/

void password_input(lv_obj_t *pwd_input)
{
	
	lv_obj_set_size(pwd_input, LV_HOR_RES_MAX, LV_VER_RES_MAX);//设置为屏幕大小
	input_password  = lv_textarea_create(pwd_input, NULL);     
	lv_obj_set_size(input_password, 200, 50);                 //设置大小
	lv_obj_align(input_password, NULL, LV_ALIGN_CENTER, 0, -50); //设置位置
	lv_textarea_set_text(input_password, "");                //初始内容为空
    lv_textarea_set_pwd_mode(input_password, true);          //设置为密码模式
    lv_textarea_set_one_line(input_password, true);           //只显示为1行
	lv_textarea_set_cursor_hidden(input_password, true);      //设置光标隐藏
	
	lv_obj_t * pwd_label = lv_label_create(pwd_input, NULL);  //在ta上方显示Password
    lv_label_set_text(pwd_label, "Password:");
    lv_obj_align(pwd_label, input_password, LV_ALIGN_OUT_TOP_LEFT, 0, 0);
	
	lv_obj_set_event_cb(input_password, ta_event_cb);
}

/*************************************************钟表界面******************************************************/
void analog(lv_obj_t *central)
{   
	lv_obj_set_size(central, LV_HOR_RES_MAX, LV_VER_RES_MAX);
    lv_obj_set_pos(central,  0, 0);
	lv_obj_set_style_local_bg_color(central, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
 

    img = lv_img_create(central,NULL);
    lv_img_set_src(img, &watch_bg);               //钟表背景图


    lv_obj_set_size(img, 200, 200);
    lv_obj_set_auto_realign(img, true);
    lv_obj_align(img, central, LV_ALIGN_CENTER, 0, 0);


    lvHour = lv_img_create(central,NULL);
    lv_img_set_src( lvHour, &hour);               //时针图片
    lv_obj_align(  lvHour, img,LV_ALIGN_CENTER, 0, 0);
    lv_img_set_angle(lvHour, 0);                  //设置旋转角度

    lvMinute = lv_img_create(central,NULL);        
    lv_img_set_src( lvMinute, &minute);           //分针图片
    lv_obj_align(  lvMinute, img,LV_ALIGN_CENTER, 0, 0);
    lv_img_set_angle(lvHour,0);                   //设置旋转角度

    lvSecond = lv_img_create(central,NULL);
    lv_img_set_src( lvSecond, &second);           //秒针图片
    lv_obj_align(  lvSecond, img,LV_ALIGN_CENTER, 0, 0);
    lv_img_set_angle(lvSecond,0);               //设置旋转角度
	
}

void analog_update_time(uint8_t Hour,uint8_t Minute,uint8_t Second)
{
	uint16_t h;
    if(lvHour !=NULL)
    {
       // Hour,Minute,Second;
        h = Hour * 300 + Minute / 12 % 12 * 60;
        lv_img_set_angle(lvHour, h);
        lv_img_set_angle(lvMinute, Minute * 6 * 10);
        lv_img_set_angle(lvSecond, Second * 6 * 10);
    }
}

/************************************************************************************************************/
//传入值           			 当前界面         下一界面
void lv_change_face(lv_obj_t *lv_now,lv_obj_t *lv_next)
{
	if(kb!=NULL){               //公用键盘切换窗口要删除
		lv_obj_del(kb);
        kb = NULL;
	}
	if(lv_next==lv_sta.mainwindow)
	{
		RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure); 
		today.year  = BCD2DEC(RTC_DateStructure.RTC_Year)+2000;
		today.month = BCD2DEC(RTC_DateStructure.RTC_Month);
		today.day   = BCD2DEC(RTC_DateStructure.RTC_Date);
		lv_calendar_set_today_date(calendar, &today);
		lv_calendar_set_showed_date(calendar, &today);
	}
	lv_obj_set_hidden(lv_now, true); //隐藏上一界面
	lv_obj_set_hidden(lv_next, false);//显示下一界面
	lv_sta.now=lv_next;
}

void lv_gui_init(void)
{
	lv_sta.clock      = lv_obj_create(lv_scr_act(), NULL);
	lv_sta.inputpwd   = lv_obj_create(lv_scr_act(), NULL);
	lv_sta.mainwindow = lv_tabview_create(lv_scr_act(), NULL);
	
	analog(lv_sta.clock);					//初始化所有界面
	lv_obj_set_hidden(lv_sta.clock, true);   //隐藏所有界面
	password_input(lv_sta.inputpwd);
	lv_obj_set_hidden(lv_sta.inputpwd, true);
	lv_ex_tabview(lv_sta.mainwindow);
	lv_obj_set_hidden(lv_sta.mainwindow, true);
	
	if(lv_sta.isit_show&isit_show_clock){
		lv_obj_set_hidden(lv_sta.clock, false);//界面取消隐藏
		lv_sta.now=lv_sta.clock;
	}else{
		if(lv_sta.isit_show&isit_show_pwd){
			lv_obj_set_hidden(lv_sta.inputpwd, false);//界面取消隐藏
			lv_sta.now=lv_sta.inputpwd;
		}else{
			lv_obj_set_hidden(lv_sta.mainwindow, false);//界面取消隐藏
			lv_sta.now=lv_sta.mainwindow;
		}
	}
}


void lv_tab2_add_listbtn(uint8_t dev_num)
{
	tab2_list_btn = lv_list_add_btn(tab2_list, LV_SYMBOL_DIRECTORY, tab2_dev_list[dev_num]);
	lv_obj_set_event_cb(tab2_list_btn, tab2_list_handler);
}

void tab2_dev_ser_add_data(uint8_t *data)
{
	if(tab_list_page_now==31){
		lv_chart_set_next(tab2_dev1_chart, tab2_dev1_ser1, data[3]|(data[4]<<8));
	}
	else if(tab_list_page_now==32){
		lv_chart_set_next(tab2_dev2_chart, tab2_dev2_ser1, data[7]|(data[8]<<8));
	}
	else if(tab_list_page_now==33){
		lv_chart_set_next(tab2_dev3_chart, tab2_dev3_ser1, data[5]|(data[6]<<8));
	}
}

void send_messagebox(char *mesg)
{
	mbox_create(mesg);
}








