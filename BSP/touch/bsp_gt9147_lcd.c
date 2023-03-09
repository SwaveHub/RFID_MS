/**
  ******************************************************************************
  * @file    bsp_gt9147_lcd.c
  * @version V1.0
  * @date    2023-xx-xx
  * @brief   触摸屏驱动
  ******************************************************************************
  * @attention
  *
  * 
  * 
  * 
  *
  ******************************************************************************
  */ 

#include "bsp_gt9147_lcd.h"
#include "bsp.h"

/**
  * @brief  GT9147 初始化函数
  * @param  无
  * @retval 无
  */	
void GT9147_Init( void )
{
    I2C_Touch_Init();
}


/**
  * @brief  触摸屏检测状态机
  * @retval 触摸状态
	*   该返回值为以下值之一：
  *     @arg TOUCH_PRESSED :触摸按下
  *     @arg TOUCH_NOT_PRESSED :无触摸
  */
uint8_t GT9147_TouchDetect()
{
	static enumTouchState touch_state = GT9147_STATE_RELEASE;
	static uint32_t i;
	uint8_t detectResult = TOUCH_NOT_PRESSED;
	
	switch(touch_state)
	{
		case GT9147_STATE_RELEASE:	
			if(GT9147_PENIRQ_Read() == GT9147_PENIRQ_ActiveLevel) //第一次出现触摸信号
			{
				touch_state = GT9147_STATE_WAITING;
				detectResult =TOUCH_NOT_PRESSED;
				}
			else	//无触摸
			{
				touch_state = GT9147_STATE_RELEASE;
				detectResult =TOUCH_NOT_PRESSED;
			}
			break;
				
		case GT9147_STATE_WAITING:
				if(GT9147_PENIRQ_Read() == GT9147_PENIRQ_ActiveLevel)
				{
					 i++;
					//等待时间大于阈值则认为触摸被按下
					//消抖时间 = DURIATION_TIME * 本函数被调用的时间间隔
					//如在定时器中调用，每10ms调用一次，则消抖时间为：DURIATION_TIME*10ms
					if(i > DURIATION_TIME)		
					{
						i=0;
						touch_state = GT9147_STATE_PRESSED;
						detectResult = TOUCH_PRESSED;
					}
					else												//等待时间累加
					{
						touch_state = GT9147_STATE_WAITING;
						detectResult =	 TOUCH_NOT_PRESSED;					
					}
				}
				else	//等待时间值未达到阈值就为无效电平，当成抖动处理					
				{
				    i = 0;
            touch_state = GT9147_STATE_RELEASE; 
						detectResult = TOUCH_NOT_PRESSED;
				}
		
			break;
		
		case GT9147_STATE_PRESSED:	
				if(GT9147_PENIRQ_Read() == GT9147_PENIRQ_ActiveLevel)		//触摸持续按下
				{
					touch_state = GT9147_STATE_PRESSED;
					detectResult = TOUCH_PRESSED;
				}
				else	//触摸释放
				{
					touch_state = GT9147_STATE_RELEASE;
					detectResult = TOUCH_NOT_PRESSED;
				}
			break;			
		
		default:
				touch_state = GT9147_STATE_RELEASE;
				detectResult = TOUCH_NOT_PRESSED;
				break;
	
	}		
	
	return detectResult;    
}


/**
  * @brief  获取触摸点的坐标
  * @param  pDisplayCoordinate ：该指针存放获取到的触摸点坐标
  * @retval 获取情况
	*   该返回值为以下值之一：
  *     @arg 1 :获取成功
  *     @arg 0 :获取失败
  */
uint8_t GT9147_Get_TouchedPoint(strType_GT9147_Coordinate *pDisplayCoordinate)
{
    GTP_Execu(&(pDisplayCoordinate->x), &(pDisplayCoordinate->y));
}

/***************************end of file*****************************/


