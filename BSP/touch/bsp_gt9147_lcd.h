#ifndef __BSP_GT9147_LCD_H
#define	__BSP_GT9147_LCD_H


#include "stm32f1xx_hal.h"

/******************************* GT9147 触摸屏触摸信号指示引脚定义(不使用中断) ***************************/
#define             GT9147_PENIRQ_GPIO_PORT                       GPIOF
#define             GT9147_PENIRQ_GPIO_PIN                        GPIO_PIN_10

/******************************* 声明 GT9147 相关的数据类型 ***************************/
//触屏信号有效电平
#define             GT9147_PENIRQ_ActiveLevel                     0
#define             GT9147_PENIRQ_Read()                          HAL_GPIO_ReadPin( GT9147_PENIRQ_GPIO_PORT, GT9147_PENIRQ_GPIO_PIN )

typedef	struct          //液晶坐标结构体 
{		
	/*负数值表示无新数据*/
   int16_t x;			//记录最新的触摸参数值
   int16_t y; 
	
	/*用于记录连续触摸时(长按)的上一次触摸位置*/
   int16_t pre_x;		
   int16_t pre_y;
	
} strType_GT9147_Coordinate;   

/******触摸状态机相关******/
typedef enum
{
	GT9147_STATE_RELEASE  = 0,	//触摸释放
	GT9147_STATE_WAITING,			//触摸按下
	GT9147_STATE_PRESSED,			//触摸按下
}enumTouchState	;

#define TOUCH_PRESSED 				1
#define TOUCH_NOT_PRESSED			0

//触摸消抖阈值
#define DURIATION_TIME				2

/******************************* 声明 GT9147 相关的外部全局变量 ***************************/




/******************************** GT9147 触摸屏函数声明 **********************************/
void GT9147_Init( void );
uint8_t GT9147_TouchDetect();
uint8_t GT9147_Get_TouchedPoint();

#endif /* __BSP_TOUCH_H */

