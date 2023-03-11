/**
  ******************************************************************************
  * @file    demo.c
  * @author  SwaveYu
  * @version V1.0
  * @date    2023-03-09
  * @brief   lvgl demo
  *	    �޸ļ�¼ :
  *		�汾��  ����              ����             ˵��
  *         V1.0    2023-03-09  SwaveYu     ��ʽ����
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

/****************************** ˽�к궨�� ***********************************/

/******************************* ȫ�ֱ��� ************************************/

/***************************** ˽��ȫ�ֱ��� **********************************/


/******************************* ˽�к��� ************************************/

lv_obj_t * kb1;
lv_obj_t * ta1,* ta2;

//ʵ���Զ������,���Բο�lv_kb.cԴ���е�ӳ���ĸ�ʽ
//���������ڹٷ��Դ���Сд���̻���������2������,���ŵ��ո�����ұ�,һ������ͨ��@���Ű���,
//����һ��������������ռ�,��Clear��,����������⹦�ܼ�,
//littleVGL�в��Դ������⹦�ܵļ�,�������ǵ��Զ���������
#define MY_KB_CTRL_BTN_FLAGS (LV_BTNM_CTRL_NO_REPEAT | LV_BTNM_CTRL_CLICK_TRIG) //���ظ����¹���,ѡ�����ִ���

static const char * const my_kb_map_lc[] = {"1#", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "Bksp", "\n",
                                   "ABC", "a", "s", "d", "f", "g", "h", "j", "k", "l", "Enter", "\n",
                                   "_", "-", "z", "x", "c", "v", "b", "n", "m", ".", ",", ":", "\n",
                                   LV_SYMBOL_CLOSE, LV_SYMBOL_LEFT, " ",/*�Լ���ӵļ�*/ "@",/*�Լ���ӵļ�*/ "Clear", LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""};

static const lv_btnm_ctrl_t my_kb_ctrl_lc_map[] = {
    MY_KB_CTRL_BTN_FLAGS | 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7,
    MY_KB_CTRL_BTN_FLAGS | 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    MY_KB_CTRL_BTN_FLAGS | 2, 2, /*��ԭ����6��Ϊ��3*/ 3,/*�Լ���ӵļ��Ŀ�������*/ 1,/*�Լ���ӵļ��Ŀ�������*/ MY_KB_CTRL_BTN_FLAGS | 2, 2, MY_KB_CTRL_BTN_FLAGS | 2};


void kb_create(lv_obj_t * parent);

//�¼��ص�����
void event_handler(lv_obj_t * obj,lv_event_t event)
{
  if(obj==ta1||obj==ta2)
  {
    if(event==LV_EVENT_RELEASED)//�����¼�
    {
      if(kb1==NULL)
        kb_create(lv_scr_act());//������̲����ڵĻ�,���ȴ�������
      else
        lv_kb_set_ta(kb1,obj);//���°��ı������
    }
  }else if(obj==kb1)
  {
    const char * key_txt = lv_btnm_get_active_btn_text(kb1);//��ȡ���¼����ı�����,�ŵ�lv_kb_def_event_cb��ǰ�����
    lv_kb_def_event_cb(kb1,event);//���ü��̵�Ĭ���¼��ص�����,�����Ǵ����һЩͨ�õĲ���,����������Լ�д����������,�Ǿ�̫�鷳��
    //����Լ��Ĺ��ܴ���
    if(event==LV_EVENT_VALUE_CHANGED)
    {
      //uint16_t key_id = *(uint16_t*)lv_event_get_data();//��ȡ���¼���id,��һ��������idΪ0,����İ���������1
      if(key_txt==NULL)
        return;
      if(strcmp(key_txt,"Clear")==0)//���µ�����ռ�
      {
        lv_obj_t * bind_ta = lv_kb_get_ta(kb1);//��ȡ��ǰ�󶨵��ı������
        lv_ta_set_text(bind_ta,"");//�������������
      }else if(strcmp(key_txt,"abc")==0)//���µ����л�Сд���̼�
      {
        //�ض��������Լ��Զ����Сд����,������ϵͳ�Դ���Сд����
        lv_kb_set_map(kb1,(const char **)my_kb_map_lc);//�����Զ��尴����ӳ���
        lv_kb_set_ctrl_map(kb1,my_kb_ctrl_lc_map);//�����Զ��尴�������Կ���ӳ���
      }
    }else if(event==LV_EVENT_APPLY)
    {
      /*
        ������Ը����û�������ı���Ϣ,����Ӧ��ҵ���߼�����
      */
      lv_obj_del(kb1);//����̼�ʱ�Ѽ���ɾ����
      kb1 = NULL;
    }else if(event==LV_EVENT_CANCEL)
    {
      /*
        ����Ҳ���Ը����û�������ı���Ϣ,����Ӧ��ҵ���߼�����
      */
      lv_obj_del(kb1);//�������ʱ�Ѽ���ɾ����
      kb1 = NULL;
    }
  }
}

//��������
void kb_create(lv_obj_t * parent)
{
  kb1 = lv_kb_create(parent,NULL);
  lv_kb_set_cursor_manage(kb1,true);//ʹ�ܶԹ��Ľӹ�
  lv_kb_set_mode(kb1,LV_KB_MODE_TEXT);//����Ϊ�ı�����ģʽ,��Ҳ��Ĭ��ֵ
  lv_kb_set_ta(kb1,ta1);//��Ĭ�ϰ�ta1�ı������,�������ͨ�����ĳ�ı��������°󶨵���Ӧ���ı������
  lv_kb_set_map(kb1,(const char **)my_kb_map_lc);//�����Զ��尴����ӳ���
  lv_kb_set_ctrl_map(kb1,my_kb_ctrl_lc_map);//�����Զ��尴�������Կ���ӳ���
  lv_obj_set_event_cb(kb1,event_handler);//�����Զ�����¼��ص�����
}


//�������
void lv_kb_test_start()
{
  lv_obj_t *scr = lv_scr_act();//��ȡ��ǰ��Ծ����Ļ����
  lv_obj_set_click(scr,true);
  lv_obj_set_event_cb(scr,event_handler);
  //1.���������ı������
  //1.1 ����ta1�ı������
  ta1 = lv_ta_create(scr,NULL);
  lv_obj_set_width(ta1,200);//���ÿ��
  lv_obj_align(ta1,NULL,LV_ALIGN_IN_TOP_MID,0,15);
  lv_ta_set_one_line(ta1,true);//ʹ�ܵ���ģʽ,�ڵ���ģʽ��,�ı���ĸ߶��ǲ��ܱ����õ�
  lv_ta_set_text(ta1,"");//����Ϊ���ı�
  lv_obj_set_event_cb(ta1,event_handler);//�����¼��ص�����
  //1.2 ����ta2�ı������
  ta2 = lv_ta_create(scr,ta1);//ֱ�Ӵ�ta1���и���
  lv_ta_set_pwd_mode(ta2,true);//ʹ������ģʽ
  lv_ta_set_cursor_type(ta2,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//�����ص����
  lv_obj_align(ta2,ta1,LV_ALIGN_OUT_BOTTOM_MID,0,15);

  //2.�������̶���,���������ļ���Ĭ�����븸����ײ����ж����,
  //Ĭ�Ͽ�ȵ��ڸ�����Ŀ���Ӧ���(�����������ڱ߾��Ŀ��),
  //Ĭ�ϸ߶ȵ��ڸ�����Ŀ���Ӧ�߶�(�����������ڱ߾��ĸ߶�)��һ��
  kb_create(scr);
}

/**
 * @brief   �����û���������
 * @param  ��
 * @retval   ��
 */
void demo_create(void)
{
    printf("demo_start\r\n");
    lv_kb_test_start();    
}

 /*********************************END OF FILE*********************************/
