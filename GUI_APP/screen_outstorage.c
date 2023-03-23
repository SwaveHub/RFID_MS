/**
  ******************************************************************************
  * @file    screen_outstorage.c
  * @author  SwaveYu
  * @version V1.0
  * @date    2023-03-23
  * @brief   lvgl screen_outstorage create
  *	    �޸ļ�¼ :
  *		�汾��  ����              ����             ˵��
  *         V1.0    2023-03-23  SwaveYu     ��ʽ����
  ******************************************************************************
  * @attention
  *
  *
  *
  *
  *
  ******************************************************************************
  */

/******************************** ͷ�ļ� *************************************/
#include "demo.h"
#include "screen_outstorage.h"
#include "screen_home.h"

/****************************** ˽�к궨�� ***********************************/
//�Ƿ������б���ĵ��������Ϊ����,Ĭ����������µ�
#define DDLIST_DIRECTION_UP_EN 0

/******************************* ȫ�ֱ��� ************************************/

/***************************** ˽��ȫ�ֱ��� **********************************/
static lv_style_t my_style_btn_release;//��ť�ͷ�״̬�µ���ʽ
static lv_style_t my_style_btn_press;//��ť����ʱ����ʽ

static lv_obj_t *label_outstorage;
static lv_obj_t *label_time;
static lv_obj_t *btn_home, *btn_home_label;
static lv_obj_t *btn_upload, *btn_upload_label;

static lv_obj_t *label_mode;
static lv_obj_t *btn_ok, *btn_ok_label;
static lv_obj_t *btn_save, *btn_save_label;

static lv_style_t bg_style;
static lv_style_t sel_style;
static lv_obj_t * ddlist_mode;

static lv_style_t cell1_style;
static lv_style_t cell2_style;
static lv_obj_t * table_detail;
static bool is_cell_crop = false;//Ĭ����ʹ���ı��Զ����й��ܵ�
static bool is_cell_merge = false;//�Ƿ�ϲ���Ԫ��,Ĭ�ϲ��ϲ�
#define TABLE_COL_CNT 3 //���������
#define TABLE_ROW_CNT 11 //���������
//ÿһ�еĿ���
static const uint16_t TABLE_COL_WIDTH[TABLE_COL_CNT] = {40,260,120};

/******************************* ˽�к��� ************************************/
//�¼��ص�����
static void btn_upload_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
	}
		break;
	default:
		break;
	}
}

static void btn_home_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
		lv_obj_clean(lv_scr_act());
        screen_home_create();
	}
		break;
	default:
		break;
	}
}

//�¼��ص�����
static void ddlist_event_handler(lv_obj_t * obj,lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
    {
    char buf[32];
    uint16_t selected_index = lv_ddlist_get_selected(obj);//��ȡѡ��ֵ������
    lv_ddlist_get_selected_str(obj,buf,sizeof(buf));//��ȡѡ��ֵ���ı�����
    printf("Option index: %d,Option text:%s\r\n",selected_index,buf);
    }
}

//�������
void lv_screen_outstorage_start()
{
    lv_obj_t* scr = lv_scr_act();//��ȡ��ǰ��Ծ����Ļ����

    //���� label_outstorage ��ǩ,������ʾ label_time ��ǩ�ĳ��ı�ģʽ
    label_outstorage = lv_label_create(scr,NULL);//������ǩ
    lv_label_set_long_mode(label_outstorage,LV_LABEL_LONG_BREAK);//���ó��ı�ģʽ
    //���ÿ���,һ���÷��� lv_label_set_long_mode �ĺ���,���������õ�
    lv_obj_set_width(label_outstorage,150);
    lv_label_set_align(label_outstorage,LV_LABEL_ALIGN_CENTER);//�ı����ж���
    lv_label_set_text(label_outstorage,"OutStorage");//�����ı�
    lv_obj_set_pos(label_outstorage, 165, 10);

    //���� label_time ��ǩ
    static lv_style_t label_text_style;
    lv_style_copy(&label_text_style,&lv_style_plain);
    label_text_style.text.font = &lv_font_roboto_16;
    label_time = lv_label_create(scr,NULL);//������ǩ
    lv_label_set_long_mode(label_time,LV_LABEL_LONG_BREAK);//���ó��ı�ģʽ
    lv_obj_set_width(label_time,100);
    lv_label_set_align(label_time,LV_LABEL_ALIGN_CENTER);//�ı����ж���
    lv_label_set_style(label_time, LV_LABEL_STYLE_MAIN, &label_text_style);
    lv_label_set_text(label_time,"01:23\n2023/3/17");//�����ı�
    lv_obj_set_pos(label_time, 375, 5);

    //���� label_mode ��ǩ
    label_mode = lv_label_create(scr,NULL);//������ǩ
    lv_label_set_long_mode(label_mode,LV_LABEL_LONG_BREAK);//���ó��ı�ģʽ
    lv_obj_set_width(label_mode,80);
    lv_label_set_align(label_mode,LV_LABEL_ALIGN_RIGHT);
    lv_label_set_text(label_mode,"Mode:");//�����ı�
    lv_obj_set_pos(label_mode, 40, 105);

    //1.�ȴ��� 2 ��״̬�°�ť��ʽ
    //1.1 �ͷ�״̬�µ���ʽ
    lv_style_copy(&my_style_btn_release,&lv_style_plain_color);
    //���ô�ɫ�ı���
    my_style_btn_release.body.main_color = LV_COLOR_MAKE(0x1E,0x9F,0xFF);
    my_style_btn_release.body.grad_color = my_style_btn_release.body.main_color;
    my_style_btn_release.body.opa = LV_OPA_COVER;//���ñ���ɫ��ȫ��͸��
    my_style_btn_release.body.radius = LV_RADIUS_CIRCLE;//����Բ�ǰ�ť
    my_style_btn_release.body.shadow.color = LV_COLOR_MAKE(0x1E,0x9F,0xFF);
    my_style_btn_release.body.shadow.type = LV_SHADOW_FULL;//�����ı߶�����Ӱ
    my_style_btn_release.body.shadow.width = 3;//������Ӱ�Ŀ���
    my_style_btn_release.text.color = LV_COLOR_WHITE;
    my_style_btn_release.body.padding.left = 10;//�������ڱ߾�
    my_style_btn_release.body.padding.right = 10;//�������ڱ߾�
    //1.2 ����״̬�µ���ʽ
    lv_style_copy(&my_style_btn_press,&lv_style_plain_color);
    my_style_btn_press.body.opa = LV_OPA_0;//���ñ���ɫ͸��
    my_style_btn_press.body.radius = LV_RADIUS_CIRCLE;//����Բ�ǰ�ť
    //���ñ߿����ɫ
    my_style_btn_press.body.border.color = LV_COLOR_MAKE(0xC9,0xC9,0xC9);
    my_style_btn_press.body.border.part = LV_BORDER_FULL;//�����߿򶼻���
    my_style_btn_press.body.border.width = 2;//���ñ߿�Ŀ���
    my_style_btn_press.body.border.opa = LV_OPA_COVER;//���ñ߿���ȫ��͸��
    my_style_btn_press.text.color = LV_COLOR_BLACK;
    my_style_btn_press.body.padding.left = 10;//�������ڱ߾�
    my_style_btn_press.body.padding.right = 10;//�������ڱ߾�

    //����һ��home��ť
    btn_home = lv_btn_create(scr, NULL);
    //���ð�ť����̬���ͷ�״̬��ʽ
    lv_btn_set_style(btn_home,LV_BTN_STYLE_REL,&my_style_btn_release);
    //���ð�ť����̬�°���״̬��ʽ
    lv_btn_set_style(btn_home,LV_BTN_STYLE_PR,&my_style_btn_press);
    lv_obj_set_size(btn_home, 100, 50);//���ô�С
    lv_obj_set_pos(btn_home,190,750);//��������
    btn_home_label = lv_label_create(btn_home,NULL);//�� btn_home ���� label �Ӷ���
    lv_label_set_text(btn_home_label,"Home");
    lv_obj_set_event_cb(btn_home,btn_home_event_cb);//�����¼��ص�����

    //����һ��upload��ť
    btn_upload = lv_btn_create(scr, NULL);
    //���ð�ť����̬���ͷ�״̬��ʽ
    lv_btn_set_style(btn_upload,LV_BTN_STYLE_REL,&my_style_btn_release);
    //���ð�ť����̬�°���״̬��ʽ
    lv_btn_set_style(btn_upload,LV_BTN_STYLE_PR,&my_style_btn_press);
    lv_obj_set_size(btn_upload, 100, 35);//���ô�С
    lv_obj_set_pos(btn_upload,5,5);//��������
    btn_upload_label = lv_label_create(btn_upload,NULL);//�� btn_upload ���� label �Ӷ���
    lv_label_set_text(btn_upload_label,"Upload");
    lv_obj_set_event_cb(btn_upload,btn_upload_event_cb);//�����¼��ص�����

    //����һ��ok��ť
    btn_ok = lv_btn_create(scr, NULL);
    //���ð�ť����̬���ͷ�״̬��ʽ
    lv_btn_set_style(btn_ok,LV_BTN_STYLE_REL,&my_style_btn_release);
    //���ð�ť����̬�°���״̬��ʽ
    lv_btn_set_style(btn_ok,LV_BTN_STYLE_PR,&my_style_btn_press);
    lv_obj_set_size(btn_ok, 60, 30);//���ô�С
    lv_obj_set_pos(btn_ok,270,100);//��������
    btn_ok_label = lv_label_create(btn_ok,NULL);//�� btn_ok ���� label �Ӷ���
    lv_label_set_text(btn_ok_label,"OK");

    //����һ��save��ť
    btn_save = lv_btn_create(scr, NULL);
    //���ð�ť����̬���ͷ�״̬��ʽ
    lv_btn_set_style(btn_save,LV_BTN_STYLE_REL,&my_style_btn_release);
    //���ð�ť����̬�°���״̬��ʽ
    lv_btn_set_style(btn_save,LV_BTN_STYLE_PR,&my_style_btn_press);
    lv_obj_set_size(btn_save, 60, 30);//���ô�С
    lv_obj_set_pos(btn_save,350, 100);//��������
    btn_save_label = lv_label_create(btn_save,NULL);//�� btn_save ���� label �Ӷ���
    lv_label_set_text(btn_save_label,"Save");

    //1.������ʽ
    //1.1 ����������ʽ
    lv_style_copy(&bg_style,&lv_style_plain);
    bg_style.body.main_color = LV_COLOR_WHITE;//����ɫ����
    bg_style.body.grad_color = bg_style.body.main_color;
    bg_style.body.border.width = 1;//�߿����
    bg_style.body.border.color = LV_COLOR_MAKE(0xAA,0xAA,0xAA);
    //LV_COLOR_MAKE(0x30,0x30,0x30);
    bg_style.body.padding.left = 10;//���������ڱ߾�
    bg_style.text.color = LV_COLOR_BLACK;//�ı���ɫ
    bg_style.body.shadow.color = bg_style.body.border.color;//��Ӱ��ɫ
    bg_style.body.shadow.width = 4;//��Ӱ����
    //1.2 ����ѡ���ѡ��ʱ����ʽ
    lv_style_copy(&sel_style,&lv_style_plain);
    sel_style.body.main_color = LV_COLOR_MAKE(0x5F,0xB8,0x78);//ǳ��ɫ����
    sel_style.body.grad_color = sel_style.body.main_color;
    sel_style.text.color = LV_COLOR_WHITE;//�ı�Ϊ��ɫ
    //2.���������б���
    ddlist_mode = lv_ddlist_create(scr,NULL);
    lv_ddlist_set_options(ddlist_mode,"Single\nMulti");//�����б�ѡ��
    lv_ddlist_set_selected(ddlist_mode,0);//����Ĭ��ѡ��ֵΪ Single
    lv_ddlist_set_fix_width(ddlist_mode,100);//���ù̶�����
    lv_ddlist_set_draw_arrow(ddlist_mode,true);//ʹ�ܻ������µļ�ͷ
    lv_ddlist_set_style(ddlist_mode,LV_DDLIST_STYLE_BG,&bg_style);//���ñ�����ʽ
    lv_ddlist_set_style(ddlist_mode,LV_DDLIST_STYLE_SEL,&sel_style);//���ñ�����ʽ
    lv_obj_set_event_cb(ddlist_mode, ddlist_event_handler);//ע���¼��ص�����
    lv_obj_set_pos(ddlist_mode,125,102);//��������

    //1.������ʽ
    //1.1 ������һ�ֵ�Ԫ����ʽ,����������ͨ���ݵ�Ԫ��
    lv_style_copy(&cell1_style,&lv_style_plain);
    cell1_style.body.border.width = 1;
    cell1_style.body.border.color = LV_COLOR_BLACK;
    cell1_style.text.font = &lv_font_roboto_16;
    //1.2 �����ڶ��ֵ�Ԫ����ʽ,�������α������еĵ�Ԫ��
    lv_style_copy(&cell2_style,&lv_style_plain_color);
    cell2_style.body.border.width = 1;
    cell2_style.body.border.color = LV_COLOR_BLACK;
    cell2_style.body.main_color = LV_COLOR_SILVER;//��ɫ�ı���
    cell2_style.body.grad_color = LV_COLOR_SILVER;
    cell2_style.text.color = LV_COLOR_BLACK;
    cell2_style.text.font = &lv_font_roboto_16;

    //2.��������
    table_detail = lv_table_create(scr,NULL);
    lv_table_set_col_cnt(table_detail,TABLE_COL_CNT);//���ñ����������
    lv_table_set_row_cnt(table_detail,TABLE_ROW_CNT);//���ñ����������

    uint16_t row,col;
    for(col=0;col<TABLE_COL_CNT;col++)
    {
        //����ÿһ�еĿ���
        lv_table_set_col_width(table_detail,col,TABLE_COL_WIDTH[col]);
        for (row = 0; row < TABLE_ROW_CNT; row++) {
            //���е�Ԫ����ı����ݾ��ж���
            lv_table_set_cell_align(table_detail,row,col,LV_LABEL_ALIGN_CENTER);
        }
        lv_table_set_cell_type(table_detail,0,col,2);//���ñ�����Ϊ�ڶ��ֵ�Ԫ������
    }

    //���õ�Ԫ����ı�����
    lv_table_set_cell_value(table_detail,0,0,"No");
    lv_table_set_cell_value(table_detail,0,1,"TID");
    lv_table_set_cell_value(table_detail,0,2,"Time");
    //���ñ���ı�����ʽ,Ϊ͸��
    lv_table_set_style(table_detail,LV_TABLE_STYLE_BG,&lv_style_transp_tight);
    //���õ�һ�ֵ�Ԫ�����ʽ
    lv_table_set_style(table_detail,LV_TABLE_STYLE_CELL1,&cell1_style);
    //���õڶ��ֵ�Ԫ�����ʽ
    lv_table_set_style(table_detail,LV_TABLE_STYLE_CELL2,&cell2_style);
    lv_obj_set_pos(table_detail,30,190);//��������

    //���õ�Ԫ����ı�����
    lv_table_set_cell_value(table_detail,1,0,"0");
    lv_table_set_cell_value(table_detail,1,1,"E280F3372000F00001258DC9");
    lv_table_set_cell_value(table_detail,1,2,"5:02  2023/3/23");

}

/**
 * @brief   �����û���������
 * @param  ��
 * @retval   ��
 */
void screen_outstorage_create(void)
{
    printf("screen_outstorage_start\r\n");
    lv_screen_outstorage_start();
}

 /*********************************END OF FILE*********************************/