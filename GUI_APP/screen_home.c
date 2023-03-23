/**
  ******************************************************************************
  * @file    screen_home.c
  * @author  SwaveYu
  * @version V1.0
  * @date    2023-03-23
  * @brief   lvgl screen_home create
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
#include "screen_home.h"
#include "screen_instorage.h"
#include "screen_outstorage.h"

/****************************** ˽�к궨�� ***********************************/

/******************************* ȫ�ֱ��� ************************************/

/***************************** ˽��ȫ�ֱ��� **********************************/
static lv_style_t my_style_btn_release;//��ť�ͷ�״̬�µ���ʽ
static lv_style_t my_style_btn_press;//��ť����ʱ����ʽ
    
static lv_obj_t *label_home;
static lv_obj_t *label_time;
static lv_obj_t *btn_home, *btn_home_label;
static lv_obj_t *btn_upload, *btn_upload_label;

//ͼƬ����
LV_IMG_DECLARE(btn_img_instorage);
LV_IMG_DECLARE(btn_img_outstorage);

static lv_style_t pr_style;

/******************************* ˽�к��� ************************************/

//�¼��ص�����
static void btn_upload_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
	}        
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
//        lv_obj_clean(lv_scr_act());
//        screen_home_create();
	}
		break;
	default:
		break;
	}
}

static void imgbtn_to_instorage_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_RELEASED:
	{
		lv_obj_clean(lv_scr_act());
        screen_instorage_create();
	}
		break;
	default:
		break;
	}
}

static void imgbtn_to_outstorage_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_RELEASED:
	{
		lv_obj_clean(lv_scr_act());
        screen_outstorage_create();
	}
		break;
	default:
		break;
	}
}

//�������
void lv_screen_home_start()
{
    lv_obj_t* scr = lv_scr_act();//��ȡ��ǰ��Ծ����Ļ����
    
    //���� label_home ��ǩ,������ʾ label_time ��ǩ�ĳ��ı�ģʽ
    label_home = lv_label_create(scr,NULL);//������ǩ
    lv_label_set_long_mode(label_home,LV_LABEL_LONG_BREAK);//���ó��ı�ģʽ
    //���ÿ��,һ���÷��� lv_label_set_long_mode �ĺ���,���������õ�
    lv_obj_set_width(label_home,150);
    lv_label_set_align(label_home,LV_LABEL_ALIGN_CENTER);//�ı����ж���
    lv_label_set_text(label_home,"Home");//�����ı�    
    lv_obj_set_pos(label_home, 165, 10);
    
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
    my_style_btn_release.body.shadow.width = 3;//������Ӱ�Ŀ��
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
    my_style_btn_press.body.border.width = 2;//���ñ߿�Ŀ��
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
    btn_home_label = lv_label_create(btn_home,NULL);//�� btn_home ��� label �Ӷ���
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
    btn_upload_label = lv_label_create(btn_upload,NULL);//�� btn_upload ��� label �Ӷ���
    lv_label_set_text(btn_upload_label,"Upload");
    lv_obj_set_event_cb(btn_upload,btn_upload_event_cb);//�����¼��ص�����
    
    //1.��������ʱ����ʽ
    lv_style_copy(&pr_style,&lv_style_plain);
    pr_style.image.color = LV_COLOR_BLACK;//ͼƬ�ػ�ɫʱ�Ļ��ɫΪ��ɫ,��������ȥ�а��µ�Ч��
    pr_style.image.intense = LV_OPA_50;//���ǿ��
    pr_style.text.color = LV_COLOR_MAKE(0xAA,0xAA,0xAA);//����ʱ���ı�ɫ

    //2.����imgbtn_to_instorageͼƬ��ť����
    lv_obj_t * imgbtn_to_instorage = lv_imgbtn_create(scr,NULL);
    lv_imgbtn_set_src(imgbtn_to_instorage,LV_BTN_STATE_REL,&btn_img_instorage);//��������̬����ʱ��ͼƬ
//    lv_imgbtn_set_src(imgbtn_to_instorage,LV_BTN_STATE_PR,&btn_img_instorage);//��������̬����ʱ��ͼƬ
//    lv_imgbtn_set_style(imgbtn_to_instorage,LV_BTN_STATE_PR,&pr_style);//��������̬����ʱ����ʽ
    lv_obj_set_size(imgbtn_to_instorage, 120, 80);//���ô�С
    lv_obj_set_pos(imgbtn_to_instorage,30,100);//�������� 
    lv_obj_set_event_cb(imgbtn_to_instorage,imgbtn_to_instorage_event_cb);//�����¼��ص�����
    lv_obj_t * imgbtn_to_instorage_label = lv_label_create(imgbtn_to_instorage,NULL);//��ͼƬ��ť��ӱ���
    lv_label_set_text(imgbtn_to_instorage_label, "InStorage");    

    //2.����imgbtn_to_outstorageͼƬ��ť����
    lv_obj_t * imgbtn_to_outstorage = lv_imgbtn_create(scr,NULL);
    lv_imgbtn_set_src(imgbtn_to_outstorage,LV_BTN_STATE_REL,&btn_img_outstorage);//��������̬����ʱ��ͼƬ
//    lv_imgbtn_set_src(imgbtn_to_outstorage,LV_BTN_STATE_PR,&btn_img_outstorage);//��������̬����ʱ��ͼƬ
//    lv_imgbtn_set_style(imgbtn_to_outstorage,LV_BTN_STATE_PR,&pr_style);//��������̬����ʱ����ʽ
    lv_obj_set_size(imgbtn_to_outstorage, 120, 80);//���ô�С
    lv_obj_set_pos(imgbtn_to_outstorage,180,100);//�������� 
    lv_obj_set_event_cb(imgbtn_to_outstorage,imgbtn_to_outstorage_event_cb);//�����¼��ص�����
    lv_obj_t * imgbtn_to_outstorage_label = lv_label_create(imgbtn_to_outstorage,NULL);//��ͼƬ��ť��ӱ���
    lv_label_set_text(imgbtn_to_outstorage_label, "OutStorage");    
}

/**
 * @brief   �����û���������
 * @param  ��
 * @retval   ��
 */
void screen_home_create(void)
{
    printf("screen_home_start\r\n");
    lv_screen_home_start();
}

 /*********************************END OF FILE*********************************/
