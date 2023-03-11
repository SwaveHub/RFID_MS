#ifndef __BSP_H
#define __BSP_H

/******************************** ͷ�ļ� *************************************/
#include "./lcd/bsp_nt35510_lcd.h"
#include "bsp_touch.h"
#include "./flash/bsp_spi_flash.h"
#include "./font/fonts.h"	
#include "./touch/bsp_i2c_touch.h"
#include "./touch/bsp_gt9147_touch.h"
#include "gpio.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "protocol.h"
#include "Unix.h"

/******************************** �궨�� *************************************/
#define delay_us    user_delaynus_tim
#define delay_ms    delay_ms_tim
/***************************** ȫ�ֱ������� **********************************/

/******************************* �������� ************************************/


#endif /* __BSP_H */
