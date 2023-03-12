#ifndef __KEY_H
#define	__KEY_H

#include "stm32f1xx_hal.h"
#include "main.h"
//���Ŷ���
/*******************************************************/
#define SCANKEY_PIN                  KEY0_Pin                 
#define SCANKEY_GPIO_PORT            KEY0_GPIO_Port

/*******************************************************/

 /** �������±��ú�
	* ��������Ϊ�ߵ�ƽ������ KEY_ON=1�� KEY_OFF=0
	* ����������Ϊ�͵�ƽ���Ѻ����ó�KEY_ON=0 ��KEY_OFF=1 ����
	*/
#define KEY_ON	0
#define KEY_OFF	1

#define KEY_PRESS	1
#define KEY_RELEASE	2

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif /* __KEY_H */

