#ifndef __BSP_GT9147_LCD_H
#define	__BSP_GT9147_LCD_H


#include "stm32f1xx_hal.h"

/******************************* GT9147 �����������ź�ָʾ���Ŷ���(��ʹ���ж�) ***************************/
#define             GT9147_PENIRQ_GPIO_PORT                       GPIOF
#define             GT9147_PENIRQ_GPIO_PIN                        GPIO_PIN_10

/******************************* ���� GT9147 ��ص��������� ***************************/
//�����ź���Ч��ƽ
#define             GT9147_PENIRQ_ActiveLevel                     0
#define             GT9147_PENIRQ_Read()                          HAL_GPIO_ReadPin( GT9147_PENIRQ_GPIO_PORT, GT9147_PENIRQ_GPIO_PIN )

typedef	struct          //Һ������ṹ�� 
{		
	/*����ֵ��ʾ��������*/
   int16_t x;			//��¼���µĴ�������ֵ
   int16_t y; 
	
	/*���ڼ�¼��������ʱ(����)����һ�δ���λ��*/
   int16_t pre_x;		
   int16_t pre_y;
	
} strType_GT9147_Coordinate;   

/******����״̬�����******/
typedef enum
{
	GT9147_STATE_RELEASE  = 0,	//�����ͷ�
	GT9147_STATE_WAITING,			//��������
	GT9147_STATE_PRESSED,			//��������
}enumTouchState	;

#define TOUCH_PRESSED 				1
#define TOUCH_NOT_PRESSED			0

//����������ֵ
#define DURIATION_TIME				2

/******************************* ���� GT9147 ��ص��ⲿȫ�ֱ��� ***************************/




/******************************** GT9147 �������������� **********************************/
void GT9147_Init( void );
uint8_t GT9147_TouchDetect();
uint8_t GT9147_Get_TouchedPoint();

#endif /* __BSP_TOUCH_H */

